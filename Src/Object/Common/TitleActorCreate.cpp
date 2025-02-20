#include <fstream>
#include "../Lib/nlohmann/json.hpp"
#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ActorManager.h"
#include "../Scene/TitleScene.h"
#include "../Object/TitlePlayer.h"
#include "../Object/Stage.h"
#include "TitleActorCreate.h"

TitleActorCreate::TitleActorCreate()
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
	std::shared_ptr<TitleScene> titleScene =
		std::dynamic_pointer_cast<TitleScene>(SceneManager::GetInstance().GetNowScene());

	// NULL�`�F�b�N
	if (!titleScene) return;

	// �A�N�^�[�}�l�[�W���[���擾
	std::shared_ptr<ActorManager> actorManager = titleScene->GetActorManager();

	// �X�e�[�W
	//const auto& stageData = objectData[3]["StageData"];

	//// �X�e�[�W�𐶐�
	//actorManager->CreateActor<Stage>(stageData, { 0.0f,0.0f,0.0f });
	//actorManager->ActiveData(ActorType::STAGE, { stageData["POS"]["x"], stageData["POS"]["y"] ,stageData["POS"]["z"] });

	// �v���C���[
	const auto& playerData = objectData[0]["PlayerData"];

	// �v���C���[�𐶐�
	actorManager->CreateActor<TitlePlayer>(playerData, { -80000.0f,-19500.0f,25900.0f });
	actorManager->ActiveData(ActorType::PLAYER, { -80000.0f,-19500.0f,25900.0f });

}