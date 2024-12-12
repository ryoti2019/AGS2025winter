#pragma once
class ActorCreate
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

	// �G���A5�̂��ׂĂ̓G�̐�
	const int AREA5_TOTAL_ENEMYS;

	ActorCreate();

	~ActorCreate() = default;

	void Update();

	void Draw();

private:

	// �G���A1�ƏՓ˂��Ă��邩
	bool isCollisionArea1_;

	// �G���A2�ƏՓ˂��Ă��邩
	bool isCollisionArea2_;

	// ���񂾓G�̐����J�E���g
	int deathEnemyCnt_;

	// �G���A�̏Փ˔���̏���
	void AreaCollision();

	// �G���A�̏Փ˔�����Ȃ�������
	void DeactiveAreaCollision();

	// �G�����̓|�������J�E���g���鏈��
	void CountDeathEnemys();

};

