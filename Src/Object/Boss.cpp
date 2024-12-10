#include "../Lib/ImGui/imgui.h"
#include "Boss.h"

Boss::Boss(const VECTOR& pos, const json& data)
	:
	EnemyBase(pos, data),
	ATTACK_PROJECTILE_START_FRAME(data["ANIM"][static_cast<int>(BossState::ATTACK_PROJECTILE) - 1]["ATTACK_START_FRAME"]),
	ATTACK_PROJECTILE_DAMAGE(data["ANIM"][static_cast<int>(BossState::ATTACK_PROJECTILE) - 1]["DAMAGE"]),
	ATTACK_PROJECTILE_COLLISION_TIME(data["ATTACK_PROJECTILE_COLLISION_TIME"]),
	projectileDir_({0.0f,0.0f,0.0f}),
	projectileCollisionCnt_(0.0f)
{

	// �@�\�̏�����
	InitFunction();

	// ���f��ID
	modelId_ = resMng_.LoadModelDuplicate(ResourceManager::SRC::MODEL_BOSS);

	// �֐��|�C���^�̏�����
	InitFunctionPointer();

	// ���ʕ����͊��N���X�ŏ�����
	ActorBase::Init(pos);

	// �p�����[�^�̏�����
	InitParameter();

	// ���ʕ����͊��N���X�ŏ�����
	ActorBase::Init(pos);

	// �A�j���[�V�����̏�����
	InitAnimation();

	// �G�t�F�N�g�̏�����
	InitEffect();

}

void Boss::Init(const VECTOR& pos)
{

	// �@�\�̏�����
	InitFunction();

	// ���f��ID
	modelId_ = resMng_.LoadModelDuplicate(ResourceManager::SRC::MODEL_BOSS);

	// ���ʕ����͊��N���X�ŏ�����
	ActorBase::Init(pos);

	// �֐��|�C���^�̏�����
	InitFunctionPointer();

	// �G���m�̋��ʕ����̏�����
	EnemyBase::Init();

	// �p�����[�^�̏�����
	InitParameter();

	// �A�j���[�V�����̏�����
	InitAnimation();

	//�G�t�F�N�g�̏�����
	InitEffect();

	// AI�R���|�[�l���g��ǉ�
	aiComponent_ = std::make_unique<BossAIComponent>(std::static_pointer_cast<Boss>(GetThis()));

	// �ړ��p�̃R���|�[�l���g��ǉ�
	moveComponent_ = std::make_unique<MoveComponent>(std::static_pointer_cast<Boss>(GetThis()));

	// �`��p�̃R���|�[�l���g
	drawComponent_ = std::make_unique<DrawComponent>(std::static_pointer_cast<Boss>(GetThis()));

}

void Boss::InitFunction()
{
}

