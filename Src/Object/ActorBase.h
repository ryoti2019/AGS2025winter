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
	MAX
};

class ActorBase
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

		// �E��̍��W
		VECTOR rightHandPos;

		// ����̍��W
		VECTOR leftHandPos;

		// �E���̍��W
		VECTOR rightFootPos;

		// �����̍��W
		VECTOR leftFootPos;

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

		// �E��̌���
		Quaternion rightHandRot;

		// ����̌���
		Quaternion leftHandRot;

		// �E���̌���
		Quaternion rightFootRot;

		// �����̌���
		Quaternion leftFootRot;

		// �����蔻��̔��a
		float collisionRadius;

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

	// �E��̃t���[����
	const std::string RIGHT_HAND_FRAME;

	// ����̃t���[����
	const std::string LEFT_HAND_FRAME;

	// �E���̃t���[����
	const std::string RIGHT_FOOT_FRAME;

	// �����̃t���[����
	const std::string LEFT_FOOT_FRAME;

	// �U������Ƃ��ɐi�ވړ���
	const float ATTACK_MOVE_POW;

	// ��]��
	const float ROTATION_POW;

	// �����蔻��̔��a
	const float COLLISION_RADIUS;

	ActorBase(const VECTOR& pos, const json& data);

	virtual ~ActorBase() = default;

	virtual void Init(const VECTOR& pos);
	virtual void Create(const VECTOR& pos);
	virtual void Update(const float deltaTime);
	virtual void Draw();

	void SetPos(const VECTOR& pos) { transform_.pos = pos; };

	const bool GetIsActive() const { return isActive_; }

	void SetIsActive(const bool isActive) { isActive_ = isActive; }

	const ActorType GetActorType() const { return actorType_; }

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

	// ����
	VECTOR dir_;

	// �ړI�̊p�x
	Quaternion goalQuaRot_;

	// �A�j���[�V�����R���g���[���[�ɏ�Ԃ𕶎��^�œn���ϐ�
	std::string key_;

	// 1�O�̃A�j���[�V����
	std::string preKey_;

	// JSON�f�[�^
	json jsonData_;

	// �X�s�[�h
	float speed_;

	// HP
	int hp_;

	// ���f��ID
	int modelId_;

	// ���f���̑傫��
	float scl_;

	// �A�N�^�[�̎��
	ActorType actorType_;

	// �����Ă��邩�ǂ���
	bool isAlive_;

	// ��]���鎞��
	float stepRotTime_;

	// �@�\�̏�����
	virtual void InitFunction() = 0;

	// �p�����[�^�̏�����
	virtual void InitParameter() = 0;

	// �A�j���[�V�����̏�����
	virtual void InitAnimation() = 0;

	// �֐��|�C���^�̏�����
	virtual void InitFunctionPointer() = 0;

	// �Փ˔���̍X�V
	void CollisionUpdate();

	// �ړ�����
	virtual void Move() = 0;

	// �U������
	virtual void ComboAttack(const float deltaTime) = 0;

	// �U�����
	virtual bool AttackState() = 0;

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