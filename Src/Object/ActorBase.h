#pragma once
#include <DxLib.h>
#include <memory>
#include <vector>
#include "../Lib/nlohmann/json.hpp"
#include "../Common/Vector2F.h"
#include "../Object/Common/Transform.h"
#include "../Manager/ResourceManager.h"
#include "../Object/Common/AnimationController.h"

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

		// �E��̍��W
		VECTOR rightHandPos;

		// ����̍��W
		VECTOR leftHandPos;

		// �E���̍��W
		VECTOR rightFootPos;

		// �����̍��W
		VECTOR leftFootPos;

		// �̑S�̂̍��W
		VECTOR bodyPos;

		// �E��̃J�v�Z���̏���W
		VECTOR rightHandCapsuleUpPos;

		// �E��̃J�v�Z���̉����W
		VECTOR rightHandCapsuleDownPos;

		// ����̃J�v�Z���̏���W
		VECTOR leftHandCapsuleUpPos;

		// ����̃J�v�Z���̉����W
		VECTOR leftHandCapsuleDownPos;

		// �E���̃J�v�Z���̏���W
		VECTOR rightFootCapsuleUpPos;

		// �E���̃J�v�Z���̉����W
		VECTOR rightFootCapsuleDownPos;

		// �����̃J�v�Z���̏���W
		VECTOR leftFootCapsuleUpPos;

		// �����̃J�v�Z���̉����W
		VECTOR leftFootCapsuleDownPos;

		// �̂̃J�v�Z���̏���W
		VECTOR bodyCapsuleUpPos;

		// �̂̃J�v�Z���̉����W
		VECTOR bodyCapsuleDownPos;

		// �E��̌���
		Quaternion rightHandRot;

		// ����̌���
		Quaternion leftHandRot;

		// �E���̌���
		Quaternion rightFootRot;

		// �����̌���
		Quaternion leftFootRot;

		// �̂̌���
		Quaternion bodyRot;

		// �葫�̓����蔻��̔��a
		float handAndFootCollisionRadius;

		// �̂̓����蔻��̔��a
		float bodyCollisionRadius;

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

	// �U������Ƃ��ɐi�ވړ���
	const float ATTACK_MOVE_POW;

	// ��]��
	const float ROTATION_POW;

	// �葫�̓����蔻��̔��a
	const float HAND_AND_FOOT_COLLISION_RADIUS;

	// �̂̓����蔻��̔��a
	const float BODY_COLLISION_RADIUS;

	ActorBase(const VECTOR& pos, const json& data);

	virtual ~ActorBase() = default;

	virtual void Init(const VECTOR& pos);
	void Create(const VECTOR& pos);
	virtual void Update(const float deltaTime);
	void Draw();

	// ���W��ݒ�
	void SetPos(const VECTOR& pos) { transform_.pos = pos; }

	// ���������ݒ�
	void SetIsActive(const bool isActive) { isActive_ = isActive; }

	// HP��ݒ�
	void SetHp(const int hp) { hp_ = hp; }

	// HP�����炷
	void SubHp(const int hp) { hp_ -= hp;}

	// ���b�N�I������Ă��邩�����ݒ�
	void SetIsLockOn(const bool isLockOn);

	// ���f���̊�{�����擾
	const Transform& GetTransform() const { return transform_; }

	// �A�N�^�[�^�C�v���擾
	const ActorType& GetActorType() const { return actorType_; }

	// �R���W�����f�[�^�̂��擾
	const CollisionData& GetCollisionData() const { return collisionData_; }

	// ���W���擾
	const VECTOR& GetPos() const { return transform_.pos; }

	// ����������擾
	bool GetIsActive() const { return isActive_; }

	// �U����Ԃ��擾
	virtual bool GetAttackState();

	// HP���擾
	const int GetHp()const { return hp_; }

	// ���b�N�I������Ă��邩������擾
	bool GetIsLockOn();

protected:

	// ���\�[�X�Ǘ�
	ResourceManager& resMng_;

#pragma region �I�u�W�F�N�g�̏��

	// �A�j���[�V����
	std::unique_ptr<AnimationController> animationController_;

	// ���f������̊�{���
	Transform transform_;

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

	// �X�s�[�h
	float speed_;

	// ���f���̑傫��
	float scl_;

	// ��]���鎞��
	float stepRotTime_;

	// �����Ă��邩
	bool isAlive_;

	// ���b�N�I������Ă��邩(�G�̂�)
	bool isLockOn_;
	
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
	virtual void ComboAttack(const float deltaTime);

	// �U�����
	virtual bool AttackState();

	// �x����]
	void LazyRotation(float goalRot);

	// �Փ˔���Ŏg�����̂�o�^
	void CollisionRegister();

#pragma endregion

private:

	// �A�N�e�B�u��Ԃ��ǂ���
	bool isActive_;

	// �f�o�b�O�`��
	void DrawDebug();

};