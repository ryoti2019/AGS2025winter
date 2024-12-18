#include <fstream>
#include "../Lib/nlohmann/json.hpp"
#include "../Application.h"
#include "../Component/InputComponent.h"
#include "../Object/Common/Transform.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ActorManager.h"
#include "../Scene/GameScene.h"
#include "../Object/Player.h"
#include "../Object/Enemy.h"
#include "../Object/Boss.h"
#include "../Object/Stage.h"
#include "../Object/StageCollision.h"
#include "../Object/Area1Collision.h"
#include "../Object/Area2Collision.h"
#include "../Object/Area3Collision.h"
#include "../Object/Area4Collision.h"
#include "../Object/Area5Collision.h"
#include "ActorCreate.h"

ActorCreate::ActorCreate()
	:
	AREA1_TOTAL_ENEMYS(10),
	AREA2_TOTAL_ENEMYS(10),
	AREA3_TOTAL_ENEMYS(10),
	AREA4_TOTAL_ENEMYS(10),
	AREA5_TOTAL_ENEMYS(10),
	isCollisionArea1_(false),
	isCollisionArea2_(false),
	isCollisionArea3_(false),
	isCollisionArea4_(false),
	isCollisionArea5_(false),
	deathEnemyCnt_(0)
{

	// �O���t�@�C���̓ǂݍ���
	std::ifstream ifs;

	ifs.open(Application::PATH_JSON + "ObjectData.json");

	if (!ifs)
	{
		// �O���t�@�C���̓ǂݍ��ݎ��s
		return;
	}

	// �t�@�C���X�g���[������json�I�u�W�F�N�g�ɕϊ�
	nlohmann::json objectJson = nlohmann::json::parse(ifs);

	const auto& objectData = objectJson["ObjectData"];

	// �X�R�[�v���؂��ۂ� �����I�Ƀt�@�C���N���[�Y���ĖႦ�܂����A
	// ���s�V�ǂ��A�����I�Ƀt�@�C���X�g���[�������
	ifs.close();

	// ���N���X����g�������^�փL���X�g����
	std::shared_ptr<GameScene> gameScene =
		std::dynamic_pointer_cast<GameScene>(SceneManager::GetInstance().GetNowScene());

	// NULL�`�F�b�N
	if (!gameScene) return;

	// �A�N�^�[�}�l�[�W���[���擾
	std::shared_ptr<ActorManager> actorManager = gameScene->GetActorManager();

	// �X�e�[�W
	const auto& stageData = objectData[3]["StageData"];

	// �X�e�[�W�𐶐�
	actorManager->CreateActor<Stage>(stageData, { 0.0f,0.0f,0.0f });
	actorManager->ActiveData(ActorType::STAGE, { stageData["POS"]["x"], stageData["POS"]["y"] ,stageData["POS"]["z"] });

	// �X�e�[�W�̓����蔻��𐶐�
	actorManager->CreateActor<StageCollision>(stageData, { 0.0f,0.0f,0.0f });
	actorManager->ActiveData(ActorType::STAGE, { stageData["POS"]["x"], stageData["POS"]["y"] ,stageData["POS"]["z"] });

	// �X�e�[�W�̓����蔻��𐶐�
	actorManager->CreateActor<Area1Collision>(stageData, { 0.0f,0.0f,0.0f });
	actorManager->ActiveData(ActorType::STAGE, { stageData["POS"]["x"], stageData["POS"]["y"] ,stageData["POS"]["z"] });

	// �X�e�[�W�̓����蔻��𐶐�
	actorManager->CreateActor<Area2Collision>(stageData, { 0.0f,0.0f,0.0f });
	actorManager->ActiveData(ActorType::STAGE, { stageData["POS"]["x"], stageData["POS"]["y"] ,stageData["POS"]["z"] });

	// �X�e�[�W�̓����蔻��𐶐�
	actorManager->CreateActor<Area3Collision>(stageData, { 0.0f,0.0f,0.0f });
	actorManager->ActiveData(ActorType::STAGE, { stageData["POS"]["x"], stageData["POS"]["y"] ,stageData["POS"]["z"] });

	// �X�e�[�W�̓����蔻��𐶐�
	actorManager->CreateActor<Area4Collision>(stageData, { 0.0f,0.0f,0.0f });
	actorManager->ActiveData(ActorType::STAGE, { stageData["POS"]["x"], stageData["POS"]["y"] ,stageData["POS"]["z"] });

	// �X�e�[�W�̓����蔻��𐶐�
	actorManager->CreateActor<Area5Collision>(stageData, { 0.0f,0.0f,0.0f });
	actorManager->ActiveData(ActorType::STAGE, { stageData["POS"]["x"], stageData["POS"]["y"] ,stageData["POS"]["z"] });

	// �v���C���[
	const auto& playerData = objectData[0]["PlayerData"];

	// �v���C���[�𐶐�
	actorManager->CreateActor<Player>(playerData, { -80000.0f,-19500.0f,25900.0f });
	actorManager->ActiveData(ActorType::PLAYER, { -80000.0f,-19500.0f,25900.0f });

	//actorManager->CreateActor<Player>(playerData, { -10800.0f,-19500.0f,-132000.0f });
	//actorManager->ActiveData(ActorType::PLAYER, { -10800.0f,-19500.0f,-132000.0f });

	// �G
	const auto& enemyData = objectData[1]["EnemyData"];

	// �G�𐶐�
	for (int i = 0; i < 50; i++)
	{
		float x = std::rand() % 10;
		float z = std::rand() % 10;
		actorManager->CreateActor<Enemy>(enemyData, { 0.0f,0.0f,0.0f });
	}

	// �{�X
	const auto& bossData = objectData[2]["BossData"];

	// �{�X�𐶐�
	actorManager->CreateActor<Boss>(bossData, { -10800.0f,-10000.0f,-150000.0f });
	actorManager->ActiveData(ActorType::BOSS, { -10800.0f,-10000.0f,-150000.0f });

}