void Boss::InitParameter()
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

	// ��ѓ���̓����蔻��̍��W��ݒ�
	collisionData_.projectilePos = VAdd(transform_->pos, BODY_RELATIVE_CENTER_POS);

	// �葫�̏Փ˔���̔��a
	collisionData_.handAndFootCollisionRadius = HAND_AND_FOOT_COLLISION_RADIUS;

	// �̂̏Փ˔���̔��a
	collisionData_.bodyCollisionRadius = BODY_COLLISION_RADIUS;

	// ��ѓ���̏Փ˔���̔��a
	collisionData_.projectileCollisionRadius = jsonData_["PROJECTILE_COLLISION_RADIUS"];

	// �E��ɍU�����肪���邩�ǂ���
	collisionData_.isRightHandAttack = false;

	// ����ɍU�����肪���邩�ǂ���
	collisionData_.isLeftHandAttack = false;

	// �E���ɍU�����肪���邩�ǂ���
	collisionData_.isRightFootAttack = false;

	// �����ɍU�����肪���邩�ǂ���
	collisionData_.isLeftFootAttack = false;

	// ��ѓ���ɍU�����肪���邩�ǂ���
	collisionData_.isProjectileAttack = false;

	// �v���C���[�̍��W
	std::optional<VECTOR> playerPos = GetPlayerPos();

	// ����̍��W
	targetPos_ = playerPos.value();

	// �p���`�̍U���J�n�t���[��
	PUNCH_ATTACK_START_FRAME = jsonData_["ANIM"][static_cast<int>(BossState::ATTACK_PUNCH) - 1]["ATTACK_START_FRAME"];

	// �p���`�̍U���I���t���[��
	PUNCH_ATTACK_END_FRAME = jsonData_["ANIM"][static_cast<int>(BossState::ATTACK_PUNCH) - 1]["ATTACK_END_FRAME"];

	// �L�b�N�̍U���J�n�t���[��
	KICK_ATTACK_START_FRAME = jsonData_["ANIM"][static_cast<int>(BossState::ATTACK_KICK) - 1]["ATTACK_START_FRAME"];

	// �L�b�N�̍U���I���t���[��
	KICK_ATTACK_END_FRAME = jsonData_["ANIM"][static_cast<int>(BossState::ATTACK_KICK) - 1]["ATTACK_END_FRAME"];

	// ������Ԏ��̏����
	HIT_FLY_UP_VEC_POW = jsonData_["HIT_FLY_UP_VEC_POW"];

	// ������Ԏ��̈ړ���
	HIT_FLY_MOVE_POW = jsonData_["HIT_FLY_MOVE_POW"];

	// �܂��������ł�������
	KNOCK_BACK_TIME = jsonData_["KNOCK_BACK_TIME"];

	// �܂��������ł����Ƃ��������鍂��
	KNOCK_BACK_HEIGHT_OFFSET = jsonData_["KNOCK_BACK_HEIGHT_OFFSET"];

	// ��ɔ��ł����Ƃ��̏�����̗�
	FLINCH_UP_UP_VEC_POW = jsonData_["FLINCH_UP_UP_VEC_POW"];

	// ������ɔ��ł����Ƃ��̏�����̗�
	FLINCH_UP_UP_VEC_SMALL_POW = jsonData_["FLINCH_UP_UP_VEC_SMALL_POW"];

	// ��ɔ��ł����Ƃ��̃X�s�[�h
	FLINCH_UP_SPEED = jsonData_["FLINCH_UP_SPEED"];

	// ��ɔ��ł����Ƃ���X���̊p�x
	FLINCH_UP_ANGLE_X = jsonData_["FLINCH_UP_ANGLE_X"];

	// ��ɔ��ł����Ƃ��̏d�͂��ɂ����鋭��
	FLINCH_UP_GRAVITY_SCALE = jsonData_["FLINCH_UP_GRAVITY_SCALE"];

	// ����Ƃ��̈ړ���
	RUN_MOVE_POW = jsonData_["RUN_MOVE_POW"];

	// �A�j���[�V�����ԍ�
	ANIM_INDEX = jsonData_["ANIM_INDEX"];

}

void Boss::InitAnimation()
{

	// �A�j���[�V�����R���g���[���[�̐���
	animationController_ = std::make_unique<AnimationController>(transform_->modelId);

	// �A�j���[�V�����R���g���[���[�ɃA�j���[�V������ǉ�
	for (int i = static_cast<int>(BossState::IDLE); i < static_cast<int>(BossState::MAX); ++i)
	{

		// ���[�v�Đ�����A�j���[�V��������isLoop��true�ɂ���
		bool isLoop = i == static_cast<int>(BossState::IDLE) || i == static_cast<int>(BossState::RUN);
		animationController_->Add(

			// �A�j���[�V�����̖��O
			jsonData_["ANIM"][i - 1]["NAME"],

			// �A�j���[�V�������n�܂鎞��
			0.0f,

			// �A�j���[�V�����X�s�[�h
			jsonData_["ANIM"][i - 1]["SPEED"],

			// �A�j���[�V�����n���h��
			resMng_.LoadModelDuplicate(static_cast<ResourceManager::SRC>(static_cast<int>(ResourceManager::SRC::MODEL_BOSS) + i)),

			// �A�j���[�V�����̃��[�v�Đ�
			isLoop,

			// �A�j���[�V�����ԍ�
			ANIM_INDEX,

			// �A�j���[�V�����̋t�Đ�
			false
		);
	}

	// �A�j���[�V�����Đ�����L�[
	key_ = "IDLE";

	// 1�O�̃A�j���[�V����
	preKey_ = key_;

	// �������
	ChangeState(BossState::IDLE);

}

