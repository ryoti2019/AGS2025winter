#include <fstream>
#include <memory>
#include "../Lib/nlohmann/json.hpp"
#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ActorManager.h"
#include "../Scene/GameScene.h"
#include "../Object/Player.h"
#include "../Object/Enemy.h"
#include "../Object/Stage.h"
#include "ActorCreate.h"

ActorCreate::ActorCreate()
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
	actorManager->CreateActor<Player>(playerData, { 0.0f,-1960.0f,0.0f });
	actorManager->ActiveData(ActorType::PLAYER, { 0.0f,-1960.0f,0.0f });

	// �G
	const auto& enemyData = objectData[1]["EnemyData"];

	// �G�𐶐�
	for (int i = 0; i < 5; i++)
	{
		float x = std::rand() % 10000;
		float z = std::rand() % 10000;
		actorManager->CreateActor<Enemy>(enemyData, { 0.0f,-1500.0f,0.0f });
		actorManager->ActiveData(ActorType::ENEMY, { -5000.0f + x,0.0f,-5000.0f + z });
	}

}
