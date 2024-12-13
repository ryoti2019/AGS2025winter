#include "../Lib/ImGui/imgui.h"
#include "../Application.h"
#include "../Common/Quaternion.h"
#include "../Object/Common/Transform.h"
#include "../Object/Common/ModelMaterial.h"
#include "../Object/Common/Renderer.h"
#include "Stage.h"

Stage::Stage(const VECTOR& pos, const json& data)
	:
	ActorBase(pos,data)
{

	// モデルID
	modelId_ = resMng_.LoadModelDuplicate(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::STAGE_MODEL)]);

	// モデル制御の基本情報を生成
	stageCollision_ = std::make_unique<Transform>();
	area1Collision_ = std::make_unique<Transform>();
	area2Collision_ = std::make_unique<Transform>();
	area3Collision_ = std::make_unique<Transform>();
	area4Collision_ = std::make_unique<Transform>();
	area5Collision_ = std::make_unique<Transform>();

	// 衝突判定のモデルID
	stageCollisionModelId_ = resMng_.LoadModelDuplicate(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::STAGE_COLLISION_MODEL)]);
	area1CollisionModelId_ = resMng_.LoadModelDuplicate(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::STAGE_AREA1_COLLISION_MODEL)]);
	area2CollisionModelId_ = resMng_.LoadModelDuplicate(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::STAGE_AREA2_COLLISION_MODEL)]);
	area3CollisionModelId_ = resMng_.LoadModelDuplicate(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::STAGE_AREA3_COLLISION_MODEL)]);
	area4CollisionModelId_ = resMng_.LoadModelDuplicate(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::STAGE_AREA4_COLLISION_MODEL)]);
	area5CollisionModelId_ = resMng_.LoadModelDuplicate(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::STAGE_AREA5_COLLISION_MODEL)]);

	// ステージの衝突判定しているかどうか
	isStageCollision_ = true;
	isArea1Collision_ = false;
	isArea2Collision_ = false;
	isArea3Collision_ = false;
	isArea4Collision_ = false;
	isArea5Collision_ = false;

	// モデル制御の基本情報を初期化
	CollisionInit(stageCollision_,stageCollisionModelId_, isStageCollision_);
	CollisionInit(area1Collision_, area1CollisionModelId_, isArea1Collision_);
	CollisionInit(area2Collision_, area2CollisionModelId_, isArea2Collision_);
	CollisionInit(area3Collision_, area3CollisionModelId_, isArea3Collision_);
	CollisionInit(area4Collision_, area4CollisionModelId_, isArea4Collision_);
	CollisionInit(area5Collision_, area5CollisionModelId_, isArea5Collision_);

	// 共通部分は基底クラスで初期化
	ActorBase::Init(pos);

}

void Stage::Init(const VECTOR& pos)
{

	// 共通部分は基底クラスで初期化
	ActorBase::Init(pos);

	// 描画用のコンポーネント
	drawComponent_ = std::make_unique<DrawComponent>(std::static_pointer_cast<Stage>(GetThis()));

}

void Stage::Update(const float deltaTime)
{



}

void Stage::Draw(const float deltaTime)
{

	// ゲートミストの描画
	deltaTime_ += deltaTime;
	if (isDissolve_)
	{
		alphaTime_ -= 0.01f;
		time_ += 0.01f;
	}
	modelMaterial_->SetConstBufsPS({ 0.0f,0.0f,0.0f,deltaTime_ }, 2);
	modelMaterial_->SetConstBufsPS({ alphaTime_,time_,0.0f,0.0f }, 3);
	modelMaterial_->SetTextureAddress(ModelMaterial::TEXADDRESS::MIRROR);
	renderer_->Draw();

}

void Stage::CollisionInit(std::unique_ptr<Transform>& transform, const int modelId, const bool isCollision)
{

	// 初期化
	transform->SetModel(modelId);
	transform->pos = { 0.0f, -20000.0f, 0.0f };
	transform->scl = { 10.0f,10.0f,10.0f };
	transform->quaRot = Quaternion::Euler({ Utility::Deg2RadF(0.0f) , Utility::Deg2RadF(0.0f),Utility::Deg2RadF(0.0f) });
	transform->quaRotLocal = Quaternion::Euler({ Utility::Deg2RadF(0.0f) , Utility::Deg2RadF(180.0f),Utility::Deg2RadF(0.0f) });
	transform->Update();

	// 衝突判定データ
	CollisionModel data;

	data.isActive = isCollision;
	data.modelId = transform->modelId;

	// 衝突モデルリストに追加
	collisionModels_.emplace_back(data);

	// テクスチャ
	texId_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::IMAGE_NOISE_TEXTURE)]).handleId_;

	// モデル描画用
	std::vector<FLOAT4> constBufsPtrVS;
	//constBufsPtrVS.push_back({ 1.0f, 1.0f, 1.0f, 1.0f });

	std::vector<FLOAT4> constBufsPtrPS;
	// 拡散光
	constBufsPtrPS.push_back({ 1.0f, 1.0f, 1.0f, 1.0f });

	// 環境光
	constBufsPtrPS.push_back({ 0.2f, 0.2f, 0.2f, 1.0f });

	// 光の向いている方向(ワールド空間)(ディレクショナルライト)
	auto lDir = GetLightDirection();
	deltaTime_ = 0.0f;
	constBufsPtrPS.push_back({ lDir.x,lDir.y,lDir.z, deltaTime_ });
	alphaTime_ = 0.0f;
	time_ = 1.0f;
	constBufsPtrPS.push_back({ alphaTime_,time_,0.0f,0.0f });

	std::map<int, int> textures;
	textures.emplace(1, texId_);
	modelMaterial_ = std::make_shared<ModelMaterial>(
		(Application::PATH_SHADER + "GateModelVS.cso"), sizeof(FLOAT4) * 1, constBufsPtrVS,
		(Application::PATH_SHADER + "GateModelPS.cso"), sizeof(FLOAT4) * 4, constBufsPtrPS, textures
	);

	renderer_ = std::make_shared<Renderer>(transform->modelId, modelMaterial_);

	isDissolve_ = false;

}