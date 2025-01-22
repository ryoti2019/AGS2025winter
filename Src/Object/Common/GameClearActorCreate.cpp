#include <fstream>
#include "../Application.h"
#include "../Lib/nlohmann/json.hpp"
#include "../Manager/SceneManager.h"
#include "../Manager/ActorManager.h"
#include "../Scene/GameClearScene.h"
#include "../Object/GameClearPlayer.h"
#include "../Object/Stage.h"
#include "GameClearActorCreate.h"

GameClearActorCreate::GameClearActorCreate()
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
	std::shared_ptr<GameClearScene> gameClearScene =
		std::dynamic_pointer_cast<GameClearScene>(SceneManager::GetInstance().GetNowScene());

	// NULL�`�F�b�N
	if (!gameClearScene) return;

	// �A�N�^�[�}�l�[�W���[���擾
	std::shared_ptr<ActorManager> actorManager = gameClearScene->GetActorManager();

	// �X�e�[�W
	const auto& stageData = objectData[3]["StageData"];

	// �X�e�[�W�𐶐�
	actorManager->CreateActor<Stage>(stageData, { 0.0f,0.0f,0.0f });
	int cnt = actorManager->GetDeActiveActorData().find(ActorType::STAGE)-> second[0].use_count();
	//actorManager->ActiveData(ActorType::STAGE, { stageData["POS"]["x"], stageData["POS"]["y"] ,stageData["POS"]["z"] });
	auto s = actorManager->ActiveData(ActorType::STAGE, { stageData["POS"]["x"], stageData["POS"]["y"] ,stageData["POS"]["z"] });
	cnt = s.use_count();
	int b = 2;
	b = 3;
	// �v���C���[
	const auto& playerData = objectData[0]["PlayerData"];

	// �v���C���[�𐶐�
	actorManager->CreateActor<GameClearPlayer>(playerData, { -8700.0f,-19500.0f,-140000.0f });
	actorManager->ActiveData(ActorType::PLAYER, { -8700.0f,-19500.0f,-140000.0f });

}
