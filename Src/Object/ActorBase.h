#pragma once
#include <DxLib.h>
#include "../Common/Vector2F.h"

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

	void SetPos(const VECTOR& pos) { pos_ = pos; };

	void SetIsActive(const bool isActive) { isActive_ = isActive; }

	const ActorType GetActorType() const { return actorType_; }

protected:

#pragma region �I�u�W�F�N�g�̏��

	// �ړ�����W
	VECTOR movedPos_;

	// �X�s�[�h
	float speed_;

	// �ړ���
	float movePow_;

	// HP
	int hp_;

	// �Փ˂��Ă��邩
	bool isHit_;

	// �A�N�^�[�̎��
	ActorType actorType_;

	// �����Ă��邩�ǂ���
	bool isAlive_;

#pragma endregion

private:

	// ���W
	VECTOR pos_;

	// �A�N�e�B�u��Ԃ��ǂ���
	bool isActive_;

};