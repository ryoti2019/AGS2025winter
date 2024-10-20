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

	// ���ߕ���
	bool ChargeWalk();

	// ����
	bool Run();

	// ���
	bool Roll();
	
	// �U��
	bool Attack();

	// ���ߍU��
	bool ChargeAttack();

private:

	// �A�j���[�V����
	AnimationController* animationController_;

	// �v���C���[
	Player* player_;

	// ����
	VECTOR dir_;

};

