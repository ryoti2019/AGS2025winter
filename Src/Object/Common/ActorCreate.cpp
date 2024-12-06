#include <fstream>
#include <memory>
#include "../Lib/nlohmann/json.hpp"
#include "../Application.h"
#include "../Component/InputComponent.h"
#include "../Object/Common/Transform.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ActorManager.h"
#include "../Scene/GameScene.h"
#include "../Object/Player.h"
#include "../Object/Enemy.h"
#include "../Object/Stage.h"
#include "ActorCreate.h"

ActorCreate::ActorCreate()
	:
	isCollisionArea1_(false),
	isCollisionArea2_(false)
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
	const auto& stageData = objectData[2]["StageData"];

	// �X�e�[�W�𐶐�
	actorManager->CreateActor<Stage>(stageData, { 0.0f,0.0f,0.0f });
	actorManager->ActiveData(ActorType::STAGE, { stageData["POS"]["x"], stageData["POS"]["y"] ,stageData["POS"]["z"] });

	// �v���C���[
	const auto& playerData = objectData[0]["PlayerData"];

	// �v���C���[�𐶐�
	actorManager->CreateActor<Player>(playerData, { -80000.0f,-19500.0f,25900.0f });
	actorManager->ActiveData(ActorType::PLAYER, { -80000.0f,-19500.0f,25900.0f });

	// �G
	const auto& enemyData = objectData[1]["EnemyData"];

	// �G�𐶐�
	for (int i = 0; i < 50; i++)
	{
		float x = std::rand() % 10;
		float z = std::rand() % 10;
		actorManager->CreateActor<Enemy>(enemyData, { 0.0f,-1500.0f,0.0f });
	}

}

void ActorCreate::Update()
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

	// �G
	const auto& enemyData = objectData[1]["EnemyData"];

	// �v���C���[
	const auto& players = actorManager->GetActiveActorData().find(ActorType::PLAYER);

	// �v���C���[�̒��g�������Ă��邩�m�F
	if (players == actorManager->GetActiveActorData().end()) return;

	for (auto& player : players->second)
	{

		if (!isCollisionArea1_ && HitCheck_Sphere_Sphere(player->GetTransform()->pos, player->GetCollisionData().bodyCollisionRadius, { 9300.0f,-18000.0f,23600.0f }, 10000.0f))
		{

			// �G���A1�ƏՓ˂���
			isCollisionArea1_ = true;

			// �G�𐶐�
			for (int i = 0; i < 10; i++)
			{
				float x = std::rand() % 10000;
				float z = std::rand() % 10000;
				actorManager->ActiveData(ActorType::ENEMY, { 9300.0f + x,-15000,23600.0f + z });
			}

		}

		if (!isCollisionArea2_ && HitCheck_Sphere_Sphere(player->GetTransform()->pos, player->GetCollisionData().bodyCollisionRadius, { -3100.0f,-18000.0f,-22000.0f }, 10000.0f))
		{

			// �G���A1�ƏՓ˂���
			isCollisionArea2_ = true;

			// �G�𐶐�
			for (int i = 0; i < 10; i++)
			{
				float x = std::rand() % 10000;
				float z = std::rand() % 10000;
				actorManager->ActiveData(ActorType::ENEMY, { -27000.0f + x,-15000,-22000.0f + z });
			}

		}

	}

}

void ActorCreate::Draw()
{
	DrawSphere3D({ 9300.0f,-18000.0f,23600.0f }, 10000.0f, 10, 0xff0000, 0xff0000, false);
	DrawSphere3D({ -3100.0f,-18000.0f,-22000.0f }, 10000.0f, 10, 0xff0000, 0xff0000, false);
}
