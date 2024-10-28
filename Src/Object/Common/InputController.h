#pragma once
#include <DxLib.h>

class AnimationController;
class Player;

class InputController
{
public:

	// �R���X�g���N�^
	InputController(Player* player);

	// �f�X�g���N�^
	~InputController(void);

	// ����
	VECTOR Dir();

	// ���b�N�I�����̕���
	VECTOR LockOnDir(Transform transform);

	// ����
	bool Run();

	// �U��
	bool ComboAttack();

	// �A�b�p�[
	bool Upper();

private:

	// �A�j���[�V����
	AnimationController* animationController_;

	// �v���C���[
	Player* player_;

	// ����
	VECTOR dir_;

};

