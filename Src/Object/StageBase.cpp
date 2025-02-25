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

	// �`��p�̃R���|�[�l���g��������
	drawComponent_ = std::make_unique<DrawComponent>(shared_from_this());

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
			alphaTime_ += IMG1_ALPHA_INCREMENT;
		}
		modelMaterial_->SetConstBufsPS({ 0.0f,0.0f,0.0f,deltaTime_ }, PS_CONST_BUF_SLOT_2);
		modelMaterial_->SetConstBufsPS({ alphaTime_,0.0f,0.0f,0.0f }, PS_CONST_BUF_SLOT_3);
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

	std::vector<FLOAT4> constBufsPtrPS;

	// �g�U��
	constBufsPtrPS.push_back(DIFFUSE_LIGHT);

	// ����
	constBufsPtrPS.push_back(AMBIENT_LIGHT);

	// ���̌����Ă������(���[���h���)(�f�B���N�V���i�����C�g)
	auto lDir = GetLightDirection();
	deltaTime_ = 0.0f;
	constBufsPtrPS.push_back({ lDir.x,lDir.y,lDir.z, deltaTime_ });
	alphaTime_ = 0.0f;
	constBufsPtrPS.push_back({ alphaTime_,0.0f,0.0f,0.0f });

	std::map<int, int> textures;
	textures.emplace(TEXTURE_SLOT_DIFFUSE, texId_);
	modelMaterial_ = std::make_shared<ModelMaterial>(
		(Application::PATH_SHADER + "GateModelVS.cso"), VS_CONST_BUF_SIZE, constBufsPtrVS,
		(Application::PATH_SHADER + "GateModelPS.cso"), PS_CONST_BUF_SIZE, constBufsPtrPS, textures
	);

	renderer_ = std::make_shared<Renderer>(transform->modelId, modelMaterial_);

	isDissolve_ = false;

}