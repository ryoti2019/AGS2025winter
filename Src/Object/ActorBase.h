#pragma once
#include <DxLib.h>
#include <memory>
#include <vector>
#include "../Lib/nlohmann/json.hpp"
#include "../Common/Vector2F.h"
#include "../Object/Common/Transform.h"
#include "../Component/MoveComponent.h"
#include "../Manager/ResourceManager.h"
#include "../Object/Common/AnimationController.h"

class Component;
class Camera;

using json = nlohmann::json;

enum class ActorType {
	NONE = -1,
	PLAYER,
	ENEMY,
	STAGE,
	MAX
};

class ActorBase : public std::enable_shared_from_this<ActorBase>
{

public:

	struct CollisionData
	{

#pragma region �U����X�e�[�W�Ƃ̓����蔻��Ŏg������

#pragma region �t���[��

		// �E��̃t���[��
		int rightHand;

		// ����̃t���[��
		int leftHand;

		// �E���̃t���[��
		int rightFoot;

		// �����̃t���[��
		int leftFoot;

		// �̑S�̂̃t���[��
		int body;

#pragma endregion

#pragma region ���S���W

		// �E��̒��S���W
		VECTOR rightHandPos;

		// ����̒��S���W
		VECTOR leftHandPos;

		// �E���̒��S���W
		VECTOR rightFootPos;

		// �����̒��S���W
		VECTOR leftFootPos;

		// �̑S�̂̒��S���W
		VECTOR bodyPos;

#pragma endregion

#pragma region �J�v�Z���̏���W

		// �E��̃J�v�Z���̏���W
		VECTOR rightHandCapsuleUpPos;

		// ����̃J�v�Z���̏���W
		VECTOR leftHandCapsuleUpPos;

		// �E���̃J�v�Z���̏���W
		VECTOR rightFootCapsuleUpPos;

		// �����̃J�v�Z���̏���W
		VECTOR leftFootCapsuleUpPos;

		// �̂̃J�v�Z���̏���W
		VECTOR bodyCapsuleUpPos;

#pragma endregion

#pragma region �J�v�Z���̉����W

		// �E��̃J�v�Z���̉����W
		VECTOR rightHandCapsuleDownPos;

		// ����̃J�v�Z���̉����W
		VECTOR leftHandCapsuleDownPos;

		// �E���̃J�v�Z���̉����W
		VECTOR rightFootCapsuleDownPos;

		// �����̃J�v�Z���̉����W
		VECTOR leftFootCapsuleDownPos;

		// �̂̃J�v�Z���̉����W
		VECTOR bodyCapsuleDownPos;

#pragma endregion

#pragma region ����

		// �E��̌���
		Quaternion rightHandRot;

		// ����̌���
		Quaternion leftHandRot;

		// �E���̌���
		Quaternion rightFootRot;

		// �����̌���
		Quaternion leftFootRot;

#pragma endregion

#pragma region ���a

		// �葫�̓����蔻��̔��a
		float handAndFootCollisionRadius;

		// �̂̓����蔻��̔��a
		float bodyCollisionRadius;

#pragma endregion

#pragma region �U�����肪���邩�ǂ���

		// �E��ɍU�����肪���邩�ǂ���
		bool isRightHandAttack;

		// ����ɍU�����肪���邩�ǂ���
		bool isLeftHandAttack;

		// �E���ɍU�����肪���邩�ǂ���
		bool isRightFootAttack;

		// �����ɍU�����肪���邩�ǂ���
		bool isLeftFootAttack;

#pragma endregion

#pragma endregion

#pragma region �v���C���[�ƓG���d�Ȃ�Ȃ��悤�ɂ��邽�߂ɓ����蔻��Ŏg������

		// �ŏ��_
		VECTOR minPos;

		// �ő�_
		VECTOR maxPos; 

#pragma endregion

	};

	// �E��̃J�v�Z���̏�̑��΍��W
	const VECTOR RIGHT_HAND_RELATIVE_UP_POS;

	// �E��̃J�v�Z���̉��̑��΍��W
	const VECTOR RIGHT_HAND_RELATIVE_DOWN_POS;

	// ����̃J�v�Z���̏�̑��΍��W
	const VECTOR LEFT_HAND_RELATIVE_UP_POS;

	// ����̃J�v�Z���̉��̑��΍��W
	const VECTOR LEFT_HAND_RELATIVE_DOWN_POS;

	// �E���̃J�v�Z���̏�̑��΍��W
	const VECTOR RIGHT_FOOT_RELATIVE_UP_POS;