void ActorCreate::Update()
{

	// �G���A�̏Փ˔���̏���
	AreaCollision();

	// �G���A�̏Փ˔�����Ȃ�������
	DeactiveAreaCollision();

	// �G�����̓|�������J�E���g���鏈��
	CountDeathEnemys();

}

void ActorCreate::Draw()
{
	DrawSphere3D({ 9300.0f,-18000.0f,23600.0f }, 10000.0f, 10, 0xff0000, 0xff0000, false);
	DrawSphere3D({ -3100.0f,-18000.0f,-22000.0f }, 10000.0f, 10, 0xff0000, 0xff0000, false);
}

void ActorCreate::AreaCollision()
{

	// ���N���X����g�������^�փL���X�g����
	std::shared_ptr<GameScene> gameScene =
		std::dynamic_pointer_cast<GameScene>(SceneManager::GetInstance().GetNowScene());

	// NULL�`�F�b�N
	if (!gameScene) return;

	// �A�N�^�[�}�l�[�W���[���擾
	std::shared_ptr<ActorManager> actorManager = gameScene->GetActorManager();

	// �v���C���[
	const auto& players = actorManager->GetActiveActorData().find(ActorType::PLAYER);

	// �X�e�[�W
	const auto& stages = actorManager->GetActiveActorData().find(ActorType::STAGE);

	// �v���C���[�̒��g�������Ă��邩�m�F
	if (players == actorManager->GetActiveActorData().end()) return;

	// �X�e�[�W�̒��g�������Ă��邩�m�F
	if (stages == actorManager->GetActiveActorData().end()) return;

	for (auto& player : players->second)
	{
		for (auto& stage : stages->second)
		{

			// Area1Collision�ɃL���X�g
			auto area1Collision = std::dynamic_pointer_cast<Area1Collision>(stage);

			// �G���A1�ɏՓ˂��Ă��邩����
			if (area1Collision && area1Collision->GetIsCollision() && !isCollisionArea1_)
			{

				// �G���A1�ƏՓ˂���
				isCollisionArea1_ = true;

				// �G�𐶐�
				for (int i = 0; i < AREA1_TOTAL_ENEMYS; i++)
				{
					float x = std::rand() % 10000;
					float z = std::rand() % 10000;
					actorManager->ActiveData(ActorType::ENEMY, { 9300.0f + x,-15000,23600.0f + z });
				}

			}

			// Area2Collision�ɃL���X�g
			auto area2Collision = std::dynamic_pointer_cast<Area2Collision>(stage);

			// �G���A2�ɏՓ˂��Ă��邩����
			if (area2Collision && area2Collision->GetIsCollision() && !isCollisionArea2_)
			{

				// �G���A2�ƏՓ˂���
				isCollisionArea2_ = true;

				// �G�𐶐�
				for (int i = 0; i < AREA2_TOTAL_ENEMYS; i++)
				{
					float x = std::rand() % 100;
					float z = std::rand() % 100;
					actorManager->ActiveData(ActorType::ENEMY, { -3100.0f + x,-18000.0f,-22000.0f + z });
				}

			}

			// Area3Collision�ɃL���X�g
			auto area3Collision = std::dynamic_pointer_cast<Area3Collision>(stage);

			// �G���A3�ɏՓ˂��Ă��邩����
			if (area3Collision && area3Collision->GetIsCollision() && !isCollisionArea3_)
			{

				// �G���A3�ƏՓ˂���
				isCollisionArea3_ = true;

				// �G���A3�̓����蔻���t����
				area3Collision->SetIsCollision(true);

				// �G�𐶐�
				for (int i = 0; i < AREA3_TOTAL_ENEMYS; i++)
				{
					float x = std::rand() % 100;
					float z = std::rand() % 100;
					actorManager->ActiveData(ActorType::ENEMY, { -81000.0f + x,-19500.0f,-34000.0f + z });
				}

			}

			// Area4Collision�ɃL���X�g
			auto area4Collision = std::dynamic_pointer_cast<Area4Collision>(stage);

			// �G���A4�ɏՓ˂��Ă��邩����
			if (area4Collision && area4Collision->GetIsCollision() && !isCollisionArea4_)
			{

				// �G���A4�ƏՓ˂���
				isCollisionArea4_ = true;

				// �G�𐶐�
				for (int i = 0; i < AREA4_TOTAL_ENEMYS; i++)
				{
					float x = std::rand() % 1000;
					float z = std::rand() % 1000;
					actorManager->ActiveData(ActorType::ENEMY, { -60000.0f + x,-18000.0f,-68000.0f + z });
				}

			}

		}

	}

}

