#include <random>
#include "../Manager/SceneManager.h"
#include "../Manager/CollisionManager.h"
#include "../Manager/ActorManager.h"
#include "../Scene/GameScene.h"
#include "Enemy.h"

Enemy::Enemy(const VECTOR& pos, const json& data)
	:
	ActorBase(pos, data),
	PUNCH_ATTACK_START_FRAME(data["ANIM"][static_cast<int>(EnemyState::PUNCH) - 1]["ATTACK_START_FRAME"]),
	PUNCH_ATTACK_END_FRAME(data["ANIM"][static_cast<int>(EnemyState::PUNCH) - 1]["ATTACK_END_FRAME"]),
	KICK_ATTACK_START_FRAME(data["ANIM"][static_cast<int>(EnemyState::KICK) - 1]["ATTACK_START_FRAME"]),
	KICK_ATTACK_END_FRAME(data["ANIM"][static_cast<int>(EnemyState::KICK) - 1]["ATTACK_END_FRAME"]),
	COOL_TIME(data["COOL_TIME"]),
	ACTIVATION_DISTANCE(data["ACTIVATION_DISTANCE"]),
	HIT_FLY_MOVE_POW(data["HIT_FLY_MOVE_POW"]),
	TRACKING_MAX_TIME(data["TRACKING_MAX_TIME"])
{

	// �@�\�̏�����
	InitFunction();

	// ���f��ID
	modelId_ = resMng_.LoadModelDuplicate(ResourceManager::SRC::ENEMY);

	// �֐��|�C���^�̏�����
	InitFunctionPointer();

	// �p�����[�^�̏�����
	InitParameter();

	// �A�j���[�V�����̏�����
	InitAnimation();

}

void Enemy::Init(const VECTOR& pos)
{

	// �@�\�̏�����
	InitFunction();

	// ���f��ID
	modelId_ = resMng_.LoadModelDuplicate(ResourceManager::SRC::ENEMY);

	// ���ʕ����͊��N���X�ŏ�����
	ActorBase::Init(pos);

	// �֐��|�C���^�̏�����
	InitFunctionPointer();

	// �p�����[�^�̏�����
	InitParameter();

	// �A�j���[�V�����̏�����
	InitAnimation();

}

void Enemy::InitFunction()
{
}

void Enemy::InitParameter()
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
	collisionData_.rightHand = MV1SearchFrame(transform_.modelId, RIGHT_HAND_FRAME.c_str());

	// ����̃t���[���ԍ����擾
	collisionData_.leftHand = MV1SearchFrame(transform_.modelId, LEFT_HAND_FRAME.c_str());

	// �E���̃t���[���ԍ����擾
	collisionData_.rightFoot = MV1SearchFrame(transform_.modelId, RIGHT_FOOT_FRAME.c_str());

	// �����̃t���[���ԍ����擾
	collisionData_.leftFoot = MV1SearchFrame(transform_.modelId, LEFT_FOOT_FRAME.c_str());

	// �̂̃t���[���ԍ����擾
	collisionData_.body = MV1SearchFrame(transform_.modelId, BODY_FRAME.c_str());

	// �葫�̏Փ˔���̔��a
	collisionData_.handAndFootCollisionRadius = HAND_AND_FOOT_COLLISION_RADIUS;

	// �̂̏Փ˔���̔��a
	collisionData_.bodyCollisionRadius = BODY_COLLISION_RADIUS;

	// �E��ɍU�����肪���邩�ǂ���
	collisionData_.isRightHandAttack = false;

	// ����ɍU�����肪���邩�ǂ���
	collisionData_.isLeftHandAttack = false;

	// �E���ɍU�����肪���邩�ǂ���
	collisionData_.isRightFootAttack = false;

	// �����ɍU�����肪���邩�ǂ���
	collisionData_.isLeftFootAttack = false;

	// �v���C���[�̍��W
	std::optional<VECTOR> playerPos = GetPlayerPos();

	// ����̍��W
	targetPos_ = playerPos.value();

	// ����Ƃ��̈ړ���
	RUN_MOVE_POW = jsonData_["RUN_MOVE_POW"];

	// �U�����󂯂��Ƃ��ɐi�ޗ�
	HIT_MOVE_POW = jsonData_["HIT_MOVE_POW"];

	// �A�j���[�V�����ԍ�
	ANIM_INDEX = jsonData_["ANIM_INDEX"];

	// �N�[���^�C��
	coolTime_ = 0.0f;

	// �ǂ������Ă��鎞��
	trackingTime_ = 0.0f;

	// �s�������߂����ǂ���
	isActionDecided_ = false;

}

