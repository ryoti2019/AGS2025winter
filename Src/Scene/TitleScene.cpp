#include <DxLib.h>
#include "../Application.h"
#include "../Manager/ActorManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Object/Common/TitleActorCreate.h"
#include "TitleScene.h"

TitleScene::TitleScene()
{
}

void TitleScene::Init()
{

	// �A�N�^�[�Ǘ��N���X
	actorManager_ = std::make_shared<ActorManager>();

	// �A�N�^�[�̐����N���X
	actorCreate_ = std::make_shared<TitleActorCreate>();

	// �J�������[�h���{�X�̓o��p�̃��[�h�ɕύX
	SceneManager::GetInstance().GetCamera().lock()->ChangeMode(Camera::MODE::TITLE);

	// �摜�̏�����
	InitImage();

	// BGM��SE�̏�����
	InitBGMAndSE();

}
void TitleScene::InitImage()
{

	// �^�C�g�����S�̉摜�̏�����
	titleLogoImg_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::IMAGE_TITLE_LOGO)]).handleId_;

	// Press A Button or Space�̉摜�̏�����
	pressAButtonOrSpaceImg_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::IMAGE_PRESS_A_BUTTON_OR_SPACE)]).handleId_;

	// Press A Button or Space�p�̃J�E���^�̏�����
	frameCount_ = 0.0f;

}

void TitleScene::InitBGMAndSE()
{

	// BGM�̏�����
	bgm_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::SOUND_TITLE_SCENE_BGM)]).handleId_;

	// BGM�Đ�
	PlaySoundMem(bgm_, DX_PLAYTYPE_LOOP, true);

	// ����{�^��SE�̏�����
	decideSE_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::SOUND_TITLE_SCENE_DECIDE_SE)]).handleId_;

}

void TitleScene::Update(const float deltaTime)
{

	// �A�N�^�[�̊Ǘ��N���X�̍X�V
	actorManager_->Update(deltaTime);

	// �A�N�^�[�̐����N���X�̍X�V
	actorCreate_->Update();

	InputManager& ins = InputManager::GetInstance();

	// �V�[���J��
	if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_SPACE))
	{

		// ����{�^��SE�̍Đ�
		PlaySoundMem(decideSE_, DX_PLAYTYPE_LOOP, true);

		SceneManager::GetInstance().ChangeScene(SCENE_ID::GAME);

	}
	else if (ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN))
	{
		SceneManager::GetInstance().ChangeScene(SCENE_ID::GAME);
		SceneManager::GetInstance().SetGamePad(true);
	}

}

void TitleScene::Draw(const float deltaTime)
{

	// �A�N�^�[�̊Ǘ��N���X�̍X�V
	actorManager_->Draw(deltaTime);

	// �^�C�g�����S�̉摜�̕`��
	DrawRotaGraph(850, 300, 1.0, 0.0, titleLogoImg_, true);

	float scale = 1.0f;  // �g�嗦
	const float animationSpeed = 0.05f; // �A�j���[�V�����̑��x

	// �X�P�[�������Ԃɉ����ĕύX�isin�g�ŕω��j
	scale = 1.0 + 0.1f * std::sin(frameCount_ * animationSpeed);

	// Press A Button or Space�̉摜�̕`��
	DrawRotaGraph(850, 500, scale, 0.0, pressAButtonOrSpaceImg_, true);

	// �t���[���J�E���g���X�V
	frameCount_++;

}