void Boss::InitEffect(void)
{

	// �G�t�F�N�g�R���g���[���[�̐���
	effekseerController_ = std::make_unique<EffekseerController>();

	// �G�t�F�N�g�R���g���[���[�ɃA�j���[�V������ǉ�
	for (int i = static_cast<int>(EffectData::PROJECTILE); i < static_cast<int>(EffectData::MAX); ++i)
	{

		effekseerController_->Add(

			// �G�t�F�N�g�̖��O
			jsonData_["EFFECT"][i - 1]["NAME"],

			// �G�t�F�N�g�n���h��
			resMng_.Load(static_cast<ResourceManager::SRC>(static_cast<int>(ResourceManager::SRC::EFFECT_BOSS_PROJECTILE))).handleId_,

			// �G�t�F�N�g�̃X�P�[��
			jsonData_["EFFECT"][i - 1]["SCALE"]);

	}

}

void Boss::InitFunctionPointer()
{

	//�֐��|�C���^�̏�����
	stateChange_.emplace(BossState::IDLE, std::bind(&Boss::ChangeIdle, this));
	stateChange_.emplace(BossState::RUN, std::bind(&Boss::ChangeRun, this));
	stateChange_.emplace(BossState::ATTACK_PUNCH, std::bind(&Boss::ChangePunch, this));
	stateChange_.emplace(BossState::ATTACK_KICK, std::bind(&Boss::ChangeKick, this));
	stateChange_.emplace(BossState::ATTACK_PROJECTILE, std::bind(&Boss::ChangeProjectile, this));

}

void Boss::UpdateDebugImGui()
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

void Boss::AnimationFrame()
{

	// �Ώۃt���[���̃��[�J���s��������l�Ƀ��Z�b�g����
	MV1ResetFrameUserLocalMatrix(transform_->modelId, collisionData_.body);

	// ���W���Œ肷��
	if (animationController_->IsBlendPlay("ATTACK_KICK"))
	{

		// �Ώۃt���[���̃��[�J���s��(�傫���A��]�A�ʒu)���擾����
		auto mat = MV1GetFrameLocalMatrix(transform_->modelId, collisionData_.body);

		auto scl = MGetSize(mat); // �s�񂩂�傫�������o��
		auto rot = MGetRotElem(mat); // �s�񂩂��]�����o��
		auto pos = MGetTranslateElem(mat); // �s�񂩂�ړ��l�����o��

		// �傫���A��]�A�ʒu�����[�J���s��ɖ߂�
		MATRIX mix = MGetIdent();
		mix = MMult(mix, MGetScale(scl)); // �傫��
		mix = MMult(mix, rot); // ��]

		// �����Ń��[�J�����W���s��ɁA���̂܂ܖ߂����A
		// �����������[�J�����W��ݒ肷��
		mix = MMult(mix, MGetTranslate({ 0.0f, pos.y, 0.0f }));

		// ���������s���Ώۃt���[���ɃZ�b�g�������āA
		// �A�j���[�V�����̈ړ��l�𖳌���
		MV1SetFrameUserLocalMatrix(transform_->modelId, collisionData_.body, mix);

	}

}

void Boss::Update(const float deltaTime)
{

	// ImGui�̃f�o�b�O�`��̍X�V
	UpdateDebugImGui();

	// �N�[���^�C�����v�Z
	coolTime_ -= deltaTime;

	// �U����ړ����X�V
	aiComponent_->Update(deltaTime);

	// �Փ˔���̍X�V
	ActorBase::CollisionUpdate();

	// ��ѓ���̍X�V
	Projectile(deltaTime);

	// ��Ԃ��Ƃ̍X�V
	// �d�͂�������O�ɏ������Ȃ��Ƃ������ȋ����ɂȂ�̂Œ��ӁI
	stateUpdate_(deltaTime);

	//// �d�͂�������A�j���[�V�����̂ݏ�������
	//if (state_ != BossState::HIT_KNOCK_BACK && transform_->pos.y > FLOOR_HEIGHT)
	//{
		// �d��
		Gravity(gravityScale_);
	//}

	// ���f�������X�V
	transform_->Update();

	// �A�j���[�V�����Đ�
	animationController_->Update(deltaTime);

	// �A�j���[�V�����̃t���[�����Œ�
	AnimationFrame();

}

