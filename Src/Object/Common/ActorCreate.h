#pragma once
#include "CreateBase.h"

class ActorCreate : public CreateBase
{

public:

	// �G���A1�̂��ׂĂ̓G�̐�
	const int AREA1_TOTAL_ENEMYS;

	// �G���A2�̂��ׂĂ̓G�̐�
	const int AREA2_TOTAL_ENEMYS;

	// �G���A3�̂��ׂĂ̓G�̐�
	const int AREA3_TOTAL_ENEMYS;

	// �G���A4�̂��ׂĂ̓G�̐�
	const int AREA4_TOTAL_ENEMYS;

	ActorCreate();

	~ActorCreate()override = default;

	void Update()override;

	void Draw()override;

private:

	// �G���A1�ƏՓ˂��Ă��邩
	bool isCollisionArea1_;

	// �G���A2�ƏՓ˂��Ă��邩
	bool isCollisionArea2_;

	// �G���A3�ƏՓ˂��Ă��邩
	bool isCollisionArea3_;

	// �G���A4�ƏՓ˂��Ă��邩
	bool isCollisionArea4_;

	// �G���A5�ƏՓ˂��Ă��邩
	bool isCollisionArea5_;

	// ���񂾓G�̐����J�E���g
	int deathEnemyCnt_;

	// �G���A�̏Փ˔���̏���
	void AreaCollision();

	// �G���A�̏Փ˔�����Ȃ�������
	void DeactiveAreaCollision();

	// �G�����̓|�������J�E���g���鏈��
	void CountDeathEnemys();

};

