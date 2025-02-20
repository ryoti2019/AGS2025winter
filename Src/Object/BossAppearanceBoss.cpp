#include "../Lib/ImGui/imgui.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "BossAppearanceBoss.h"

BossAppearanceBoss::BossAppearanceBoss(const VECTOR& pos, const json& data)
	:
	EnemyBase(pos, data),
	isLand_(false)
{

	// �@�\�̏�����
	InitComponent();

	// ���f��ID
	modelId_ = resMng_.LoadModelDuplicate(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::MODEL_BOSS)]);

	// �֐��|�C���^�̏�����
	InitFunctionPointer();

	// ���ʕ����͊��N���X�ŏ�����
	ActorBase::Init(pos);

	// �p�����[�^�̏�����
	InitParameter();

	// �A�j���[�V�����̏�����
	InitAnimation();

}

void BossAppearanceBoss::Init(const VECTOR& pos)
{

	// �`��p�̃R���|�[�l���g
	drawComponent_ = std::make_unique<DrawComponent>(std::static_pointer_cast<BossAppearanceBoss>(GetThis()));

}

void BossAppearanceBoss::InitComponent()
{

	// �J�����𐶐�
	std::weak_ptr<Camera> camera = SceneManager::GetInstance().GetCamera();

	// �J�����̃^�[�Q�b�g���v���C���[�ɐݒ�
	camera.lock()->SetBoss(transform_);

}

void BossAppearanceBoss::InitParameter()
{

	// �E��̃t���[����
	RIGHT_HAND_FRAME = jsonData_["RIGHT_HAND_FRAME_NAME"];

	// ����̃t���[����
	LEFT_HAND_FRAME = jsonData_["LEFT_HAND_FRAME_NAME"];

	// �E���̃t���[����
	RIGHT_FOOT_FRAME = jsonData_["RIGHT_FOOT_FRAME_NAME"];

	// �����̃t���[����
	LEFT_FOOT_FRAME = jsonData_["LEFT_FOOT_FRAME_NAME"];

	// �̂̃t���[����
	BODY_FRAME = jsonData_["BODY_FRAME_NAME"];

	// �E��̃t���[���ԍ����擾
	collisionData_.rightHand = MV1SearchFrame(transform_->modelId, RIGHT_HAND_FRAME.c_str());

	// ����̃t���[���ԍ����擾
	collisionData_.leftHand = MV1SearchFrame(transform_->modelId, LEFT_HAND_FRAME.c_str());

	// �E���̃t���[���ԍ����擾
	collisionData_.rightFoot = MV1SearchFrame(transform_->modelId, RIGHT_FOOT_FRAME.c_str());

	// �����̃t���[���ԍ����擾
	collisionData_.leftFoot = MV1SearchFrame(transform_->modelId, LEFT_FOOT_FRAME.c_str());

	// �̂̃t���[���ԍ����擾
	collisionData_.body = MV1SearchFrame(transform_->modelId, BODY_FRAME.c_str());

	// �葫�̏Փ˔���̔��a
	collisionData_.handAndFootCollisionRadius = HAND_AND_FOOT_COLLISION_RADIUS;

	// �̂̏Փ˔���̔��a
	collisionData_.bodyCollisionRadius = BODY_COLLISION_RADIUS;

	// �A�j���[�V�����ԍ�
	ANIM_INDEX = jsonData_["ANIM_INDEX"];

}

void BossAppearanceBoss::InitAnimation()
{

	// �A�j���[�V�����R���g���[���[�̐���
	animationController_ = std::make_unique<AnimationController>(transform_->modelId);

	// �A�j���[�V�����R���g���[���[�ɃA�j���[�V������ǉ�
	for (int i = static_cast<int>(BossState::IDLE); i < static_cast<int>(BossState::MAX); ++i)
	{

		// ���[�v�Đ�����A�j���[�V��������isLoop��true�ɂ���
		bool isLoop = i == static_cast<int>(BossState::IDLE) || i == static_cast<int>(BossState::RUN) || i == static_cast<int>(BossState::JUMP_IDLE);
		animationController_->Add(

			// �A�j���[�V�����̖��O
			jsonData_["ANIM"][i - 1]["NAME"],

			// �A�j���[�V�������n�܂鎞��
			0.0f,

			// �A�j���[�V�����X�s�[�h
			jsonData_["ANIM"][i - 1]["SPEED"],

			// �A�j���[�V�����n���h��
			resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::MODEL_BOSS) + i]).handleId_,

			// �A�j���[�V�����̃��[�v�Đ�
			isLoop,

			// �A�j���[�V�����ԍ�
			ANIM_INDEX,

			// �A�j���[�V�����̋t�Đ�
			false
		);
	}

	// �A�j���[�V�����Đ�����L�[
	key_ = "";

	// 1�O�̃A�j���[�V����
	preKey_ = "";

	// �������
	ChangeState(BossState::IDLE);

}