void Boss::Projectile(const float deltaTime)
{

	// �U�����肪����t���[��
	if (collisionData_.isProjectileAttack && ATTACK_PROJECTILE_COLLISION_TIME >= projectileCollisionCnt_)
	{

		// ��ѓ���̓����蔻��̍��W�ݒ�
		collisionData_.projectilePos = VAdd(collisionData_.projectilePos, VScale(projectileDir_,100.0f));

		// ��ѓ���̏Փ˔��肪�������Ԃ̃J�E���^�����Z
		projectileCollisionCnt_ += deltaTime;

		// �G�t�F�N�g��Ǐ]������
		effekseerController_->FollowPos(collisionData_.projectilePos, Quaternion::Identity(), { 0.0f,500.0f,0.0f }, "PROJECTILE");

	}
	else if (ATTACK_PROJECTILE_COLLISION_TIME <= projectileCollisionCnt_)
	{

		// ��ѓ���̓����蔻����Ȃ���
		collisionData_.isProjectileAttack = false;

		// �K�E�Z�̏Փ˔��肪�������Ԃ̃J�E���^�����Z�b�g
		projectileCollisionCnt_ = 0.0f;

		// �G�t�F�N�g���~�߂�
		effekseerController_->DrawStop("PROJECTILE");

	}

}

void Boss::ChangeState(const BossState state)
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

const bool Boss::GetAttackState() const
{

	// �U���̏�Ԃ�����
	for (const auto state : attackState_)
	{
		if (state_ == state)
		{
			return true;
		}
	}

	return false;

}

const std::vector<int> Boss::GetTotalAttackTypes() const
{

	std::vector<int> intStates;
	intStates.reserve(attackState_.size());

	// �ϊ�����
	for (const auto& state : attackState_)
	{
		intStates.push_back(static_cast<int>(state));
	}

	return intStates;

}

void Boss::AttackHit(const int damage, const int state)
{

	// �ǂ̃q�b�g�A�j���[�V�������`�F�b�N����
	AttackHitCheck(state);

	// HP�����炷
	SubHp(damage);

	// HP��0�ɂȂ����玀�ʃA�j���[�V�����ɑJ��
	if (hp_ <= 0)
	{
		DeathAnim(state);
	}

	// �A�j���[�V�����̍Đ����Ԃ����Z�b�g����
	animationController_->ResetStepAnim();

}

void Boss::ProjectileHit(const int damage)
{

	// �q�b�g�A�j���[�V�����ɑJ��
	ChangeState(BossState::HIT_BODY);

	// HP�����炷
	SubHp(damage);

	// �A�j���[�V�����̍Đ����Ԃ����Z�b�g����
	animationController_->ResetStepAnim();

}

void Boss::ChangeIdle()
{

	stateUpdate_ = std::bind(&Boss::UpdateIdle, this, std::placeholders::_1);

	// �E��̍U��������Ȃ���
	collisionData_.isRightHandAttack = false;

	// ����̍U��������Ȃ���
	collisionData_.isLeftHandAttack = false;

	// �E���̍U��������Ȃ���
	collisionData_.isRightFootAttack = false;

	// �����̍U��������Ȃ���
	collisionData_.isLeftFootAttack = false;

	// �U�����������Ă��邩�����Z�b�g����
	isAttackHit_ = false;

	// �d�͂�ʏ��Ԃɖ߂�
	gravityScale_ = 1.0f;

}

void Boss::ChangeRun()
{

	stateUpdate_ = std::bind(&Boss::UpdateRun, this, std::placeholders::_1);

	// �ǂ������Ă��鎞�Ԃ����Z�b�g
	trackingTime_ = 0.0f;

}

void Boss::ChangePunch()
{

	stateUpdate_ = std::bind(&Boss::UpdatePunch, this, std::placeholders::_1);

	// �U�����������Ă��邩�����Z�b�g����
	isAttackHit_ = false;

	// �X�s�[�h
	speed_ = ATTACK_MOVE_POW;

}

void Boss::ChangeKick()
{

	stateUpdate_ = std::bind(&Boss::UpdateKick, this, std::placeholders::_1);

	// �U�����������Ă��邩�����Z�b�g����
	isAttackHit_ = false;

	// �X�s�[�h
	speed_ = ATTACK_MOVE_POW;

}

