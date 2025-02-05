#include <memory>
#include "../Lib/ImGui/imgui.h"
#include "../Utility/Utility.h"
#include "../Scene/GameScene.h"
#include "../Manager/SceneManager.h"
#include "../Manager/CollisionManager.h"
#include "../Manager/Camera.h"
#include "../Component/InputComponent.h"
#include "../Component/MoveComponent.h"
#include "../Object/Common/InputController.h"
#include "../Component/InputComponent.h"
#include "../Manager/ActorManager.h"
#include "Player.h"

Player::Player(const VECTOR& pos, const json& data)
	:
	ActorBase(pos, data),
	ATTACK_JAB_START_FRAME(data["ANIM"][static_cast<int>(PlayerState::ATTACK_JAB) - 1]["ATTACK_START_FRAME"]),
	ATTACK_JAB_END_FRAME(data["ANIM"][static_cast<int>(PlayerState::ATTACK_JAB) - 1]["ATTACK_END_FRAME"]),
	ATTACK_JAB_DAMAGE(data["ANIM"][static_cast<int>(PlayerState::ATTACK_JAB) - 1]["DAMAGE"]),
	ATTACK_STRAIGHT_START_FRAME(data["ANIM"][static_cast<int>(PlayerState::ATTACK_STRAIGHT) - 1]["ATTACK_START_FRAME"]),
	ATTACK_STRAIGHT_END_FRAME(data["ANIM"][static_cast<int>(PlayerState::ATTACK_STRAIGHT) - 1]["ATTACK_END_FRAME"]),
	ATTACK_STRAIGHT_DAMAGE(data["ANIM"][static_cast<int>(PlayerState::ATTACK_STRAIGHT) - 1]["DAMAGE"]),
	ATTACK_HOOK_START_FRAME(data["ANIM"][static_cast<int>(PlayerState::ATTACK_HOOK) - 1]["ATTACK_START_FRAME"]),
	ATTACK_HOOK_END_FRAME(data["ANIM"][static_cast<int>(PlayerState::ATTACK_HOOK) - 1]["ATTACK_END_FRAME"]),
	ATTACK_HOOK_DAMAGE(data["ANIM"][static_cast<int>(PlayerState::ATTACK_HOOK) - 1]["DAMAGE"]),
	ATTACK_LEFT_KICK_START_FRAME(data["ANIM"][static_cast<int>(PlayerState::ATTACK_LEFT_KICK) - 1]["ATTACK_START_FRAME"]),
	ATTACK_LEFT_KICK_END_FRAME(data["ANIM"][static_cast<int>(PlayerState::ATTACK_LEFT_KICK) - 1]["ATTACK_END_FRAME"]),
	ATTACK_LEFT_KICK_DAMAGE(data["ANIM"][static_cast<int>(PlayerState::ATTACK_LEFT_KICK) - 1]["DAMAGE"]),
	ATTACK_RIGHT_KICK_START_FRAME(data["ANIM"][static_cast<int>(PlayerState::ATTACK_RIGHT_KICK) - 1]["ATTACK_START_FRAME"]),
	ATTACK_RIGHT_KICK_END_FRAME(data["ANIM"][static_cast<int>(PlayerState::ATTACK_RIGHT_KICK) - 1]["ATTACK_END_FRAME"]),
	ATTACK_RIGHT_KICK_DAMAGE(data["ANIM"][static_cast<int>(PlayerState::ATTACK_RIGHT_KICK) - 1]["DAMAGE"]),
	ATTACK_UPPER_START_FRAME(data["ANIM"][static_cast<int>(PlayerState::ATTACK_UPPER) - 1]["ATTACK_START_FRAME"]),
	ATTACK_UPPER_END_FRAME(data["ANIM"][static_cast<int>(PlayerState::ATTACK_UPPER) - 1]["ATTACK_END_FRAME"]),
	ATTACK_UPPER_DAMAGE(data["ANIM"][static_cast<int>(PlayerState::ATTACK_UPPER) - 1]["DAMAGE"]),
	ATTACK_CHARGE_PUNCH_START_FRAME(data["ANIM"][static_cast<int>(PlayerState::ATTACK_CHARGE_PUNCH) - 1]["ATTACK_START_FRAME"]),
	ATTACK_CHARGE_PUNCH_END_FRAME(data["ANIM"][static_cast<int>(PlayerState::ATTACK_CHARGE_PUNCH) - 1]["ATTACK_END_FRAME"]),
	ATTACK_CHARGE_PUNCH_DAMAGE(data["ANIM"][static_cast<int>(PlayerState::ATTACK_CHARGE_PUNCH) - 1]["DAMAGE"]),
	ATTACK_CHARGE_PUNCH_TIME(data["ATTACK_CHARGE_PUNCH_TIME"]),
	ATTACK_SPECIAL_PUNCH_START_FRAME(data["ANIM"][static_cast<int>(PlayerState::ATTACK_SPECIAL_PUNCH) - 1]["ATTACK_START_FRAME"]),
	ATTACK_SPECIAL_PUNCH_DAMAGE(data["ANIM"][static_cast<int>(PlayerState::ATTACK_SPECIAL_PUNCH) - 1]["DAMAGE"]),
	ATTACK_SPECIAL_PUNCH_COLLISION_TIME(data["ATTACK_SPECIAL_PUNCH_COLLISION_TIME"]),
	EVASION_MOVE_POW(data["EVASION_MOVE_POW"])
{

	// �@�\�̏�����
	InitFunction();

	// ���f��ID
	modelId_ = resMng_.LoadModelDuplicate(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::MODEL_PLAYER)]);

	// �֐��|�C���^�̏�����
	InitFunctionPointer();

	// ���ʕ����͊��N���X�ŏ�����
	ActorBase::Init(pos);

	// �p�����[�^�̏�����
	InitParameter();

	// �摜�̏�����
	InitImage();

	// �A�j���[�V�����̏�����
	InitAnimation();

	// BGM��SE�̏�����
	InitBGMAndSE();

}