void Enemy::InitFunctionPointer()
{
	//�֐��|�C���^�̏�����
	stateChange_.emplace(EnemyState::IDLE, std::bind(&Enemy::ChangeIdle, this));
	stateChange_.emplace(EnemyState::RUN, std::bind(&Enemy::ChangeRun, this));
	stateChange_.emplace(EnemyState::PUNCH, std::bind(&Enemy::ChangePunch, this));
	stateChange_.emplace(EnemyState::KICK, std::bind(&Enemy::ChangeKick, this));
	stateChange_.emplace(EnemyState::HIT_HEAD, std::bind(&Enemy::ChangeHitHead, this));
	stateChange_.emplace(EnemyState::HIT_BODY, std::bind(&Enemy::ChangeHitBody, this));
	stateChange_.emplace(EnemyState::HIT_FLY, std::bind(&Enemy::ChangeHitFly, this));
	stateChange_.emplace(EnemyState::KIP_UP, std::bind(&Enemy::ChangeKipUp, this));
}

void Enemy::InitAnimation()
{

	// �A�j���[�V�����R���g���[���̐���
	animationController_ = std::make_unique<AnimationController>(transform_.modelId);

	// �A�j���[�V�����R���g���[���[�ɃA�j���[�V������ǉ�
	for (int i = static_cast<int>(EnemyState::IDLE); i < static_cast<int>(EnemyState::MAX); ++i)
	{

		// ���[�v�Đ�����A�j���[�V��������isLoop��true�ɂ���
		bool isLoop = i == static_cast<int>(EnemyState::IDLE) || i == static_cast<int>(EnemyState::RUN);
		animationController_->Add(

			// �A�j���[�V�����̖��O
			jsonData_["ANIM"][i - 1]["NAME"],

			// �A�j���[�V�����̃p�X
			jsonData_["ANIM"][i - 1]["PATH"],

			// �A�j���[�V�������n�܂鎞��
			0.0f,

			// �A�j���[�V�����X�s�[�h
			jsonData_["ANIM"][i - 1]["SPEED"],

			// �A�j���[�V�����n���h��
			resMng_.LoadModelDuplicate(static_cast<ResourceManager::SRC>(static_cast<int>(ResourceManager::SRC::ENEMY) + i)),

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
	ChangeState(EnemyState::IDLE);

}

void Enemy::Update(const float deltaTime)
{

	// �N�[���^�C�����v�Z
	coolTime_ -= deltaTime;

	// �ǂ̍s�������邩���߂�
	if (!isActionDecided_ && coolTime_ <= 0.0f)
	{
		SelsectAction();
	}

	// ��Ԃ��Ƃ̍X�V
	stateUpdate_(deltaTime);

	// �d��
	Gravity();

	// �A�j���[�V�����Đ�
	animationController_->Update(deltaTime);

	// ���f�������X�V
	transform_.Update();

	// �Փ˔���̍X�V
	ActorBase::CollisionUpdate();

	// �A�j���[�V�����̃t���[�����Œ�
	AnimationFrame();

}

void Enemy::Draw()
{

	ActorBase::Draw();

	// HP�o�[
	int hpLength = 200;
	int hpGauge;

	hpGauge = hpLength * hp_ / HP_MAX;

	// HP��`��
	DrawBox(0, 100, 0 + hpGauge, 120, 0xff0000, true);

	DrawFormatString(0, 15, 0xff0000, "velocity:(%0.2f,%0.2f,%0.2f)", velocity_.x, velocity_.y, velocity_.z);

}

bool Enemy::GetAttackState()
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

bool Enemy::GetHitState()
{

	// �U�����󂯂Ă����Ԃ�����
	for (const auto state : hitState_)
	{
		if (state_ == state)
		{
			return true;
		}
	}

	return false;

}

void Enemy::AttackHit(const int damage, const int state)
{

	// ���Ƀq�b�g����A�j���[�V�������`�F�b�N
	for (const auto hitState : hitHeadState_)
	{
		if (hitState == static_cast<PlayerState>(state))
		{
			ChangeState(EnemyState::HIT_HEAD);
		}
	}

	// �̂Ƀq�b�g����A�j���[�V�������`�F�b�N
	for (const auto hitState : hitBodyState_)
	{
		if (hitState == static_cast<PlayerState>(state))
		{
			ChangeState(EnemyState::HIT_BODY);
		}
	}

	// �������ł����A�j���[�V�������`�F�b�N
	for (const auto hitState : hitFlyState_)
	{
		if (hitState == static_cast<PlayerState>(state))
		{
			ChangeState(EnemyState::HIT_FLY);
		}
	}

	// HP�����炷
	SubHp(damage);
	
	// �A�j���[�V�����̍Đ����Ԃ����Z�b�g����
	animationController_->ResetStepAnim();

}

void Enemy::AnimationFrame()
{

	// �Ώۃt���[���̃��[�J���s��������l�Ƀ��Z�b�g����
	MV1ResetFrameUserLocalMatrix(transform_.modelId, collisionData_.body);

	// ���W���Œ肷��
	if (animationController_->IsBlendPlay("HIT_FLY"))
	{

		// �Ώۃt���[���̃��[�J���s��(�傫���A��]�A�ʒu)���擾����
		auto mat = MV1GetFrameLocalMatrix(transform_.modelId, collisionData_.body);

		auto scl = MGetSize(mat); // �s�񂩂�傫�������o��
		auto rot = MGetRotElem(mat); // �s�񂩂��]�����o��
		auto pos = MGetTranslateElem(mat); // �s�񂩂�ړ��l�����o��

		// �傫���A��]�A�ʒu�����[�J���s��ɖ߂�
		MATRIX mix = MGetIdent();
		mix = MMult(mix, MGetScale(scl)); // �傫��
		mix = MMult(mix, rot); // ��]

		// �����Ń��[�J�����W���s��ɁA���̂܂ܖ߂����A
		// �����������[�J�����W��ݒ肷��
		mix = MMult(mix, MGetTranslate({ 0.0f, 0.0f, 0.0f }));

		// ���������s���Ώۃt���[���ɃZ�b�g�������āA
		// �A�j���[�V�����̈ړ��l�𖳌���
		MV1SetFrameUserLocalMatrix(transform_.modelId, collisionData_.body, mix);

	}

}

std::optional<VECTOR> Enemy::GetPlayerPos()
{

	// �Q�[���V�[���̏��������Ă���
	std::shared_ptr<GameScene> gameScene =
		std::dynamic_pointer_cast<GameScene>(SceneManager::GetInstance().GetNowScene());

	// NULL�`�F�b�N
	if (!gameScene)
	{
		return std::nullopt;
	}

	// �A�N�^�[�}�l�[�W���[���擾
	auto actorManager = gameScene->GetActorManager();

	// �Ǐ]�Ώ�
	auto players = actorManager->GetActiveActorData().find(ActorType::PLAYER);

	// �v���C���[�̍��W���擾
	for (const auto& player : players->second)
	{
		return player->GetPos();
	}

}

void Enemy::ChangeState(EnemyState state)
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

void Enemy::SelsectAction()
{

	// �q�b�g���͍s���ł��Ȃ�
	for (const auto hitState : hitState_)
	{
		if (hitState == state_)
		{
			return;
		}
	}

	// ����
	
	// �񌈒�I�ȗ���������
	std::random_device rd;

	// �����Z���k�E�c�C�X�^�@�ɂ�闐��������
	std::mt19937 gen(rd());

	// �w��͈̔͂Ń����_���Ȑ����擾
	std::uniform_int_distribution<> dist_int(0,1); 
	int number = dist_int(gen);

	if (number == 0)
	{
		// �ړ�����
		Move();
	}
	else if (number == 1)
	{
		//�U������
		Attack();
	}

}

void Enemy::Move()
{

	// �v���C���[�̍��W
	std::optional<VECTOR> playerPos = GetPlayerPos();
	targetPos_ = playerPos.value();

	// �G����v���C���[�ւ̃x�N�g��
	VECTOR vec = VSub(targetPos_, transform_.pos);

	// �x�N�g���̒���
	float length = Utility::Magnitude(vec);

	// �v���C���[�̋߂��Ɉړ�
	if (length >= ACTIVATION_DISTANCE)
	{

		ChangeState(EnemyState::RUN);

		// �s�������߂�
		isActionDecided_ = true;

	}

}

void Enemy::Attack()
{

	// ����

	// �񌈒�I�ȗ���������
	std::random_device rd;

	// �����Z���k�E�c�C�X�^�@�ɂ�闐��������
	std::mt19937 gen(rd());

	// �w��͈̔͂Ń����_���Ȑ����擾
	std::uniform_int_distribution<> dist_int(0, 1);
	int number = dist_int(gen);
	//int number = 1;

	// �v���C���[�̍��W
	std::optional<VECTOR> playerPos = GetPlayerPos();

	// ����̍��W
	targetPos_ = playerPos.value();

	// �G����v���C���[�ւ̃x�N�g��
	VECTOR vec = VSub(targetPos_, transform_.pos);

	// �x�N�g���̒���
	float length = Utility::Magnitude(vec);

	// �v���C���[�Ƃ̋���������������U���ł��Ȃ�
	if (length >= ACTIVATION_DISTANCE) return;

	if (number == 0)
	{

		ChangeState(EnemyState::PUNCH);

		// �s�������߂�
		isActionDecided_ = true;

	}
	else if (number == 1)
	{

		ChangeState(EnemyState::KICK);

		// �s�������߂�
		isActionDecided_ = true;

	}

}

void Enemy::ChangeIdle()
{

	stateUpdate_ = std::bind(&Enemy::UpdateIdle, this, std::placeholders::_1);

	// �E��̍U��������Ȃ���
	collisionData_.isRightHandAttack = false;

	// ����̍U��������Ȃ���
	collisionData_.isLeftHandAttack = false;

	// �E���̍U��������Ȃ���
	collisionData_.isRightFootAttack = false;

	// �����̍U��������Ȃ���
	collisionData_.isLeftFootAttack = false;

}

void Enemy::ChangeRun()
{

	stateUpdate_ = std::bind(&Enemy::UpdateRun, this, std::placeholders::_1);

	// �ǂ������Ă��鎞�Ԃ����Z�b�g
	trackingTime_ = 0.0f;

}

void Enemy::ChangePunch()
{

	stateUpdate_ = std::bind(&Enemy::UpdatePunch, this, std::placeholders::_1);

	// �U�����������Ă��邩�����Z�b�g����
	isAttackHit_ = false;

}

void Enemy::ChangeKick()
{

	stateUpdate_ = std::bind(&Enemy::UpdateKick, this, std::placeholders::_1);

	// �U�����������Ă��邩�����Z�b�g����
	isAttackHit_ = false;

}

void Enemy::ChangeHitHead()
{

	stateUpdate_ = std::bind(&Enemy::UpdateHitHead, this, std::placeholders::_1);

	// �v���C���[�̕��������߂�
	VECTOR vec = VSub(targetPos_, transform_.pos);

	// ���K��
	vec = VNorm(vec);

	// �v���C���[�̕����Ƌt�����̃x�N�g��
	vec = { -vec.x, vec.y,-vec.z };

	// �ړ���
	movePow_ = VAdd(transform_.pos, VScale(vec, ATTACK_MOVE_POW));

}

void Enemy::ChangeHitBody()
{

	stateUpdate_ = std::bind(&Enemy::UpdateHitBody, this, std::placeholders::_1);

	// �v���C���[�̕��������߂�
	VECTOR vec = VSub(targetPos_, transform_.pos);

	// ���K��
	vec = VNorm(vec);

	// �v���C���[�̕����Ƌt�����̃x�N�g��
	vec = { -vec.x, vec.y,-vec.z };

	// �ړ���
	movePow_ = VAdd(transform_.pos, VScale(vec, ATTACK_MOVE_POW));

}

void Enemy::ChangeHitFly()
{
	stateUpdate_ = std::bind(&Enemy::UpdateHitFly, this, std::placeholders::_1);

	// �v���C���[�̕��������߂�
	VECTOR vec = VSub(targetPos_, transform_.pos);
	
	// ���K��
	vec = VNorm(vec);

	// �v���C���[�̕����Ƌt�����̃x�N�g��
	vec = { -vec.x, vec.y,-vec.z };

	// ������ɔ�΂�
	velocity_.y = 1.0f;
	vec.y = velocity_.y;

	// �ړ���
	movePow_ = VScale(vec,HIT_FLY_MOVE_POW);

}

void Enemy::ChangeKipUp()
{
	stateUpdate_ = std::bind(&Enemy::UpdateKipUp, this, std::placeholders::_1);
}

void Enemy::UpdateIdle(const float deltaTime)
{

	// �v���C���[�̍��W
	std::optional<VECTOR> playerPos = GetPlayerPos();
	targetPos_ = playerPos.value();

	// �v���C���[�̕��������߂�
	VECTOR vec = VSub(targetPos_, transform_.pos);

	// ���K��
	vec = VNorm(vec);

	// �������p�x�ɕϊ�����
	float angle = atan2f(vec.x, vec.z);

	// �v���C���[�����ɉ�]
	LazyRotation(angle);

	// ���̍s�����l����
	isActionDecided_ = false;

}

void Enemy::UpdateRun(const float deltaTime)
{

	// �v���C���[�̍��W
	std::optional<VECTOR> playerPos = GetPlayerPos();
	targetPos_ = playerPos.value();

	// �v���C���[�̕��������߂�
	VECTOR vec = VSub(targetPos_, transform_.pos);

	// �x�N�g���̒���
	float length = Utility::Magnitude(vec);

	// ���K��
	vec = VNorm(vec);

	// �������p�x�ɕϊ�����
	float angle = atan2f(vec.x, vec.z);

	// �v���C���[�����ɉ�]
	LazyRotation(angle);

	// �ړ���
	movePow_ = VScale(vec, RUN_MOVE_POW);

	// �v���C���[�����Ɉړ�
 	transform_.pos = VAdd(transform_.pos, movePow_);

	// �v���C���[�̋߂��ɗ����玟�̍s�������߂�
	if (length <= ACTIVATION_DISTANCE)
	{
		isActionDecided_ = false;
	}
	// �v���C���[�ɒǂ����Ȃ�������ҋ@�ɑJ��
	else if (trackingTime_ >= TRACKING_MAX_TIME)
	{

		ChangeState(EnemyState::IDLE);

		// �N�[���^�C����ݒ�
		coolTime_ = COOL_TIME;

	}

	trackingTime_ += deltaTime;
}

void Enemy::UpdatePunch(const float deltaTime)
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

		ChangeState(EnemyState::IDLE);

		// �N�[���^�C����ݒ�
		coolTime_ = COOL_TIME;

	}

}

