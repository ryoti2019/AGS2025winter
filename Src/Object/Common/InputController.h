#pragma once
#include <DxLib.h>
#include "../Object/Common/Transform.h"

class AnimationController;
class Player;

class InputController
{
public:

	// �R���X�g���N�^
	InputController();

	// �f�X�g���N�^
	~InputController(void);

	// ����
	VECTOR Dir();

	// ���b�N�I�����̕���
	VECTOR LockOnDir(Transform transform);

	// ����
	bool Run();

	// �U��
	bool Attack();

	// �������U��
	bool ChargeAttack();

	// �A�b�p�[
	bool Upper();

	// �K�E�Z
	bool SpecialAttack();

	// ����
	bool Decide();

	// �L�����Z��
	bool Cancel();

	// �E�I��
	bool SelectRight();

	// ���I��
	bool SelectLeft();

private:

	// �A�j���[�V����
	AnimationController* animationController_;

	// ����
	VECTOR dir_;

};

