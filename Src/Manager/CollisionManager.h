#pragma once
#include "../Object/ActorBase.h"
#include "../Manager/Camera.h"
#include "CollisionTypes.h"

class CollisionManager
{

public:



	// �U���̍ŏ��̏�Ԃ̔ԍ�
	const int ATTACK_START_NUM;

	// �X�e�[�W�ƏՓ˂��Ă��邩���肷���
	const int STAGE_COLLISION_COUNT;

	// �G���m�̉���������
	const float ENEMY_PUSH_FORCE;

	// �X�e�[�W�Ƃ̓����蔻��ŉ����Ԃ���
	const float OBJECT_COLLISION_PUSH_FORCE;

	// �X�e�[�W�ƃJ�����̓����蔻��ŉ����Ԃ���
	const float CAMERA_COLLISION_PUSH_FORCE;

	// �قڐ^���ւ̌���
	const float DOWN_DIR;

	// �J�����̏Փ˔���Ŏg������
	const float LEN_CAMERACOLLISION_LINE;

	// �J�����̏Փ˔���ň�ԒZ������
	const float MIN_LEN_CAMERA_COLLISION_LINE;

	CollisionManager();
	~CollisionManager() = default;

	void Init();
	void Update(const float deltaTime);

	// �����蔻������������̂�o�^����
	void Register(const std::shared_ptr<ActorBase>& actor);

	// �J�����̏���ݒ�
	void SetCamera(const std::weak_ptr<Camera>& camera);

private:

	// �J����
	std::weak_ptr<Camera> camera_;

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
			{ActorType::PLAYER,ActorType::BOSS},
			{ActorType::ENEMY,ActorType::PLAYER},
			{ActorType::BOSS,ActorType::PLAYER}
		}
	};

	// �G�ƃv���C���[���X�e�[�W�ɓ������Ă��邩�̑g�ݍ��킹��o�^����
	const std::vector<collisionChannnelInfo> stageCollisionChannelList_ =
	{
		{
			{ActorType::PLAYER,ActorType::STAGE},
			{ActorType::ENEMY,ActorType::STAGE},
			{ActorType::BOSS,ActorType::STAGE}
		}
	};

	// �v���C���[�ƓG���m���������Ă��邩�̑g�ݍ��킹��o�^����
	const std::vector<collisionChannnelInfo> resolveCollisionChannelList_ =
	{
		{
			{ActorType::PLAYER,ActorType::ENEMY},
			{ActorType::PLAYER,ActorType::BOSS},
			{ActorType::ENEMY,ActorType::ENEMY},
			{ActorType::ENEMY,ActorType::BOSS}
		}
	};

	//�ǏՓˌ��m�p�̐����̒���
	float  collisionLineStageCamera_;

	// �ǂꂾ���d�Ȃ��Ă��邩
	VECTOR overlap_;

	// ���ׂẴA�N�^�[�̏Փ˔�����܂Ƃ߂��f�[�^
	std::unordered_map<ActorType, std::vector<std::shared_ptr<ActorBase>>> collisionActorData_;

	// �U�����������Ă��邩�ǂ����̃f�[�^
	std::map<std::shared_ptr<ActorBase>, std::map<int, float>> invincibleData_;

	// �v���C���[�ƓG�̍U���̓����蔻��
	void CheckAttackCollision(const float deltaTime);

	// �U���������������̏���
	void OnAttackCollision(const std::shared_ptr<ActorBase>& attacker, const std::shared_ptr<ActorBase>& target);

	// ��ѓ�������������̏���
	void OnProjectileCollision(const std::shared_ptr<ActorBase>& attacker, const std::shared_ptr<ActorBase>& target);

	// �X�e�[�W�Ƃ̓����蔻��
	void CheckStageCollision();

	// �A�N�^�[�ƃX�e�[�W�Ƃ̓����蔻��
	void CheckActorsAndStageCollision();

	// �J�����ƃX�e�[�W�Ƃ̓����蔻��
	void CheckCameraAndStageCollision();

	// �v���C���[��G���m���d�Ȃ��Ă����̓����蔻��
	void CheckResolveCollision();

	// �v���C���[�ƓG���d�Ȃ��Ă����̏���
	void ResolvePlayerEnemyCollision(const std::shared_ptr<ActorBase>& actor1, const std::shared_ptr<ActorBase>& actor2);

	// �G���m���d�Ȃ��Ă����̏���
	void ResolveEnemysCollision(const std::shared_ptr<ActorBase>& actor1, const std::shared_ptr<ActorBase>& actor2);

};