void Enemy::UpdateKick(const float deltaTime)
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

		ChangeState(EnemyState::IDLE);

		// �N�[���^�C����ݒ�
		coolTime_ = COOL_TIME;

	}

}

void Enemy::UpdateHitHead(const float deltaTime)
{

	// �������ɂ������ړ�
	transform_.pos = Utility::Lerp(transform_.pos, movePow_, 0.1f);

	// �A�j���[�V�������I��������ҋ@��Ԃ֑J�ڂ���
	if (animationController_->IsEndPlayAnimation())
	{
		ChangeState(EnemyState::IDLE);
	}

}

void Enemy::UpdateHitBody(const float deltaTime)
{

	// �������ɂ������ړ�
	transform_.pos = Utility::Lerp(transform_.pos, movePow_, 0.1f);

	// �A�j���[�V�������I��������ҋ@��Ԃ֑J�ڂ���
	if (animationController_->IsEndPlayAnimation())
	{
		ChangeState(EnemyState::IDLE);
	}

}

void Enemy::UpdateHitFly(const float deltaTime)
{

	// �n�ʂɂ��܂ŉ��Z����
	if (velocity_.y != 0.0f)
	{
		// ���ɔ��ł����Ȃ���ړ�
		transform_.pos = VAdd(transform_.pos, movePow_);
	}

	// �A�j���[�V�������I��������N���オ���Ԃ֑J�ڂ���
	if (animationController_->IsEndPlayAnimation())
	{
		ChangeState(EnemyState::KIP_UP);
	}

}

void Enemy::UpdateKipUp(const float deltaTime)
{

	// �A�j���[�V�������I��������ҋ@��Ԃ֑J�ڂ���
	if (animationController_->IsEndPlayAnimation())
	{
		ChangeState(EnemyState::IDLE);
	}

}
