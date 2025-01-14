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

	// �L�[�{�[�h���R���g���[���[�ǂ��炩�����߂�
	bool SelectInputDevice();

	// ����
	bool Decide();

	// �L�����Z��
	bool Cancel();

private:

	// �A�j���[�V����
	AnimationController* animationController_;

	// ����
	VECTOR dir_;

};

