#include <fstream>
#include "../Lib/nlohmann/json.hpp"
#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ActorManager.h"
#include "../Scene/BossAppearanceScene.h"
#include "BossAppearanceCreate.h"
#include "../Object/BossAppearanceBoss.h"
#include "../Object/BossAppearancePlayer.h"
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
	const auto& stageData = objectData[static_cast<int>(ActorType::STAGE)]["StageData"];

	// �X�e�[�W�𐶐�
	actorManager->CreateActor<Stage>(stageData, { 0.0f,0.0f,0.0f });
	actorManager->ActiveData(ActorType::STAGE, { stageData["POS"]["x"], stageData["POS"]["y"] ,stageData["POS"]["z"] });

	// �X�e�[�W�̓����蔻��𐶐�
	actorManager->CreateActor<StageCollision>(stageData, { 0.0f,0.0f,0.0f });
	actorManager->ActiveData(ActorType::STAGE, { stageData["POS"]["x"], stageData["POS"]["y"] ,stageData["POS"]["z"] });

	// �v���C���[
	const auto& playerData = objectData[static_cast<int>(ActorType::PLAYER)]["PlayerData"];

	// �v���C���[�𐶐�
	actorManager->CreateActor<BossAppearancePlayer>(playerData, PLAYER_POS);
	actorManager->ActiveData(ActorType::PLAYER, PLAYER_POS);

	// �{�X
	const auto& bossData = objectData[static_cast<int>(ActorType::BOSS)]["BossData"];

	// �{�X�𐶐�
	actorManager->CreateActor<BossAppearanceBoss>(bossData, BOSS_POS);
	actorManager->ActiveData(ActorType::BOSS, BOSS_POS);

}

void BossAppearanceCreate::Update()
{
}

void BossAppearanceCreate::Draw()
{
}
