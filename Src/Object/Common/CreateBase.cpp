#include <random>
#include "CreateBase.h"

CreateBase::CreateBase()
{
}

void CreateBase::Update()
{
}

void CreateBase::Draw()
{
}

float CreateBase::GenerateRandNumber()
{

	// �񌈒�I�ȗ���������
	std::random_device rd;

	// �����Z���k�E�c�C�X�^�@�ɂ�闐��������
	std::mt19937 gen(rd());

	// �w��͈̔͂Ń����_���Ȑ����擾
	std::uniform_int_distribution<> dist_int(-ENEMY_SPAWN_AREA, ENEMY_SPAWN_AREA);
	float number = dist_int(gen);

    return number;

}
