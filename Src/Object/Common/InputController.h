#pragma once
#include <DxLib.h>

class AnimationController;
class Player;

class InputContoroller
{
public:

	// �R���X�g���N�^
	InputContoroller(Player* player);

	// �f�X�g���N�^
	~InputContoroller(void);

	// ����
	VECTOR Dir();

	// �ړ�
	VECTOR Move();

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

