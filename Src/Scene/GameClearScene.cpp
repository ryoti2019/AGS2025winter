#include "GameClearScene.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ActorManager.h"
#include "../Object/Common/GameClearActorCreate.h"

GameClearScene::GameClearScene()
{
}

void GameClearScene::Init()
{

	// �A�N�^�[�Ǘ��N���X
	actorManager_ = std::make_shared<ActorManager>();

	// �A�N�^�[�̐����N���X
	actorCreate_ = std::make_shared<GameClearActorCreate>();

	// �J�������[�h���{�X�̓o��p�̃��[�h�ɕύX
	SceneManager::GetInstance().GetCamera().lock()->ChangeMode(Camera::MODE::GAME_CLEAR);

	// �摜�̏�����
	InitImage();

	// BGM��SE�̏�����
	InitBGMAndSE();

}

void GameClearScene::InitImage()
{

	// �Q�[���N���A�摜�̏�����
	gameClearImg_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::IMAGE_GAME_CLEAR)]).handleId_;

	// Press A Button or Space�̉摜�̏�����
	pressAButtonOrSpaceImg_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::IMAGE_PRESS_A_BUTTON_OR_SPACE)]).handleId_;

	// Press A Button or Space�p�̃J�E���^�̏�����
	frameCount_ = 0.0f;

}

void GameClearScene::InitBGMAndSE()
{

	// BGM�̏�����
	bgm_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::SOUND_GAME_CLEAR_SCENE_BGM)]).handleId_;

	// BGM�̃{�����[���̕ύX
	ChangeVolumeSoundMem(255 * 0.5, bgm_);

	// BGM�Đ�
	PlaySoundMem(bgm_, DX_PLAYTYPE_LOOP, true);

	// ����{�^��SE�̏�����
	decideSE_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::SOUND_TITLE_SCENE_DECIDE_SE)]).handleId_;

}

void GameClearScene::Update(const float deltaTime)
{

	// �A�N�^�[�̊Ǘ��N���X�̍X�V
	actorManager_->Update(deltaTime);

	// �A�N�^�[�̐����N���X�̍X�V
	actorCreate_->Update();

	// �V�[���J��
	if (inputController_->Decide())
	{

		// ����{�^��SE�̍Đ�
		PlaySoundMem(decideSE_, DX_PLAYTYPE_BACK, true);

		// �^�C�g���V�[���ɑJ��
		SceneManager::GetInstance().ChangeScene(SCENE_ID::TITLE);

		// Release����񂾂��ʂ�Ȃ��悤�ɂ���t���O��true�ɂ���
		SceneManager::GetInstance().SetIsFirstRelease(true);

		// �Q�[���p�b�h�����g����悤�ɂ���̂����Z�b�g
		SceneManager::GetInstance().SetGamePad(false);

		StopSoundMem(bgm_);

	}

}

void GameClearScene::Draw(const float deltaTime)
{

	// �A�N�^�[�̊Ǘ��N���X�̍X�V
	actorManager_->Draw(deltaTime);

	// �Q�[���N���A�摜�̕`��
	DrawRotaGraph(850, 200, 2.0, 0.0, gameClearImg_, true);

	float scale = 1.0f;  // �g�嗦
	const float animationSpeed = 0.05f; // �A�j���[�V�����̑��x

	// �X�P�[�������Ԃɉ����ĕύX�isin�g�ŕω��j
	scale = 1.0 + 0.1f * std::sin(frameCount_ * animationSpeed);

	// Press A Button or Space�̉摜�̕`��
	DrawRotaGraph(850, 500, scale, 0.0, pressAButtonOrSpaceImg_, true);

	// �t���[���J�E���g���X�V
	frameCount_++;

}