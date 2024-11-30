#pragma once
#include <DxLib.h>
#include "../Component/TransformComponent.h"

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
	VECTOR LockOnDir(TransformComponent transform);

	// ����
	bool Run();

	// �U��
	bool Attack();

	// �������U��
	bool ChargeAttack();

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