void Player::Init(const VECTOR& pos)
{

	// ���͗p�̃R���|�[�l���g��ǉ�
	inputComponent_ = std::make_unique<InputComponent>(std::static_pointer_cast<Player>(GetThis()));

	// �ړ��p�̃R���|�[�l���g��ǉ�
	moveComponent_ = std::make_unique<MoveComponent>(std::static_pointer_cast<Player>(GetThis()));

	// �`��p�̃R���|�[�l���g
	drawComponent_ = std::make_unique<DrawComponent>(std::static_pointer_cast<Player>(GetThis()));

}

void Player::InitFunction()
{

	// �J�����𐶐�
	std::weak_ptr<Camera> camera = SceneManager::GetInstance().GetCamera();

	inputController_ = std::make_unique<InputController>();

	// �J�����̃^�[�Q�b�g���v���C���[�ɐݒ�
	camera.lock()->SetPlayer(transform_);

}

void Player::InitFunctionPointer()
{
	
	//�֐��|�C���^�̏�����
	stateChange_.emplace(PlayerState::IDLE, std::bind(&Player::ChangeIdle, this));
	stateChange_.emplace(PlayerState::RUN, std::bind(&Player::ChangeRun, this));
	stateChange_.emplace(PlayerState::ATTACK_JAB, std::bind(&Player::ChangeJab, this));
	stateChange_.emplace(PlayerState::ATTACK_STRAIGHT, std::bind(&Player::ChangeStraight, this));
	stateChange_.emplace(PlayerState::ATTACK_HOOK, std::bind(&Player::ChangeHook, this));
	stateChange_.emplace(PlayerState::ATTACK_LEFT_KICK, std::bind(&Player::ChangeLeftKick, this));
	stateChange_.emplace(PlayerState::ATTACK_RIGHT_KICK, std::bind(&Player::ChangeRightKick, this));
	stateChange_.emplace(PlayerState::ATTACK_UPPER, std::bind(&Player::ChangeUpper, this));
	stateChange_.emplace(PlayerState::ATTACK_CHARGE_PUNCH, std::bind(&Player::ChangeChargePunch, this));
	stateChange_.emplace(PlayerState::ATTACK_SPECIAL_PUNCH, std::bind(&Player::ChangeSpecialAttack, this));
	stateChange_.emplace(PlayerState::POWER_CHARGE, std::bind(&Player::ChangePowerCharge, this));
	stateChange_.emplace(PlayerState::EVASION, std::bind(&Player::ChangeEvasion, this));
	stateChange_.emplace(PlayerState::HIT_HEAD, std::bind(&Player::ChangeHitHead, this));
	stateChange_.emplace(PlayerState::HIT_BODY, std::bind(&Player::ChangeHitBody, this));
	stateChange_.emplace(PlayerState::DEATH, std::bind(&Player::ChangeDeath, this));

}