	// �E���̃J�v�Z���̉��̑��΍��W
	const VECTOR RIGHT_FOOT_RELATIVE_DOWN_POS;

	// �����̃J�v�Z���̏�̑��΍��W
	const VECTOR LEFT_FOOT_RELATIVE_UP_POS;

	// �����̃J�v�Z���̉��̑��΍��W
	const VECTOR LEFT_FOOT_RELATIVE_DOWN_POS;

	// �̂̃J�v�Z���̒��S�̑��΍��W
	const VECTOR BODY_RELATIVE_CENTER_POS;

	// �̂̃J�v�Z���̏�̑��΍��W
	const VECTOR BODY_RELATIVE_UP_POS;

	// �̂̃J�v�Z���̉��̑��΍��W
	const VECTOR BODY_RELATIVE_DOWN_POS;

	// �E��̃t���[����
	std::string RIGHT_HAND_FRAME;

	// ����̃t���[����
	std::string LEFT_HAND_FRAME;

	// �E���̃t���[����
	std::string RIGHT_FOOT_FRAME;

	// �����̃t���[����
	std::string LEFT_FOOT_FRAME;

	// �̂̃t���[����
	std::string BODY_FRAME;

	// ����Ƃ��̈ړ���
	float RUN_MOVE_POW;

	// �U������Ƃ��ɐi�ވړ���
	const float ATTACK_MOVE_POW;
	
	// �d�͉����x
	const float GRAVITY;

	// �U�����󂯂��Ƃ��ɐi�ވړ���
	float HIT_MOVE_POW;

	// ��]��
	const float ROTATION_POW;

	// �葫�̓����蔻��̔��a
	const float HAND_AND_FOOT_COLLISION_RADIUS;

	// �̂̓����蔻��̔��a
	const float BODY_COLLISION_RADIUS;

	// �ŏ��Ɍ����Ă���p�x
	float INIT_ANGLE;

	// ���̍���
	const float FLOOR_HEIGHT;

	// HP�̍ő�l
	int HP_MAX;

	// �A�j���[�V�����ԍ�
	int ANIM_INDEX;

	ActorBase(const VECTOR& pos, const json& data);

	virtual ~ActorBase() = default;

	virtual void Init(const VECTOR& pos);
	void Create(const VECTOR& pos);
	virtual void Update(const float deltaTime);
	virtual void Draw();

	// ���W��ݒ�
	void SetPos(const VECTOR& pos) {
		transform_->pos = pos;
		transform_->Update();
		// �Փ˔���̍X�V
		ActorBase::CollisionUpdate();
	}

	// ����̍��W���擾
	const VECTOR& GetTargetPos()const { return targetPos_; }

	// ����̍��W��ݒ�
	void SetTargetPos(const VECTOR& pos) { targetPos_ = pos; }

	// ���������ݒ�
	void SetIsActive(const bool isActive) { isActive_ = isActive; }

	// ���W�𑫂�
	void AddPos(const VECTOR& pos);

	// ���W�𑫂�
	void SubPos(const VECTOR& pos) { transform_->pos = VSub(transform_->pos, pos); }

	// HP��ݒ�
	void SetHp(const int hp) { hp_ = hp; }

	// HP�����炷
	void SubHp(const int hp) { hp_ -= hp;}

	// ���b�N�I������Ă��邩�����ݒ�
	void SetIsLockOn(const bool isLockOn);

	// ���f���̊�{�����擾
	const std::shared_ptr<Transform>& GetTransform() const { return transform_; }

	// �A�N�^�[�^�C�v���擾
	const ActorType& GetActorType() const { return actorType_; }

	// �R���W�����f�[�^�̂��擾
	const CollisionData& GetCollisionData() const { return collisionData_; }

	// �A�N�^�[�̏d�͕����̑��x�̎擾
	const VECTOR& GetVelocity()const { return velocity_; }

	// �A�N�^�[�̏d�͕����̑��x�̐ݒ�
	void SetVelocity(const VECTOR& velocity) { velocity_ = velocity; }

	// ����������擾
	const bool GetIsActive() const { return isActive_; }

	// �U����Ԃ��擾
	virtual const bool GetAttackState()const ;

	// �U����ނ��擾
	virtual const std::vector<int> GetTotalAttackTypes()const { return {}; }

	// �U�����󂯂Ă����Ԃ��擾
	virtual const bool GetHitState()const;

	// �U�����������Ă��邩�ݒ�
	void SetIsAttackHit(const bool hit) { isAttackHit_ = hit; }

