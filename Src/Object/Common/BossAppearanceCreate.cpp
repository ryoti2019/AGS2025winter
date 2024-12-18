#include <fstream>
#include "../Lib/nlohmann/json.hpp"
#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ActorManager.h"
#include "../Scene/BossAppearanceScene.h"
#include "BossAppearanceCreate.h"
#include "../Object/Boss.h"
#include "../Object/Stage.h"
#include "../Object/StageCollision.h"

BossAppearanceCreate::BossAppearanceCreate()
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
	std::shared_ptr<BossAppearanceScene> bossAppearanceScene =
		std::dynamic_pointer_cast<BossAppearanceScene>(SceneManager::GetInstance().GetNowScene());

	// NULL�`�F�b�N
	if (!bossAppearanceScene) return;

	// �A�N�^�[�}�l�[�W���[���擾
	std::shared_ptr<ActorManager> actorManager = bossAppearanceScene->GetActorManager();

	// �X�e�[�W
	const auto& stageData = objectData[3]["StageData"];

	// �X�e�[�W�𐶐�
	actorManager->CreateActor<Stage>(stageData, { 0.0f,0.0f,0.0f });
	actorManager->ActiveData(ActorType::STAGE, { stageData["POS"]["x"], stageData["POS"]["y"] ,stageData["POS"]["z"] });

	// �X�e�[�W�̓����蔻��𐶐�
	actorManager->CreateActor<StageCollision>(stageData, { 0.0f,0.0f,0.0f });
	actorManager->ActiveData(ActorType::STAGE, { stageData["POS"]["x"], stageData["POS"]["y"] ,stageData["POS"]["z"] });

	// �{�X
	const auto& bossData = objectData[2]["BossData"];

	// �{�X�𐶐�
	actorManager->CreateActor<Boss>(bossData, { -10800.0f,-10000.0f,-150000.0f });
	actorManager->ActiveData(ActorType::BOSS, { -10800.0f,-10000.0f,-150000.0f });

}

void BossAppearanceCreate::Update()
{
}

void BossAppearanceCreate::Draw()
{
}
