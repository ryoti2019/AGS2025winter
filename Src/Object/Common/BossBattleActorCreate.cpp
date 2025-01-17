#include <fstream>
#include "../Lib/nlohmann/json.hpp"
#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ActorManager.h"
#include "../Scene/BossBattleScene.h"
#include "BossBattleActorCreate.h"
#include "../Object/Player.h"
#include "../Object/Enemy.h"
#include "../Object/Boss.h"
#include "../Object/Stage.h"
#include "../Object/StageCollision.h"
#include "../Object/Area5Collision.h"

BossBattleActorCreate::BossBattleActorCreate()
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
	std::shared_ptr<BossBattleScene> bossBattleScene =
		std::dynamic_pointer_cast<BossBattleScene>(SceneManager::GetInstance().GetNowScene());

	// NULL�`�F�b�N
	if (!bossBattleScene) return;

	// �A�N�^�[�}�l�[�W���[���擾
	std::shared_ptr<ActorManager> actorManager = bossBattleScene->GetActorManager();

	// �X�e�[�W
	const auto& stageData = objectData[3]["StageData"];

	// �X�e�[�W�𐶐�
	actorManager->CreateActor<Stage>(stageData, { 0.0f,0.0f,0.0f });
	actorManager->ActiveData(ActorType::STAGE, { stageData["POS"]["x"], stageData["POS"]["y"] ,stageData["POS"]["z"] });

	// �X�e�[�W�̓����蔻��𐶐�
	actorManager->CreateActor<StageCollision>(stageData, { 0.0f,0.0f,0.0f });
	actorManager->ActiveData(ActorType::STAGE, { stageData["POS"]["x"], stageData["POS"]["y"] ,stageData["POS"]["z"] });

	// �X�e�[�W�̓����蔻��𐶐�
	actorManager->CreateActor<Area5Collision>(stageData, { 0.0f,0.0f,0.0f });
	actorManager->ActiveData(ActorType::STAGE, { stageData["POS"]["x"], stageData["POS"]["y"] ,stageData["POS"]["z"] });

	// ���N���X����g�������^�փL���X�g����
	std::shared_ptr<GameScene> gameScene =
		std::dynamic_pointer_cast<GameScene>(SceneManager::GetInstance().GetNowScene());

	// �X�e�[�W
	const auto& stages = actorManager->GetActiveActorData().find(ActorType::STAGE);

	// �X�e�[�W�̒��g�������Ă��邩�m�F
	if (stages == actorManager->GetActiveActorData().end()) return;

	for (auto& stage : stages->second)
	{
		// Area5Collision�ɃL���X�g
		auto area5Collision = std::dynamic_pointer_cast<Area5Collision>(stage);

		// �G���A5�ɓ����蔻�������
		if (area5Collision)
		{
			area5Collision->SetIsCollision(true);
		}

	}

	// �v���C���[
	const auto& playerData = objectData[0]["PlayerData"];

	// �v���C���[�𐶐�
	actorManager->CreateActor<Player>(playerData, { -10800.0f,-19500.0f,-120000.0f });
	actorManager->ActiveData(ActorType::PLAYER, { -10800.0f,-19500.0f,-120000.0f });

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
	actorManager->CreateActor<Boss>(bossData, { -10800.0f,-19500.0f,-140000.0f });
	actorManager->ActiveData(ActorType::BOSS, { -10800.0f,-19500.0f,-140000.0f });

}

void BossBattleActorCreate::Update()
{
}

void BossBattleActorCreate::Draw()
{
}
