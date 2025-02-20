#pragma once
#include "CreateBase.h"

class BossBattleActorCreate : public CreateBase
{

public:

	BossBattleActorCreate();

	~BossBattleActorCreate()override = default;

	void Update()override;

	void Draw()override;

private:

	// �v���C���[�̏������W
	static constexpr VECTOR PLAYER_INIT_POS = { -10800.0f,-19500.0f,-120000.0f };

	// �G�𐶐����鐔
	static constexpr int CREATE_ENEMYS = 50;

	// �{�X�̏������W
	static constexpr VECTOR BOSS_INIT_POS = { -10800.0f,-19500.0f,-140000.0f };

	// ��x�ɐ�������G�̐�
	static constexpr int ENEMY_CREATE_NUM = 5;

	// �G���o��������W
	static constexpr VECTOR ENEMY_POS = { -9000.0f, 1000.0f, -140000.0f };

};