void Player::InitParameter()
{

	// �U���̓���
	for (int i = static_cast<int>(PlayerState::ATTACK_JAB); i <= static_cast<int>(PlayerState::ATTACK_RIGHT_KICK); i++)
	{
		isCombo_.emplace(static_cast<PlayerState>(i), false);
	}

	// ����
	dir_ = { 0.0f,0.0f,1.0f };

	// ��������
	moveDir_ = { 0.0f,0.0f,0.0f };

	// �{�X��̂݊p�x��ς���
	if (SceneManager::GetInstance().GetSceneID() == SCENE_ID::BOSS_BATTLE)
	{
		// �p�x��ύX
		transform_->quaRot = Quaternion::Euler({ Utility::Deg2RadF(0.0f) , Utility::Deg2RadF(180.0f),Utility::Deg2RadF(0.0f) });
		transform_->Update();
	}

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

	// �K�E�Z�̓����蔻��̍��W��ݒ�
	collisionData_.projectilePos = VAdd(transform_->pos, BODY_RELATIVE_CENTER_POS);

	// �葫�̏Փ˔���̔��a
	collisionData_.handAndFootCollisionRadius = HAND_AND_FOOT_COLLISION_RADIUS;

	// �̂̏Փ˔���̔��a
	collisionData_.bodyCollisionRadius = BODY_COLLISION_RADIUS;

	// �K�E�Z�̏Փ˔���̔��a
	collisionData_.projectileCollisionRadius = jsonData_["PROJECTILE_COLLISION_RADIUS"];

	// �E��ɍU�����肪���邩�ǂ���
	collisionData_.isRightHandAttack = false;

	// ����ɍU�����肪���邩�ǂ���
	collisionData_.isLeftHandAttack = false;

	// �E���ɍU�����肪���邩�ǂ���
	collisionData_.isRightFootAttack = false;

	// �����ɍU�����肪���邩�ǂ���
	collisionData_.isLeftFootAttack = false;

	// �K�E�Z�ɍU�����肪���邩�ǂ���
	collisionData_.isProjectileAttack = false;

	// ���߃p���`�̃J�E���^
	attackChargePunchCnt_ = 0.0f;

	// HP�̍ő�l
	HP_MAX = jsonData_["HP"];

	// HP
	hp_ = HP_MAX;

	// HP�o�[�̒���
	HP_BAR_LENGTH = jsonData_["HP_BAR_LENGTH"];

	// �K�E�Z�Q�[�W
	specialAttackGauge_ = 0;

	// �K�E�Z�Q�[�W�̍ő�l
	SPECIAL_ATTACK_MAX_GAUGE = jsonData_["SPECIAL_ATTACK_MAX_GAUGE"];
		
	// �K�E�Z�Q�[�W�̒���
	SPECIAL_ATTACK_GAUGE_LENGTH = jsonData_["SPECIAL_ATTACK_GAUGE_LENGTH"];

	// ����Ƃ��̈ړ���
	RUN_MOVE_POW = jsonData_["RUN_MOVE_POW"];

	// �A�j���[�V�����ԍ�
	ANIM_INDEX = jsonData_["ANIM_INDEX"];

	// �Q�[���V�[���̏����擾
	std::shared_ptr<BossBattleScene> bossBattleScene =
		std::dynamic_pointer_cast<BossBattleScene>(SceneManager::GetInstance().GetNowScene());

	// NULL�`�F�b�N
	if (!bossBattleScene)return;

	// �A�N�^�[�}�l�[�W���[���擾
	if (bossBattleScene)
	{

		// �v���C���[�K�E�Z�̃Q�[�W��ݒ� 
		specialAttackGauge_ = SceneManager::GetInstance().GetPlayerSpecialAttackGauge();
		//specialAttackGauge_ = 0;

		// �v���C���[��HP��ݒ�
		hp_ = SceneManager::GetInstance().GetPlayerHp();
		//hp_ = 100;

	}

}

void Player::InitImage()
{

	// �A�C�R���̉摜
	iconImg_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::IMAGE_PLAYER_ICON)]).handleId_;

	// HP�o�[�̉摜
	hpBarImg_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::IMAGE_PLAYER_HP_BAR)]).handleId_;

}

void Player::InitAnimation()
{

	// �A�j���[�V�����R���g���[���̐���
	animationController_ = std::make_unique<AnimationController>(transform_->modelId);

	// �A�j���[�V�����R���g���[���[�ɃA�j���[�V������ǉ�
	for (int i = static_cast<int>(PlayerState::IDLE); i < static_cast<int>(PlayerState::MAX); ++i)
	{

		// ���[�v�Đ�����A�j���[�V��������isLoop��true�ɂ���
		bool isLoop = i == static_cast<int>(PlayerState::IDLE) || i == static_cast<int>(PlayerState::RUN);
		animationController_->Add(

			// �A�j���[�V�����̖��O
			jsonData_["ANIM"][i - 1]["NAME"],

			// �A�j���[�V�������n�܂鎞��
			0.0f,

			// �A�j���[�V�����X�s�[�h
			jsonData_["ANIM"][i - 1]["SPEED"],

			// �A�j���[�V�����n���h��
			resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::MODEL_PLAYER) + i]).handleId_,

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
	ChangeState(PlayerState::IDLE);

}

void Player::InitBGMAndSE()
{

	// �����̏�����
	footStepsSE_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::SOUND_PLAYER_FOOT_STEPS_SE)]).handleId_;

	// �����̃{�����[���̕ύX
	ChangeVolumeSoundMem(255 * 60 / 100, footStepsSE_);

	// �W���u�̉��̏�����
	jabSE_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::SOUND_PLAYER_JAB_SE)]).handleId_;

	// �X�g���[�g�̉��̏�����
	straightSE_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::SOUND_PLAYER_STRAIGHT_SE)]).handleId_;

	// �t�b�N�̉��̏�����
	hookSE_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::SOUND_PLAYER_HOOK_SE)]).handleId_;

	// �A�b�p�[�̉��̏�����
	upperSE_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::SOUND_PLAYER_UPPER_SE)]).handleId_;

	// ���ߍU���̉��̏�����
	chargePunchSE_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::SOUND_PLAYER_CHARGE_PUNCH_SE)]).handleId_;
	
	// ���L�b�N�̉��̏�����
	leftKickSE_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::SOUND_PLAYER_LEFT_KICK_SE)]).handleId_;
	
	// �E�L�b�N�̉��̏�����
	rightKickSE_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::SOUND_PLAYER_RIGHT_KICK_SE)]).handleId_;

	// �K�E�ւ̂̉��̏�����
	specialAttackSE_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::SOUND_PLAYER_SPECIAL_ATTACK_SE)]).handleId_;

}