void Boss::ChangeProjectile()
{

	stateUpdate_ = std::bind(&Boss::UpdateProjectile, this, std::placeholders::_1);

	// �U�����������Ă��邩�����Z�b�g����
	isAttackHit_ = false;

	// �_���[�W��
	damage_ = ATTACK_PROJECTILE_DAMAGE;

	// �K�E�Z�̓����蔻��̍��W��ݒ�
	collisionData_.projectilePos = VAdd(transform_->pos, BODY_RELATIVE_CENTER_POS);

	// ��ѓ���̔��ł�������
	projectileDir_ = VNorm(transform_->quaRot.GetForward());

	// ��ѓ���̓����蔻������Z�b�g
	collisionData_.isProjectileAttack = false;

}

void Boss::ChangeHitHead()
{

	stateUpdate_ = std::bind(&Boss::UpdateHitHead, this, std::placeholders::_1);

	// �v���C���[�̕��������߂�
	VECTOR vec = VSub(targetPos_, transform_->pos);

	// �v���C���[�̕����Ƌt�����̃x�N�g��
	moveDir_ = { -vec.x, vec.y,-vec.z };

	// �X�s�[�h
	speed_ = ATTACK_MOVE_POW;

}

void Boss::ChangeHitBody()
{

	stateUpdate_ = std::bind(&Boss::UpdateHitBody, this, std::placeholders::_1);

	// �v���C���[�̕��������߂�
	VECTOR vec = VSub(targetPos_, transform_->pos);

	// �v���C���[�̕����Ƌt�����̃x�N�g��
	moveDir_ = { -vec.x, vec.y,-vec.z };

	// �X�s�[�h
	speed_ = ATTACK_MOVE_POW;

}

void Boss::ChangeHitFly()
{

	stateUpdate_ = std::bind(&Boss::UpdateHitFly, this, std::placeholders::_1);

	// �v���C���[�̕��������߂�
	VECTOR vec = VSub(targetPos_, transform_->pos);

	// ���K��
	vec = VNorm(vec);

	// �v���C���[�̕����Ƌt�����̃x�N�g��
	moveDir_ = { -vec.x, vec.y,-vec.z };

	// ��O�̃A�j���[�V�������܂��������ł����̂������������ɔ�΂��Ȃ�
	if (key_ != ANIM_DATA_KEY[static_cast<int>(BossState::HIT_KNOCK_BACK)])
	{

		// ������ɔ�΂�
		velocity_.y = HIT_FLY_UP_VEC_POW;
		moveDir_.y = velocity_.y;

		// �X�s�[�h
		speed_ = HIT_FLY_MOVE_POW;

	}

	// �̂̊p�x�����Z�b�g
	transform_->quaRot.x = Quaternion().x;
	transform_->quaRot.z = Quaternion().z;

	// �d�͂�ʏ��Ԃɖ߂�
	gravityScale_ = 1.0f;

}

void Boss::ChangeHitFlinchUp()
{

	stateUpdate_ = std::bind(&Boss::UpdateHitFlinchUp, this, std::placeholders::_1);

	// �v���C���[�̕��������߂�
	VECTOR vec = VSub(targetPos_, transform_->pos);

	// ���K��
	vec = VNorm(vec);

	// �v���C���[�̕����Ƌt�����̃x�N�g��
	vec = { -vec.x, vec.y,-vec.z };

	// �n�ʂɂ��Ă������Ɉړ�������
	if (velocity_.y == 0.0f)
	{
		velocity_.y = FLINCH_UP_UP_VEC_POW;
	}
	// �n�ʂɂ��Ă��Ȃ������班���ړ�������
	else
	{
		velocity_.y = FLINCH_UP_UP_VEC_SMALL_POW;
	}

	// ������ɔ�΂�
	vec.y = velocity_.y;

	// ���ۂɓ�������
	moveDir_ = vec;

	// �X�s�[�h
	speed_ = FLINCH_UP_SPEED;

	// ���łɊp�x���ς���Ă����珈�����Ȃ�
	if (!isChangeAngle_)
	{
		// �̂̊p�x��ύX
		transform_->quaRot = Quaternion::Mult(transform_->quaRot, Quaternion::AngleAxis(Utility::Deg2RadF(FLINCH_UP_ANGLE_X), Utility::AXIS_X));
		isChangeAngle_ = true;
	}

	// �d�͂��ɂ�����
	gravityScale_ = FLINCH_UP_GRAVITY_SCALE;

}

