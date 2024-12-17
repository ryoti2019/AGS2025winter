#include "../Lib/ImGui/imgui.h"
#include "../Application.h"
#include "../Common/Quaternion.h"
#include "../Object/Common/Transform.h"
#include "../Object/Common/ModelMaterial.h"
#include "../Object/Common/Renderer.h"
#include "StageBase.h"

StageBase::StageBase(const VECTOR& pos, const json& data)
	:
	ActorBase(pos,data),
	restrictedAreaRadius_(10000.0f)
{
}

void StageBase::Init(const VECTOR& pos)
{

	// ���ʕ����͊��N���X�ŏ�����
	ActorBase::Init(pos);

	// �`��p�̃R���|�[�l���g
	drawComponent_ = std::make_unique<DrawComponent>(std::static_pointer_cast<StageBase>(GetThis()));

}

void StageBase::Update(const float deltaTime)
{
}

void StageBase::Draw(const float deltaTime)
{

	ActorBase::Draw(deltaTime);

}

void StageBase::CollisionDraw(const float deltaTime)
{

	if (isStageCollision_ || isDissolve_)
	{

		// �Q�[�g�~�X�g�̕`��
		deltaTime_ += deltaTime;
		if (isDissolve_)
		{
			alphaTime_ += 0.005f;
		}
		modelMaterial_->SetConstBufsPS({ 0.0f,0.0f,0.0f,deltaTime_ }, 2);
		modelMaterial_->SetConstBufsPS({ alphaTime_,0.0f,0.0f,0.0f }, 3);
		modelMaterial_->SetTextureAddress(ModelMaterial::TEXADDRESS::MIRROR);
		renderer_->Draw();
	}

}

void StageBase::CollisionInit(std::shared_ptr<Transform>& transform)
{

	// �e�N�X�`��
	texId_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::IMAGE_NOISE_TEXTURE)]).handleId_;

	// ���f���`��p
	std::vector<FLOAT4> constBufsPtrVS;
	//constBufsPtrVS.push_back({ 1.0f, 1.0f, 1.0f, 1.0f });

	std::vector<FLOAT4> constBufsPtrPS;
	// �g�U��
	constBufsPtrPS.push_back({ 1.0f, 1.0f, 1.0f, 1.0f });

	// ����
	constBufsPtrPS.push_back({ 0.2f, 0.2f, 0.2f, 1.0f });

	// ���̌����Ă������(���[���h���)(�f�B���N�V���i�����C�g)
	auto lDir = GetLightDirection();
	deltaTime_ = 0.0f;
	constBufsPtrPS.push_back({ lDir.x,lDir.y,lDir.z, deltaTime_ });
	alphaTime_ = 0.0f;
	constBufsPtrPS.push_back({ alphaTime_,0.0f,0.0f,0.0f });

	std::map<int, int> textures;
	textures.emplace(1, texId_);
	modelMaterial_ = std::make_shared<ModelMaterial>(
		(Application::PATH_SHADER + "GateModelVS.cso"), sizeof(FLOAT4) * 1, constBufsPtrVS,
		(Application::PATH_SHADER + "GateModelPS.cso"), sizeof(FLOAT4) * 4, constBufsPtrPS, textures
	);

	renderer_ = std::make_shared<Renderer>(transform->modelId, modelMaterial_);

	isDissolve_ = false;

}