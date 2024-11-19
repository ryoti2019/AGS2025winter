#pragma once
#include "../Object/ActorBase.h"

class CollisionManager
{

public:

	CollisionManager() = default;
	~CollisionManager() = default;

	void Init();
	void Update(const float deltaTime);

	// �����蔻������������̂�o�^����
	void Register(const std::shared_ptr<ActorBase>& actor);

private:

	// �Q��Actor�̓����蔻������邽�߂ɑg�ݍ��킹��o�^����^
	struct collisionChannnelInfo
	{
		ActorType type1;
		ActorType type2;
	};

	// �G�ƃv���C���[�̍U�����������Ă��邩�̑g�ݍ��킹��o�^����
	const std::vector<collisionChannnelInfo> attackCollisionChannelList_ =
	{
		{
			{ActorType::PLAYER,ActorType::ENEMY},
			{ActorType::ENEMY,ActorType::PLAYER}
		}
	};

	// �G�ƃv���C���[���X�e�[�W�ɓ������Ă��邩�̑g�ݍ��킹��o�^����
	const std::vector<collisionChannnelInfo> stageCollisionChannelList_ =
	{
		{
			{ActorType::PLAYER,ActorType::STAGE},
			{ActorType::ENEMY,ActorType::STAGE}
		}
	};

	// ���ׂẴA�N�^�[�̏Փ˔�����܂Ƃ߂��f�[�^
	std::unordered_map<ActorType, std::vector<std::shared_ptr<ActorBase>>> collisionActorData_;

	// �U�����������Ă��邩�ǂ����̃f�[�^
	std::map<std::shared_ptr<ActorBase>, std::map<int, float>> invincibleData_;

	// �v���C���[�ƓG�̍U���̓����蔻��
	void CheckAttackCollision(const float deltaTime);

	// �����������̏���
	void OnAttackCollision(const std::shared_ptr<ActorBase>& attacker, const std::shared_ptr<ActorBase>& target);

	// �v���C���[�ƓG�̃X�e�[�W�Ƃ̓����蔻��
	void CheckStageCollision(const float deltaTime);

};