void Boss::ChangeHitKnockback()
{

	stateUpdate_ = std::bind(&Boss::UpdateHitKnockback, this, std::placeholders::_1);

	// �v���C���[�̕��������߂�
	VECTOR vec = VSub(targetPos_, transform_->pos);

	// �v���C���[�̕����Ƌt�����̃x�N�g��
	moveDir_ = { -vec.x, vec.y,-vec.z };

	// y����������
	moveDir_.y = 0.0f;

	// �X�s�[�h
	speed_ = HIT_FLY_MOVE_POW;

	// �����𒲐�����
	transform_->pos.y = transform_->pos.y + KNOCK_BACK_HEIGHT_OFFSET;

}

void Boss::ChangeHitKipUp()
{

	stateUpdate_ = std::bind(&Boss::UpdateHitKipUp, this, std::placeholders::_1);

	// �̂̊p�x�����Z�b�g
	transform_->quaRot.x = Quaternion().x;
	transform_->quaRot.z = Quaternion().z;

}

void Boss::ChangeDeath()
{

	stateUpdate_ = std::bind(&Boss::UpdateDeath, this, std::placeholders::_1);

	// �̂̊p�x�����Z�b�g
	transform_->quaRot.x = Quaternion().x;
	transform_->quaRot.z = Quaternion().z;

}

void Boss::UpdateIdle(const float deltaTime)
{

	// �v���C���[�̍��W
	std::optional<VECTOR> playerPos = GetPlayerPos();
	targetPos_ = playerPos.value();

	// �v���C���[�̕��������߂�
	VECTOR vec = VSub(targetPos_, transform_->pos);

	// ���K��
	vec = VNorm(vec);

	// �������p�x�ɕϊ�����
	float angle = atan2f(vec.x, vec.z);

	// �v���C���[�����ɉ�]
	LazyRotation(angle);

	// ���̍s�����l����
	isActionDecided_ = false;

}

void Boss::UpdateRun(const float deltaTime)
{

	// �v���C���[�̍��W
	std::optional<VECTOR> playerPos = GetPlayerPos();
	targetPos_ = playerPos.value();

	// �v���C���[�̕��������߂�
	moveDir_ = VSub(targetPos_, transform_->pos);

	// �x�N�g���̒���
	float length = Utility::Magnitude(moveDir_);

	// �X�s�[�h
	speed_ = RUN_MOVE_POW;

	// �ړ�����
	moveComponent_->Move();

	// �������p�x�ɕϊ�����
	float angle = atan2f(moveDir_.x, moveDir_.z);

	// �v���C���[�����ɉ�]
	LazyRotation(angle);

	// �v���C���[�̋߂��ɗ����玟�̍s�������߂�
	if (length <= ACTIVATION_DISTANCE)
	{
		isActionDecided_ = false;
	}
	// �v���C���[�ɒǂ����Ȃ�������ҋ@�ɑJ��
	else if (trackingTime_ >= TRACKING_MAX_TIME)
	{

		ChangeState(BossState::IDLE);

		// �N�[���^�C����ݒ�
		coolTime_ = COOL_TIME;

	}

	trackingTime_ += deltaTime;

}

void Boss::UpdatePunch(const float deltaTime)
{

	// �U�����肪����t���[��
	if (PUNCH_ATTACK_START_FRAME <= animationController_->GetStepAnim() && animationController_->GetStepAnim() <= PUNCH_ATTACK_END_FRAME)
	{
		collisionData_.isRightHandAttack = true;
	}
	else
	{
		collisionData_.isRightHandAttack = false;
	}

	// �A�j���[�V�������I��������ҋ@��Ԃ֑J�ڂ���
	if (animationController_->IsEndPlayAnimation())
	{

		ChangeState(BossState::IDLE);

		// �N�[���^�C����ݒ�
		coolTime_ = COOL_TIME;

	}

}

