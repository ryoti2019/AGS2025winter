#pragma once
#include <DxLib.h>
#include "../Common/Vector2F.h"
#include "../Object/Common/Transform.h"
#include "../Manager/ResourceManager.h"

enum class ActorType {
	NONE = -1,
	PLAYER,
	ENEMY,
	MAX
};

class ActorBase
{

public:

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

	// ���f������̊�{���
	Transform transform_;

	// �X�s�[�h
	float speed_;

	// HP
	int hp_;

	// �A�N�^�[�̎��
	ActorType actorType_;

	// �����Ă��邩�ǂ���
	bool isAlive_;

#pragma endregion

private:

	// �A�N�e�B�u��Ԃ��ǂ���
	bool isActive_;

};