void Player::Update(const float deltaTime)
{

	// ImGui�̃f�o�b�O�`��̍X�V
	//UpdateDebugImGui();

	// ���͂̍X�V
	inputComponent_->Update(deltaTime);

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

	// �A�j���[�V�����̃t���[�����Œ�
	AnimationFrame();

	// �����Ă���Ƃ��ȊO�͑������~�߂�
	if (state_ != PlayerState::RUN)
	{
		StopSoundMem(footStepsSE_);
	}

}

void Player::Draw(const float deltaTime)
{

	// ���N���X�̕`�揈��
	ActorBase::Draw(deltaTime);

	// HP�o�[�`��
	int H;
	int hpGauge;
	H = hp_ * (static_cast<int>(512.0f) / HP_MAX) - 100;
	int R = min(max((384 - H), 0), 0xff);
	int G = min(max((H + 64), 0), 0xff);
	int B = max((H - 384), 0);
	hpGauge = HP_BAR_LENGTH * hp_ / HP_MAX;

	// �K�E�Z�Q�[�W�̕`��
	int specialAttackGauge = SPECIAL_ATTACK_GAUGE_LENGTH * specialAttackGauge_ / SPECIAL_ATTACK_MAX_GAUGE;

	if (hp_ >= 0)
	{

		// HP�o�[
		DrawBox(200, 550, 200 + hpGauge, 570, GetColor(R, G, B), true);

		// HP�o�[�̔�
		DrawExtendGraph(200, 550, 200 + HP_BAR_LENGTH, 570, hpBarImg_, true);

		// �K�E�Z�Q�[�W
		DrawBox(200, 600, 200 + specialAttackGauge, 620, 0x65bbe9, true);

		// �K�E�Z�Q�[�W�̔�
		DrawExtendGraph(200, 600, 200 + SPECIAL_ATTACK_GAUGE_LENGTH, 620, hpBarImg_, true);

	}

	// �A�C�R���̕`��
	DrawRotaGraph(100, 550, 0.5, 0.0, iconImg_, true);

}