	// �U�����������Ă��邩
	const bool GetIsAttackHit()const { return isAttackHit_; }

	// �n�ʂɓ������Ă��邩
	void SetIsOnGround(const bool isOnGround) { isOnGround_ = isOnGround; }

	// HP���擾
	const int GetHp()const { return hp_; }

	// ���b�N�I������Ă��邩������擾
	const bool GetIsLockOn()const;

	// �U���̃q�b�g����
	virtual void AttackHit(const int damage, const int state);

	// ���̏�Ԃ��擾
	virtual const int GetState()const { return 0; }

	// �_���[�W�ʂ��擾
	virtual const int GetDamage()const { return 0; }

	// �X�s�[�h���擾
	virtual const float GetSpeed()const { return speed_; }

	// �������擾
	const VECTOR& GetDir()const { return dir_; }

	// ������ݒ�
	void SetDir(const VECTOR& dir) { dir_ = dir; }

	// ���ۂɓ����������擾
	const VECTOR& GetMoveDir()const { return moveDir_; }

	// �A�j���[�V�����R���g���[���[�ɏ�Ԃ𕶎��^�œn���ϐ����擾
	const std::string& GetKey()const { return key_; }

	// �A�j���[�V����
	const std::unique_ptr<AnimationController>& GetAnimationController()const { return animationController_; }

	// JSON�f�[�^
	const json& GetJsonData()const { return jsonData_; }

protected:

	// ���\�[�X�Ǘ�
	ResourceManager& resMng_;

	// �A�j���[�V����
	std::unique_ptr<AnimationController> animationController_;

	// �ړ��̃R���|�[�l���g
	std::shared_ptr<MoveComponent> moveComponent_;

	// ���f������̊�{���
	std::shared_ptr<Transform> transform_;

	// �Փ˔���̃f�[�^
	CollisionData collisionData_;

	// JSON�f�[�^
	json jsonData_;

	// �A�N�^�[�̎��
	ActorType actorType_;

	// ����
	VECTOR dir_;

	// ���ۂɓ�������
	VECTOR moveDir_;

	// �ړ���
	VECTOR movePow_;

	// ����̍��W
	VECTOR targetPos_;

	// ���̂̑��x
	VECTOR velocity_;

	// ���̂̉����x(�d��)
	VECTOR acceleration_;

	// �ړI�̊p�x
	Quaternion goalQuaRot_;

	// �A�j���[�V�����R���g���[���[�ɏ�Ԃ𕶎��^�œn���ϐ�
	std::string key_;

	// 1�O�̃A�j���[�V����
	std::string preKey_;

	// HP
	int hp_;

	// ���f��ID
	int modelId_;

	// �_���[�W��
	int damage_;

	// �X�s�[�h
	float speed_;

	// ���f���̑傫��
	float scl_;

	// ��]���鎞��
	float stepRotTime_;

	// �d�͂��ɂ�����
	float gravityScale_;

	// �����Ă��邩
	bool isAlive_;

	// �U�����������Ă��邩
	bool isAttackHit_;

	// �n�ʂɓ������Ă��邩
	bool isOnGround_;

	// ���b�N�I������Ă��邩(�G�̂�)
	bool isLockOn_;

	// �A�N�e�B�u��Ԃ��ǂ���
	bool isActive_;
	
	// �|�C���^�̎擾
	const std::shared_ptr<ActorBase>& GetThis() { return shared_from_this(); };
	
	// �@�\�̏�����
	virtual void InitFunction();

	// �p�����[�^�̏�����
	virtual void InitParameter();

	// �A�j���[�V�����̏�����
	virtual void InitAnimation();

	// �֐��|�C���^�̏�����
	virtual void InitFunctionPointer();

	// ImGui�̃f�o�b�O�`��̍X�V
	virtual void UpdateDebugImGui();

	// �Փ˔���̍X�V
	void CollisionUpdate();

	// �ړ�����
	virtual void Move();

	// �U������
	virtual void Attack(const float deltaTime);

	// �x����]
	void LazyRotation(float goalRot);

	// �Փ˔���Ŏg�����̂�o�^
	void CollisionRegister();

	// �A�j���[�V�����̃t���[���Œ�
	virtual void AnimationFrame();

	// �ǂ̃q�b�g�A�j���[�V�������`�F�b�N����
	virtual void AttackHitCheck(const int state);

	// �d��
	void Gravity(const float scale);

	// HP��0�ɂȂ����玀�S�A�j���[�V�����ɑJ��
	virtual void DeathAnim(int state);

private:

	// �f�o�b�O�`��
	void DrawDebug();

};