void Boss::UpdateKick(const float deltaTime)
{

	// �U�����肪����t���[��
	if (KICK_ATTACK_START_FRAME <= animationController_->GetStepAnim() && animationController_->GetStepAnim() <= KICK_ATTACK_END_FRAME)
	{
		collisionData_.isRightFootAttack = true;
	}
	else
	{
		collisionData_.isRightFootAttack = false;
	}

	// �A�j���[�V�������I��������ҋ@��Ԃ֑J�ڂ���
	if (animationController_->IsEndPlayAnimation())
	{

		ChangeState(BossState::IDLE);

		// �N�[���^�C����ݒ�
		coolTime_ = COOL_TIME;

	}

}

void Boss::UpdateProjectile(const float deltaTime)
{

	bool a = effekseerController_->IsDraw("PROJECTILE");
	if (!a && ATTACK_PROJECTILE_START_FRAME <= animationController_->GetStepAnim())
	{

		collisionData_.isProjectileAttack = true;

		// �G�t�F�N�g��`��
		effekseerController_->Draw(collisionData_.projectilePos, Quaternion::Identity(), { 0.0f,500.0f,0.0f }, "PROJECTILE");

	}

	// �ҋ@��ԂɑJ��
	if (animationController_->IsEndPlayAnimation())
	{

		ChangeState(BossState::IDLE);

		// �N�[���^�C����ݒ�
		coolTime_ = COOL_TIME;

	}

}

void Boss::UpdateHitHead(const float deltaTime)
{

	// �������ɂ������ړ�
	moveComponent_->Lerp();

	// �A�j���[�V�������I��������ҋ@��Ԃ֑J�ڂ���
	if (animationController_->IsEndPlayAnimation())
	{
		ChangeState(BossState::IDLE);
	}

}

void Boss::UpdateHitBody(const float deltaTime)
{

	// �������ɂ������ړ�
	moveComponent_->Lerp();

	// �A�j���[�V�������I��������ҋ@��Ԃ֑J�ڂ���
	if (animationController_->IsEndPlayAnimation())
	{
		ChangeState(BossState::IDLE);
	}

}

void Boss::UpdateHitFly(const float deltaTime)
{

	// �n�ʂɂ��܂ŉ��Z����
	if (velocity_.y != 0.0f)
	{
		// ���ɔ��ł����Ȃ���ړ�
		moveComponent_->HitMove();
	}

	// HP��0�ȉ����������A�N�e�B�u�ɂ���
	if (animationController_->IsEndPlayAnimation() && hp_ <= 0)
	{
		isActive_ = false;
	}

	// HP��0�ȉ��͒ʂ�Ȃ�
	if (hp_ <= 0)return;

	// �A�j���[�V�������I��������N���オ���Ԃ֑J�ڂ���
	if (animationController_->IsEndPlayAnimation())
	{
		ChangeState(BossState::KIP_UP);
	}

}

void Boss::UpdateHitFlinchUp(const float deltaTime)
{

	// �n�ʂɂ��܂ŉ��Z����
	if (velocity_.y != 0.0f)
	{
		// ��Ɋɂ��ړ�����
		moveComponent_->HitMove();
	}

	// �A�j���[�V�������I��������N���オ���Ԃ֑J�ڂ���
	if (velocity_.y == 0.0f)
	{
		ChangeState(BossState::KIP_UP);
	}

}

void Boss::UpdateHitKnockback(const float deltaTime)
{
	
	// ���ł����鎞�Ԃ܂ňړ���������
	if (KNOCK_BACK_TIME > knockBackCnt_)
	{
		// ���ɔ��ł����Ȃ���ړ�
		moveComponent_->Move();
	}
	else
	{
		knockBackCnt_ = 0.0f;
		ChangeState(BossState::HIT_FLY);
	}

	// ���ł��鎞�Ԃ��J�E���g
	knockBackCnt_ += deltaTime;

}

void Boss::UpdateHitKipUp(const float deltaTime)
{

	// �A�j���[�V�������I��������ҋ@��Ԃ֑J�ڂ���
	if (animationController_->IsEndPlayAnimation())
	{
		ChangeState(BossState::IDLE);
	}

}

void Boss::UpdateDeath(const float deltaTime)
{

	// �A�j���[�V�������I���������A�N�e�B�u�ɂ���
	if (animationController_->IsEndPlayAnimation())
	{
		isActive_ = false;
	}

}