void Player::UpdateDebugImGui()
{

	// �E�B���h�E�^�C�g��&�J�n����
	ImGui::Begin("Player");

	// �傫��
	ImGui::Text("scale");
	ImGui::InputFloat("Scl", &scl_);

	// �p�x
	VECTOR rotDeg = VECTOR();
	rotDeg.x = Utility::Rad2DegF(static_cast<float>(transform_->quaRot.x));
	rotDeg.y = Utility::Rad2DegF(static_cast<float>(transform_->quaRot.y));
	rotDeg.z = Utility::Rad2DegF(static_cast<float>(transform_->quaRot.z));
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

void Player::AnimationFrame()
{

	// �Ώۃt���[���̃��[�J���s��������l�Ƀ��Z�b�g����
	MV1ResetFrameUserLocalMatrix(transform_->modelId, collisionData_.body);

	// ���W���Œ肷��
	if (animationController_->IsBlendPlay("EVASION"))
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

const bool Player::GetAttackState()const
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

const bool Player::GetCloseRangeAttackState() const
{

	// �ߐڍU���̏�Ԃ�����
	for (const auto state : closeRangeAttackState_)
	{
		if (state_ == state)
		{
			return true;
		}
	}

	return false;

}

const std::vector<int> Player::GetTotalAttackTypes() const
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

const bool Player::GetHitState()const
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

const bool Player::GetIsSuperArmor() const
{
	// �X�[�p�[�A�[�}�[��Ԃ�����
	for (const auto state : superArmorState_)
	{
		if (state_ == state)
		{
			return true;
		}
	}

	return false;
}

bool Player::GetComboState()
{

	// �R���{��������
	for (const auto state : comboState_)
	{
		if (state_ == state)
		{
			return true;
		}
	}

	return false;

}

void Player::AttackHit(const int damage, const int state)
{

	// �X�[�p�[�A�[�}�[��Ԃ�����
	if (GetIsSuperArmor())
	{
		return;
	}

	// �ǂ̃A�j���[�V�������`�F�b�N����
	AttackHitCheck(state);

	// HP�����炷
	SubHp(damage);
	
	if (hp_ <= 0)
	{

		// 0�ȉ��ɂȂ�Ȃ��悤�ɂ���
		hp_ = 0;

		// ���ʃA�j���[�V�����ɑJ��
		DeathAnim(state);

	}

	// �A�j���[�V�����̍Đ����Ԃ����Z�b�g����
	animationController_->ResetStepAnim();

}

void Player::ProjectileHit(const int damage)
{

	// �X�[�p�[�A�[�}�[��Ԃ�����
	if (GetIsSuperArmor())
	{
		return;
	}

	// �q�b�g�A�j���[�V�����ɑJ��
	ChangeState(PlayerState::HIT_BODY);

	// HP�����炷
	SubHp(damage);

	// �A�j���[�V�����̍Đ����Ԃ����Z�b�g����
	animationController_->ResetStepAnim();

}

void Player::AttackHitCheck(const int type)
{

	// ���Ƀq�b�g����A�j���[�V�������`�F�b�N
	for (const auto hitState : hitHeadEnemyState_)
	{
		if (hitState == static_cast<EnemyState>(type))
		{
			ChangeState(PlayerState::HIT_HEAD);
			return;
		}
	}

	// ���Ƀq�b�g����A�j���[�V�������`�F�b�N
	for (const auto hitState : hitHeadBossState_)
	{
		if (hitState == static_cast<BossState>(type))
		{
			ChangeState(PlayerState::HIT_HEAD);
			return;
		}
	}

	// �̂Ƀq�b�g����A�j���[�V�������`�F�b�N
	for (const auto hitState : hitBodyEnemyState_)
	{
		if (hitState == static_cast<EnemyState>(type))
		{
			ChangeState(PlayerState::HIT_BODY);
			return;
		}
	}

	// �̂Ƀq�b�g����A�j���[�V�������`�F�b�N
	for (const auto hitState : hitBodyBossState_)
	{
		if (hitState == static_cast<BossState>(type))
		{
			ChangeState(PlayerState::HIT_BODY);
			return;
		}
	}

}

void Player::Rotation()
{

	float cameraAngleY = 0.0f;

	// �J�����̊p�x
	if (!Utility::EqualsVZero(inputController_->Dir()))
	{
		cameraAngleY_ = SceneManager::GetInstance().GetCamera().lock()->GetAngle().y;
		cameraAngleY = cameraAngleY_;
	}

	// Y���̍s��
	MATRIX mat = MGetIdent();
	mat = MMult(mat, MGetRotY(cameraAngleY_));

	// ��]�s����g�p���āA�x�N�g������]������
	moveDir_ = VTransform(dir_, mat);

	// �������p�x�ɕϊ�����(XZ���� Y��)
	float angle = atan2f(dir_.x, dir_.z);
	
	// ��������]����
	if (cameraAngleY != 0.0f)
	{
		LazyRotation(cameraAngleY + angle);
	}

}

void Player::DeathAnim(int state)
{

	// ���S�A�j���[�V�������`�F�b�N
	for (const auto hitState : deathEnemyState_)
	{
		if (hitState == static_cast<EnemyState>(state))
		{
			ChangeState(PlayerState::DEATH);
			return;
		}
	}

	// ���S�A�j���[�V�������`�F�b�N
	for (const auto hitState : deathBossState_)
	{
		if (hitState == static_cast<BossState>(state))
		{
			ChangeState(PlayerState::DEATH);
			return;
		}
	}

}

void Player::ChangeState(const PlayerState state)
{

	// �O�̃A�j���[�V������ۑ�
	preKey_ = key_;

	// �V�����A�j���[�V������ۑ�
	key_ = ANIM_DATA_KEY[static_cast<int>(state)];

	// �O�Ɠ����A�j���[�V�����Ȃ珈�����Ȃ�
	if (preKey_ == key_)return;

	// ��ԑJ��
	state_ = state;

	// �֐��|�C���^�̑J��
	stateChange_[state_]();

	// �O�Ɠ�����ԂłȂ���΃J�E���^�����Z�b�g����
	if (preKey_ != key_)
	{
		// ���߃p���`�̃J�E���^�����Z�b�g
		attackChargePunchCnt_ = 0.0f;
	}

	// �A�j���[�V�����R���g���[���[���̃A�j���[�V�����J��
	animationController_->ChangeAnimation(key_);

}

void Player::ChangeIdle(void)
{

	stateUpdate_ = std::bind(&Player::UpdateIdle, this, std::placeholders::_1);

	// �d�͂�ʏ��Ԃɖ߂�
	gravityScale_ = 1.0f;

}

void Player::ChangeRun(void)
{

	stateUpdate_ = std::bind(&Player::UpdateRun, this, std::placeholders::_1);

	// �X�s�[�h
	speed_ = RUN_MOVE_POW;

}

void Player::ChangeJab()
{

	stateUpdate_ = std::bind(&Player::UpdateJab, this, std::placeholders::_1);

	// ����̍U��������Ȃ���
	collisionData_.isLeftHandAttack = false;

	// �_���[�W��
	damage_ = ATTACK_JAB_DAMAGE;

	// �X�s�[�h
	speed_ = ATTACK_MOVE_POW;

	// �ړ�����
	inputComponent_->Dir();

}

void Player::ChangeStraight()
{

	stateUpdate_ = std::bind(&Player::UpdateStraight, this, std::placeholders::_1);

	// �E��̍U��������Ȃ���
	collisionData_.isRightHandAttack = false;

	// �_���[�W��
	damage_ = ATTACK_STRAIGHT_DAMAGE;

	// �X�s�[�h
	speed_ = ATTACK_MOVE_POW;

	// �ړ�����
	inputComponent_->Dir();

}

void Player::ChangeHook()
{

	stateUpdate_ = std::bind(&Player::UpdateHook, this, std::placeholders::_1);

	// ����̍U��������Ȃ���
	collisionData_.isLeftHandAttack = false;

	// �_���[�W��
	damage_ = ATTACK_HOOK_DAMAGE;

	// �X�s�[�h
	speed_ = ATTACK_MOVE_POW;

	// �ړ�����
	inputComponent_->Dir();

}

void Player::ChangeLeftKick()
{

	stateUpdate_ = std::bind(&Player::UpdateLeftKick, this, std::placeholders::_1);

	// �����̍U��������Ȃ���
	collisionData_.isLeftFootAttack = false;

	// �_���[�W��
	damage_ = ATTACK_LEFT_KICK_DAMAGE;

	// �X�s�[�h
	speed_ = ATTACK_MOVE_POW;

	// �ړ�����
	inputComponent_->Dir();

}

void Player::ChangeRightKick()
{

	stateUpdate_ = std::bind(&Player::UpdateRightKick, this, std::placeholders::_1);

	// �E���̍U��������Ȃ���
	collisionData_.isRightFootAttack = false;

	// �_���[�W��
	damage_ = ATTACK_RIGHT_KICK_DAMAGE;

	// �X�s�[�h
	speed_ = ATTACK_MOVE_POW;

	// �ړ�����
	inputComponent_->Dir();

}

void Player::ChangeUpper()
{

	stateUpdate_ = std::bind(&Player::UpdateUpper, this, std::placeholders::_1);

	// �E��̍U��������Ȃ���
	collisionData_.isRightHandAttack = false;

	// �_���[�W��
	damage_ = ATTACK_UPPER_DAMAGE;

	// �X�s�[�h
	speed_ = ATTACK_MOVE_POW;

}

void Player::ChangeChargePunch()
{

	stateUpdate_ = std::bind(&Player::UpdateChargePunch, this, std::placeholders::_1);

	// �E��̍U��������Ȃ���
	collisionData_.isRightHandAttack = false;

	// �_���[�W��
	damage_ = ATTACK_CHARGE_PUNCH_DAMAGE;

	// �X�s�[�h
	speed_ = ATTACK_MOVE_POW;

}

void Player::ChangeSpecialAttack()
{

	stateUpdate_ = std::bind(&Player::UpdateSpecialAttack, this, std::placeholders::_1);

	// �_���[�W��
	damage_ = ATTACK_SPECIAL_PUNCH_DAMAGE;

	// �K�E�Z�Q�[�W�����Z�b�g
	specialAttackGauge_ = 0;

	// �K�E�Z�̓����蔻��̍��W��ݒ�
	collisionData_.projectilePos = VAdd(transform_->pos, BODY_RELATIVE_CENTER_POS);

	// �K�E�Z�̏Փ˔��肪�������Ԃ̃J�E���^�����Z�b�g
	attackSpecialPunchCollisionCnt_ = 0.0f;

	// �J�����𐶐�
	std::weak_ptr<Camera> camera = SceneManager::GetInstance().GetCamera();
	camera.lock()->ChangeMode(Camera::MODE::FOLLOW);

}

void Player::ChangePowerCharge()
{

	stateUpdate_ = std::bind(&Player::UpdatePowerCharge, this, std::placeholders::_1);

	// �J�����𐶐�
	std::weak_ptr<Camera> camera = SceneManager::GetInstance().GetCamera();
	camera.lock()->ChangeMode(Camera::MODE::SPECIAL);

}

void Player::ChangeEvasion()
{

	stateUpdate_ = std::bind(&Player::UpdateEvasion, this, std::placeholders::_1);

	// �X�s�[�h
	speed_ = EVASION_MOVE_POW;

}

void Player::ChangeHitHead()
{

	stateUpdate_ = std::bind(&Player::UpdateHitHead, this, std::placeholders::_1);

	// �v���C���[�̕��������߂�
	VECTOR vec = VSub(targetPos_, transform_->pos);

	// ���K��
	vec = VNorm(vec);

	// �v���C���[�̕����Ƌt�����̃x�N�g��
	vec = { -vec.x, vec.y,-vec.z };

	// �X�s�[�h
	speed_ = ATTACK_MOVE_POW;

}

void Player::ChangeHitBody()
{

	stateUpdate_ = std::bind(&Player::UpdateHitBody, this, std::placeholders::_1);

	// �v���C���[�̕��������߂�
	VECTOR vec = VSub(targetPos_, transform_->pos);

	// ���K��
	vec = VNorm(vec);

	// �v���C���[�̕����Ƌt�����̃x�N�g��
	vec = { -vec.x, vec.y,-vec.z };

	// �X�s�[�h
	speed_ = ATTACK_MOVE_POW;

}

void Player::ChangeDeath()
{

	stateUpdate_ = std::bind(&Player::UpdateDeath, this, std::placeholders::_1);

	SceneManager::GetInstance().GetCamera().lock()->ChangeMode(Camera::MODE::GAME_OVER);

}

void Player::UpdateIdle(const float deltaTime)
{

	// �U���̓��͂�������
	for (int i = static_cast<int>(PlayerState::ATTACK_JAB); i <= static_cast<int>(PlayerState::ATTACK_RIGHT_KICK); i++)
	{
		isCombo_.at(static_cast<PlayerState>(i)) = false;
	}

}

void Player::UpdateRun(const float deltaTime)
{

	// �U���̓��͂�������
	for (int i = static_cast<int>(PlayerState::ATTACK_JAB); i <= static_cast<int>(PlayerState::ATTACK_RIGHT_KICK); i++)
	{
		isCombo_.at(static_cast<PlayerState>(i)) = false;
	}

	// ��]����
	Rotation();

	// �ړ�����
	moveComponent_->Move();

	if (!CheckSoundMem(footStepsSE_))
	{
		// �������Đ�
		PlaySoundMem(footStepsSE_, DX_PLAYTYPE_BACK, true);
	}

}

void Player::UpdateJab(const float deltaTime)
{

	// ��]����
	Rotation();

	// �����O�ɂ������ړ�
	moveComponent_->Lerp();

	// �U�����肪����t���[��
	if (ATTACK_JAB_START_FRAME <= animationController_->GetStepAnim() && animationController_->GetStepAnim() <= ATTACK_JAB_END_FRAME)
	{
		collisionData_.isLeftHandAttack = true;
	}
	else
	{
		collisionData_.isLeftHandAttack = false;
	}

	// �U�����������Ă����特���Đ�
	if (isHitAttack_)
	{
		PlaySoundMem(jabSE_, DX_PLAYTYPE_BACK, true);
		isHitAttack_ = false;
	}

	// �X�g���[�g�ɑJ��
	if (animationController_->IsEndPlayAnimation() && isCombo_.at(PlayerState::ATTACK_STRAIGHT))
	{
		ChangeState(PlayerState::ATTACK_STRAIGHT);
	}

}

void Player::UpdateStraight(const float deltaTime)
{

	// ��]����
	Rotation();

	// �����O�ɂ������ړ�
	moveComponent_->Lerp();

	// �U�����肪����t���[��
	if (ATTACK_STRAIGHT_START_FRAME <= animationController_->GetStepAnim() && animationController_->GetStepAnim() <= ATTACK_STRAIGHT_END_FRAME)
	{
		collisionData_.isRightHandAttack = true;
	}
	else
	{
		collisionData_.isRightHandAttack = false;
	}

	// �U�����������Ă����特���Đ�
	if (isHitAttack_)
	{
		PlaySoundMem(straightSE_, DX_PLAYTYPE_BACK, true);
		isHitAttack_ = false;
	}

	// �t�b�N�ɑJ��
	if (animationController_->IsEndPlayAnimation() && isCombo_.at(PlayerState::ATTACK_HOOK))
	{
		ChangeState(PlayerState::ATTACK_HOOK);
	}

}

void Player::UpdateHook(const float deltaTime)
{

	// ��]����
	Rotation();

	// �����O�ɂ������ړ�
	moveComponent_->Lerp();

	// �U�����肪����t���[��
	if (ATTACK_HOOK_START_FRAME <= animationController_->GetStepAnim() && animationController_->GetStepAnim() <= ATTACK_HOOK_END_FRAME)
	{
		collisionData_.isLeftHandAttack = true;
	}
	else
	{
		collisionData_.isLeftHandAttack = false;
	}

	// �U�����������Ă����特���Đ�
	if (isHitAttack_)
	{
		PlaySoundMem(hookSE_, DX_PLAYTYPE_BACK, true);
		isHitAttack_ = false;
	}

	// ���L�b�N�ɑJ��
	if (animationController_->IsEndPlayAnimation() && isCombo_.at(PlayerState::ATTACK_LEFT_KICK))
	{
		ChangeState(PlayerState::ATTACK_LEFT_KICK);
	}

}

void Player::UpdateLeftKick(const float deltaTime)
{

	// ��]����
	Rotation();

	// �����O�ɂ������ړ�
	moveComponent_->Lerp();

	// �U�����肪����t���[��
	if (ATTACK_LEFT_KICK_START_FRAME <= animationController_->GetStepAnim() && animationController_->GetStepAnim() <= ATTACK_LEFT_KICK_END_FRAME)
	{
		collisionData_.isLeftFootAttack = true;
	}
	else
	{
		collisionData_.isLeftFootAttack = false;
	}

	// �U�����������Ă����特���Đ�
	if (isHitAttack_)
	{
		PlaySoundMem(leftKickSE_, DX_PLAYTYPE_BACK, true);
		isHitAttack_ = false;
	}

	// �E�L�b�N�ɑJ��
	if (animationController_->IsEndPlayAnimation() && isCombo_.at(PlayerState::ATTACK_RIGHT_KICK))
	{
		ChangeState(PlayerState::ATTACK_RIGHT_KICK);
	}

}

void Player::UpdateRightKick(const float deltaTime)
{

	// ��]����
	Rotation();

	// �����O�ɂ������ړ�
	moveComponent_->Lerp();

	// �U�����肪����t���[��
	if (ATTACK_RIGHT_KICK_START_FRAME <= animationController_->GetStepAnim() && animationController_->GetStepAnim() <= ATTACK_RIGHT_KICK_END_FRAME)
	{
		collisionData_.isRightFootAttack = true;
	}
	else
	{
		collisionData_.isRightFootAttack = false;
	}

	// �U�����������Ă����特���Đ�
	if (isHitAttack_)
	{
		PlaySoundMem(rightKickSE_, DX_PLAYTYPE_BACK, true);
		isHitAttack_ = false;
	}

	// �ҋ@��ԂɑJ��
	if (animationController_->IsEndPlayAnimation())
	{
		ChangeState(PlayerState::IDLE);
	}

}

void Player::UpdateUpper(const float deltaTime)
{

	// �����O�ɂ������ړ�
	moveComponent_->Lerp();

	// �U�����肪����t���[��
	if (ATTACK_UPPER_START_FRAME <= animationController_->GetStepAnim() && animationController_->GetStepAnim() <= ATTACK_UPPER_END_FRAME)
	{
		collisionData_.isRightHandAttack = true;
	}
	else
	{
		collisionData_.isRightHandAttack = false;
	}

	// �U�����������Ă����特���Đ�
	if (isHitAttack_)
	{
		PlaySoundMem(upperSE_, DX_PLAYTYPE_BACK, true);
		isHitAttack_ = false;
	}

	// �ҋ@��ԂɑJ��
	if (animationController_->IsEndPlayAnimation())
	{
		ChangeState(PlayerState::IDLE);
	}

}

void Player::UpdateChargePunch(const float deltaTime)
{

	// �����O�ɂ������ړ�
	moveComponent_->Lerp();

	// �U�����肪����t���[��
	if (ATTACK_CHARGE_PUNCH_START_FRAME <= animationController_->GetStepAnim() && animationController_->GetStepAnim() <= ATTACK_CHARGE_PUNCH_END_FRAME)
	{
		collisionData_.isRightHandAttack = true;
	}
	else
	{
		collisionData_.isRightHandAttack = false;
	}

	// �U�����������Ă����特���Đ�
	if (isHitAttack_)
	{
		PlaySoundMem(chargePunchSE_, DX_PLAYTYPE_BACK, true);
		isHitAttack_ = false;
	}

	// �ҋ@��ԂɑJ��
	if (animationController_->IsEndPlayAnimation())
	{
		ChangeState(PlayerState::IDLE);
	}

}

void Player::UpdateSpecialAttack(const float deltaTime)
{

	// �U�����肪����t���[��
	if (ATTACK_SPECIAL_PUNCH_START_FRAME <= animationController_->GetStepAnim() && ATTACK_SPECIAL_PUNCH_COLLISION_TIME >= attackSpecialPunchCollisionCnt_)
	{

		// �����蔻�������
		collisionData_.isProjectileAttack = true;

		// �K�E�Z�̓����蔻��̍��W�ݒ�
		collisionData_.projectilePos = VAdd(collisionData_.projectilePos, VScale(transform_->quaRot.GetForward(), 1000.0f));

		// �K�E�Z�̏Փ˔��肪�������Ԃ̃J�E���^�����Z
		attackSpecialPunchCollisionCnt_ += deltaTime;

	}

	// �U�����������Ă����特���Đ�
	if (isHitAttack_)
	{
		PlaySoundMem(specialAttackSE_, DX_PLAYTYPE_BACK, true);
		isHitAttack_ = false;
	}

	// �ҋ@��ԂɑJ��
	if (animationController_->IsEndPlayAnimation())
	{
		ChangeState(PlayerState::IDLE);

		// �����蔻�������
		collisionData_.isProjectileAttack = false;

	}

}

void Player::UpdatePowerCharge(const float deltaTime)
{

	// �K�E�Z�ɑJ��
	if (animationController_->IsEndPlayAnimation())
	{
		ChangeState(PlayerState::ATTACK_SPECIAL_PUNCH);
	}

}

void Player::UpdateEvasion(const float deltaTime)
{

	// �ړ�����
	moveComponent_->Move();

	// �ҋ@��ԂɑJ��
	if (animationController_->IsEndPlayAnimation())
	{
		ChangeState(PlayerState::IDLE);
	}

}

void Player::UpdateHitHead(const float deltaTime)
{

	// �ҋ@��ԂɑJ��
	if (animationController_->IsEndPlayAnimation())
	{
		ChangeState(PlayerState::IDLE);
	}

}

void Player::UpdateHitBody(const float deltaTime)
{

	// �ҋ@��ԂɑJ��
	if (animationController_->IsEndPlayAnimation())
	{
		ChangeState(PlayerState::IDLE);
	}

}

void Player::UpdateDeath(const float deltaTime)
{

	// �ҋ@��ԂɑJ��
	if (animationController_->IsEndPlayAnimation())
	{

		// �Q�[���V�[���̏����擾
		std::shared_ptr<GameScene> gameScene =
			std::dynamic_pointer_cast<GameScene>(SceneManager::GetInstance().GetNowScene());

		// �{�X�o�g���V�[���̏����擾
		std::shared_ptr<BossBattleScene> bossBattleScene =
			std::dynamic_pointer_cast<BossBattleScene>(SceneManager::GetInstance().GetNowScene());

		// NULL�`�F�b�N
		if (gameScene)
		{
			// �A�j���V�����Đ����I������玀��
			gameScene->SetIsPlayerDeath(true);
		}

		// NULL�`�F�b�N
		if (bossBattleScene)
		{
			// �A�j���V�����Đ����I������玀��
			bossBattleScene->SetIsPlayerDeath(true);
		}

	}

}