void BossAppearanceBoss::InitFunctionPointer()
{

	//�֐��|�C���^�̏�����
	stateChange_.emplace(BossState::IDLE, std::bind(&BossAppearanceBoss::ChangeIdle, this));
	stateChange_.emplace(BossState::JUMP_IDLE, std::bind(&BossAppearanceBoss::ChangeJumpIdle, this));
	stateChange_.emplace(BossState::LAND, std::bind(&BossAppearanceBoss::ChangeLand, this));

}

void BossAppearanceBoss::Update(const float deltaTime)
{

	// ImGui�̃f�o�b�O�`��̍X�V
	//UpdateDebugImGui();

	// �Փ˔���̍X�V
	ActorBase::CollisionUpdate();

	// ��Ԃ��Ƃ̍X�V
	stateUpdate_(deltaTime);

	// �d��
	Gravity(gravityScale_);

	// ���f�������X�V
	transform_->Update();

	// �A�j���[�V�����Đ�
	animationController_->Update(deltaTime);

}

void BossAppearanceBoss::ChangeState(const BossState state)
{

	// ��ԑJ��
	state_ = state;

	// �֐��|�C���^�̑J��
	stateChange_[state_]();

	// �O�̃A�j���[�V������ۑ�
	preKey_ = key_;

	// �V�����A�j���[�V������ۑ�
	key_ = ANIM_DATA_KEY[static_cast<int>(state)];

	// �A�j���[�V�����R���g���[���[���̃A�j���[�V�����J��
	animationController_->ChangeAnimation(key_);

}

void BossAppearanceBoss::ChangeIdle()
{
	stateUpdate_ = std::bind(&BossAppearanceBoss::UpdateIdle, this, std::placeholders::_1);
}

void BossAppearanceBoss::ChangeJumpIdle()
{

	stateUpdate_ = std::bind(&BossAppearanceBoss::UpdateJumpIdle, this, std::placeholders::_1);

	// ���W��ݒ�
	transform_->pos = { -10800.0f,11000.0f,-140000.0f };

	// �Փ˔���̍X�V
	ActorBase::CollisionUpdate();

	gravityScale_ = 1.0f;

}

void BossAppearanceBoss::ChangeLand()
{
	stateUpdate_ = std::bind(&BossAppearanceBoss::UpdateLand, this, std::placeholders::_1);
}

void BossAppearanceBoss::UpdateIdle(const float deltaTime)
{

	// �J�����𐶐�
	std::weak_ptr<Camera> camera = SceneManager::GetInstance().GetCamera();

	// �J������1�ڂ�2�ڂ̓������I����Ă��邩
	if (!camera.lock()->GetIsBossAppearanceCameraMove1() && !camera.lock()->GetIsBossAppearanceCameraMove2() && !isLand_)
	{
		// �W�����v�ɑJ��
		ChangeState(BossState::JUMP_IDLE);
	}

}

void BossAppearanceBoss::UpdateJumpIdle(const float deltaTime)
{

	// ���ɂ��Ă��邩
	if (isOnGround_)
	{
		// �N���オ��A�j���[�V�����ɑJ��
		ChangeState(BossState::LAND);
	}

}

void BossAppearanceBoss::UpdateLand(const float deltaTime)
{

	// �A�j���[�V�����Đ����I����Ă��邩
	if (animationController_->IsEndPlayAnimation())
	{

		// �ҋ@�ɑJ��
		ChangeState(BossState::IDLE);

		// ���n����
		isLand_ = true;

	}

}

void BossAppearanceBoss::UpdateDebugImGui()
{

	// �E�B���h�E�^�C�g��&�J�n����
	ImGui::Begin("Boss");

	// �傫��
	ImGui::Text("scale");
	ImGui::InputFloat("Scl", &scl_);

	// �p�x
	VECTOR rotDeg = VECTOR();
	rotDeg.x = Utility::Rad2DegF(transform_->quaRot.x);
	rotDeg.y = Utility::Rad2DegF(transform_->quaRot.y);
	rotDeg.z = Utility::Rad2DegF(transform_->quaRot.z);
	ImGui::Text("angle(deg)");
	ImGui::SliderFloat("RotX", &rotDeg.x, -90.0f, 90.0f);
	ImGui::SliderFloat("RotY", &rotDeg.y, -90.0f, 90.0f);
	ImGui::SliderFloat("RotZ", &rotDeg.z, -90.0f, 90.0f);
	transform_->quaRot.x = Utility::Deg2RadF(rotDeg.x);
	transform_->quaRot.y = Utility::Deg2RadF(rotDeg.y);
	transform_->quaRot.z = Utility::Deg2RadF(rotDeg.z);

	// �ʒu
	ImGui::Text("position");
	// �\���̂̐擪�|�C���^��n���Axyz�ƘA�������������z�u�փA�N�Z�X
	ImGui::InputFloat3("Pos", &transform_->pos.x);
	// �I������
	ImGui::End();

}