void ActorCreate::DeactiveAreaCollision()
{

	// ���N���X����g�������^�փL���X�g����
	std::shared_ptr<GameScene> gameScene =
		std::dynamic_pointer_cast<GameScene>(SceneManager::GetInstance().GetNowScene());

	// NULL�`�F�b�N
	if (!gameScene) return;

	// �A�N�^�[�}�l�[�W���[���擾
	std::shared_ptr<ActorManager> actorManager = gameScene->GetActorManager();

	// �R���W�����}�l�[�W���[���擾
	std::shared_ptr<CollisionManager> collisionManager = gameScene->GetCollisionManager();

	// �X�e�[�W�����邩�`�F�b�N
	if (!actorManager->GetActiveActorData().contains(ActorType::STAGE)) return;

	// �X�e�[�W�����o��
	auto& stages = actorManager->GetActiveActorData().at(ActorType::STAGE);

	for (auto stage : stages)
	{

#pragma region Area1Collision

		// Area1Collision�ɃL���X�g
		std::shared_ptr<Area1Collision> area1Collision = std::dynamic_pointer_cast<Area1Collision>(stage);

		// �G���A1�̓G�����ׂē|������Փ˔��������
		if (area1Collision && deathEnemyCnt_ == AREA1_TOTAL_ENEMYS && area1Collision->GetIsCollision())
		{

			// �Փ˔��������
			area1Collision->SetIsCollision(false);

			// ���X�ɏ����Ă����悤�ɂ���
			area1Collision->SetIsDissolve(true);

			// ���N���X�ɃL���X�g
			std::shared_ptr<StageBase> stageBase = std::dynamic_pointer_cast<StageBase>(stage);

			//	collisionManager�̗v�f�������
			collisionManager->StageErasure(stageBase);

			// �G�̓|�����J�E���^�����Z�b�g
			deathEnemyCnt_ = 0;

		}

		// ���S�ɓ��߂������A�N�e�B�u�ɂ���
		if (area1Collision && area1Collision->GetAlphaTime() >= 1.0f)
		{
			area1Collision->SetIsActive(false);
		}

#pragma endregion

#pragma region Area2Collision

		// Area2Collision�ɃL���X�g
		std::shared_ptr<Area2Collision> area2Collision = std::dynamic_pointer_cast<Area2Collision>(stage);

		// �G���A1�̓G�����ׂē|������Փ˔��������
		if (area2Collision && deathEnemyCnt_ == AREA2_TOTAL_ENEMYS && area2Collision->GetIsCollision())
		{

			// �Փ˔��������
			area2Collision->SetIsCollision(false);

			// ���X�ɏ����Ă����悤�ɂ���
			area2Collision->SetIsDissolve(true);

			// ���N���X�ɃL���X�g
			std::shared_ptr<StageBase> stageBase = std::dynamic_pointer_cast<StageBase>(stage);

			//	collisionManager�̗v�f�������
			collisionManager->StageErasure(stageBase);

			// �G�̓|�����J�E���^�����Z�b�g
			deathEnemyCnt_ = 0;

		}

		// ���S�ɓ��߂������A�N�e�B�u�ɂ���
		if (area2Collision && area2Collision->GetAlphaTime() >= 1.0f)
		{
			area2Collision->SetIsActive(false);
		}

#pragma endregion

#pragma region Area3Collision

		// Area3Collision�ɃL���X�g
		std::shared_ptr<Area3Collision> area3Collision = std::dynamic_pointer_cast<Area3Collision>(stage);

		// �G���A1�̓G�����ׂē|������Փ˔��������
		if (area3Collision && deathEnemyCnt_ == AREA3_TOTAL_ENEMYS && area3Collision->GetIsCollision())
		{

			// �Փ˔��������
			area3Collision->SetIsCollision(false);

			// ���X�ɏ����Ă����悤�ɂ���
			area3Collision->SetIsDissolve(true);

			// ���N���X�ɃL���X�g
			std::shared_ptr<StageBase> stageBase = std::dynamic_pointer_cast<StageBase>(stage);

			//	collisionManager�̗v�f�������
			collisionManager->StageErasure(stageBase);

			// �G�̓|�����J�E���^�����Z�b�g
			deathEnemyCnt_ = 0;

		}

		// ���S�ɓ��߂������A�N�e�B�u�ɂ���
		if (area3Collision && area3Collision->GetAlphaTime() >= 1.0f)
		{
			area3Collision->SetIsActive(false);
		}

#pragma endregion

#pragma region Area4Collision

		// Area4Collision�ɃL���X�g
		std::shared_ptr<Area4Collision> area4Collision = std::dynamic_pointer_cast<Area4Collision>(stage);

		// �G���A1�̓G�����ׂē|������Փ˔��������
		if (area4Collision && deathEnemyCnt_ == AREA4_TOTAL_ENEMYS && area4Collision->GetIsCollision())
		{

			// �Փ˔��������
			area4Collision->SetIsCollision(false);

			// ���X�ɏ����Ă����悤�ɂ���
			area4Collision->SetIsDissolve(true);

			// ���N���X�ɃL���X�g
			std::shared_ptr<StageBase> stageBase = std::dynamic_pointer_cast<StageBase>(stage);

			//	collisionManager�̗v�f�������
			collisionManager->StageErasure(stageBase);

			// �G�̓|�����J�E���^�����Z�b�g
			deathEnemyCnt_ = 0;

		}

		// ���S�ɓ��߂������A�N�e�B�u�ɂ���
		if (area4Collision && area4Collision->GetAlphaTime() >= 1.0f)
		{
			area4Collision->SetIsActive(false);
		}

#pragma endregion

	}

}

void ActorCreate::CountDeathEnemys()
{

	// ���N���X����g�������^�փL���X�g����
	std::shared_ptr<GameScene> gameScene =
		std::dynamic_pointer_cast<GameScene>(SceneManager::GetInstance().GetNowScene());

	// NULL�`�F�b�N
	if (!gameScene) return;

	// �A�N�^�[�}�l�[�W���[���擾
	std::shared_ptr<ActorManager> actorManager = gameScene->GetActorManager();

	// �G
	const auto& enemys = actorManager->GetActiveActorData().find(ActorType::ENEMY);

	// �v���C���[�̒��g�������Ă��邩�m�F
	if (enemys == actorManager->GetActiveActorData().end()) return;

	for (auto& enemy : enemys->second)
	{

		// �X�e�[�W�ɏ���ύX
		auto enemyData = std::static_pointer_cast<Enemy>(enemy->GetThis());

		// ���񂾂�J�E���^�𑝂₷
		if (enemyData->GetHp() <= 0 && enemyData->GetIsAlive())
		{
			deathEnemyCnt_++;
			enemyData->SetIsAlive(false);
		}

	}

}