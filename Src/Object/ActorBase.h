#pragma once
#include <DxLib.h>
#include <memory>
#include <vector>
#include "../Common/Vector2F.h"
#include "../Object/Common/Transform.h"
#include "../Manager/ResourceManager.h"
#include "../Object/Common/AnimationController.h"

class Camera;

enum class ActorType {
	NONE = -1,
	PLAYER,
	ENEMY,
	MAX
};

class ActorBase
{

public:

	// ��]�����܂ł̎���
	const float ROTATION_POW;

	ActorBase(const VECTOR& pos);

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

	// ����
	VECTOR dir_;

	// �ړI�̊p�x
	Quaternion goalQuaRot_;

	// �A�j���[�V�����R���g���[���[�ɏ�Ԃ𕶎��^�œn���ϐ�
	std::string key_;

	// 1�O�̃A�j���[�V����
	std::string preKey_;

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

	// �ړ�����
	virtual void Move() = 0;

	// �U������
	virtual void Attack(const float deltaTime) = 0;

	// �x����]
	void LazyRotation(float goalRot);

#pragma endregion

private:

	// �A�N�e�B�u��Ԃ��ǂ���
	bool isActive_;

};