#define NOMINMAX
#include <DxLib.h>
#include "../Application.h"
#include "../Manager/ActorManager.h"
#include "../Manager/CollisionManager.h"
#include "../Manager/SceneManager.h"
#include "../Object/Common/ActorCreate.h"
#include "../Object/Common/InputController.h"
#include "GameScene.h"
#include "../Object/Player.h"
#include "../Object/Area1Collision.h"
#include "../Object/Area2Collision.h"
#include "../Object/Area5Collision.h"

GameScene::GameScene()
{
}

void GameScene::Init()
{

	// �A�N�^�[�Ǘ��N���X
	actorManager_ = std::make_shared<ActorManager>();

	// �Փ˔���Ǘ��N���X
	collisionManager_ = std::make_shared<CollisionManager>();

	// �Փ˔���Ǘ��N���X�ɃJ�����̏���ݒ�
	collisionManager_->SetCamera(SceneManager::GetInstance().GetCamera());

	// �A�N�^�[�̐����N���X
	actorCreate_ = std::make_shared<ActorCreate>();

	// �v���C���[�ɒǏ]���郂�[�h�ɕύX
	SceneManager::GetInstance().GetCamera().lock()->ChangeMode(Camera::MODE::FOLLOW);

	// �摜�̏�����
	InitImage();

	// BGM��SE�̏�����
	InitBGMAndSE();

	// �Q�[���I�[�o�[��\���������邽�߂̃J�E���^
	gameOverCnt_ = 0.0f;

	// ������������Ă��邩�̃t���O
	isViewUserGuide_ = true;

	// �v���C���[�����񂾂Ƃ��̃t���O
	isPlayerDeath_ = false;

	// �R���e�B�j���[���邩�̃t���t
	isContinue_ = true;

	// �Q�[���I�[�o�[�̃t���O
	isGameOver_ = false;

}

void GameScene::InitImage()
{

	// �Q�[���p�b�h�̑�������̉摜�̏�����
	gamePadUserGuideImg_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::IMAGE_GAME_PAD_USER_GUIDE)]).handleId_;

	// �Q�[���p�b�h�̈ړ��̐����̉摜�̏�����
	gamePadUserGuideMoveImg_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::IMAGE_GAME_PAD_USER_GUIDE_MOVE)]).handleId_;

	// �Q�[���p�b�h�̍U���̐����̉摜�̏�����
	gamePadUserGuideAttackImg_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::IMAGE_GAME_PAD_USER_GUIDE_ATTACK)]).handleId_;

	// �Q�[���p�b�h�̕K�E�Z�̐����̉摜�̏�����
	gamePadUserGuideSpecialAttackImg_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::IMAGE_GAME_PAD_USER_GUIDE_SPECIAL_ATTACK)]).handleId_;

	// �L�[�{�[�h�̑�������̉摜�̏�����
	keyboardUserGuideImg_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::IMAGE_KEYBOARD_USER_GUIDE)]).handleId_;

	// �L�[�{�[�h�̈ړ��̐����̉摜�̏�����
	keyboardUserGuideMoveImg_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::IMAGE_KEYBOARD_USER_GUIDE_MOVE)]).handleId_;

	// �L�[�{�[�h�̍U���̐����̉摜�̏�����
	keyboardUserGuideAttackImg_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::IMAGE_KEYBOARD_USER_GUIDE_ATTACK)]).handleId_;

	// �L�[�{�[�h�̕K�E�Z�̐����̉摜�̏�����
	keyboardUserGuideSpecialAttackImg_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::IMAGE_KEYBOARD_USER_GUIDE_SPECIAL_ATTACK)]).handleId_;

	// �Q�[���I�[�o�[�摜�̏�����
	gameOverImg_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::IMAGE_GAME_OVER)]).handleId_;

	// �Q�[���I�[�o�[�摜2�̏�����
	gameOverImg2_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::IMAGE_GAME_OVER)]).handleId_;

	// �摜�̃X�P�[��
	img1Scale_ = 2.0f;

	// �摜�̃X�P�[��
	img2Scale_ = 1.0f;

	// �����x
	img1Alpha_ = 0;

	// �����x
	img2Alpha_ = IMG1_ALPHA_MAX;

	// �R���e�B�j���[�摜�̏�����
	continueImg_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::IMAGE_CONTINUE)]).handleId_;
	
	// Yes�̉摜�̏�����
	yesImg_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::IMAGE_YES)]).handleId_;
	
	// No�̉摜�̏�����
	noImg_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::IMAGE_NO)]).handleId_;

}

void GameScene::InitBGMAndSE()
{

	// BGM�̏�����
	bgm_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::SOUND_GAME_SCENE_BGM)]).handleId_;

	// BGM�̃{�����[���̕ύX
	ChangeVolumeSoundMem(SOUND_MAX * BGM_VOLUME, bgm_);

	// BGM�Đ�
	PlaySoundMem(bgm_, DX_PLAYTYPE_LOOP, true);

	// �Q�[���I�[�o�[�{�C�X
	gameOverVoice_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::SOUND_GAME_OVER_SCENE_VOICE)]).handleId_;

	// ����{�^��SE�̏�����
	decideSE_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::SOUND_TITLE_SCENE_DECIDE_SE)]).handleId_;

	// �J�[�\���ړ���SE
	cursorMoveSE_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::SOUND_CURSOR_MOVE_SE)]).handleId_;

}

void GameScene::Update(const float deltaTime)
{

	// ����������X�L�b�v
	if (inputController_->Decide())
	{
		isViewUserGuide_ = false;
	}

	if (isViewUserGuide_)return;

	// �A�N�^�[�̊Ǘ��N���X�̍X�V
	actorManager_->Update(deltaTime);

	// �Փ˔���̊Ǘ��N���X�̍X�V
	collisionManager_->Update(deltaTime);

	// �A�N�^�[�̐����N���X�̍X�V
	actorCreate_->Update();

	// �{�X�̓o��V�[���ɑJ�ڂ��邩����
	CheckTransitionBossAppearanceScene();

	// �v���C���[�����񂾂�R���e�B�j���[���Q�[���I�[�o�[��I�΂���
	if (isPlayerDeath_)
	{
		SelectContinueOrGameOver(deltaTime);
	}

}

void GameScene::CheckTransitionBossAppearanceScene()
{

	// �X�e�[�W�����邩�`�F�b�N
	if (!actorManager_->GetActiveActorData().contains(ActorType::STAGE))return;

	// �v���C���[�����邩�`�F�b�N
	if (!actorManager_->GetActiveActorData().contains(ActorType::PLAYER))return;

	// �X�e�[�W�����o��
	auto& stages = actorManager_->GetActiveActorData().at(ActorType::STAGE);

	// �v���C���[�����o��
	auto& players = actorManager_->GetActiveActorData().at(ActorType::PLAYER);

	// Area5Collision��T���Ă���Α������
	auto area5Collision = std::find_if(stages.begin(), stages.end(), [](const std::shared_ptr<ActorBase>& ptr)
		{
			return ptr == std::dynamic_pointer_cast<Area5Collision>(ptr);
		});

	// Area5�ɃL���X�g
	auto area5 = std::dynamic_pointer_cast<Area5Collision>(*area5Collision);

	// �v���C���[��T���Ă���Α������
	auto player = std::find_if(players.begin(), players.end(), [](const std::shared_ptr<ActorBase>& ptr)
		{
			return ptr == std::dynamic_pointer_cast<Player>(ptr);
		});

	// �v���C���[�ɃL���X�g
	auto p = std::dynamic_pointer_cast<Player>(*player);

	// �Փ˂��Ă��邩
	if (area5->GetIsCollision())
	{

		// �{�X�̓o��V�[���ɑJ��
		SceneManager::GetInstance().ChangeScene(SCENE_ID::BOSS_APPEARANCE);

		// �v���C���[��HP��ݒ�
		SceneManager::GetInstance().SetPlayerSpecialAttackGauge(p->GetSpecialAttackGauge());

		// �v���C���[��HP��ݒ�
		SceneManager::GetInstance().SetPlayerHp(p->GetHp());

	}

}

void GameScene::SelectContinueOrGameOver(const float deltaTime)
{

	// �E�����ɃX�e�B�b�N��|���Ă��邩
	if (inputController_->SelectRight() && isContinue_)
	{

		isContinue_ = false;

		// �J�[�\���ړ���SE���Đ�
		PlaySoundMem(cursorMoveSE_, DX_PLAYTYPE_BACK);

	}

	// �������ɃX�e�B�b�N��|���Ă��邩
	if (inputController_->SelectLeft() && !isContinue_)
	{
		
		isContinue_ = true;

		// �J�[�\���ړ���SE���Đ�
		PlaySoundMem(cursorMoveSE_, DX_PLAYTYPE_BACK);

	}

	// �R���e�B�j���[����ꍇ
	if (isContinue_ && inputController_->Decide())
	{
		SceneManager::GetInstance().ChangeScene(SCENE_ID::GAME);
	}
	else if (!isContinue_ && inputController_->Decide())
	{

		isGameOver_ = true;

		// ����{�^����SE���Đ�
		PlaySoundMem(decideSE_, DX_PLAYTYPE_BACK);

		// �Q�[���I�[�o�[�{�C�X���Đ�
		PlaySoundMem(gameOverVoice_, DX_PLAYTYPE_BACK);

	}

	// �Q�[���I�[�o�[
	if (isGameOver_)
	{

		// �Q�[���I�[�o�[�ɂȂ���5�b�����Ă���^�C�g���ɖ߂�
		if (gameOverCnt_ >= GAME_OVER_DELAY)
		{

			SceneManager::GetInstance().ChangeScene(SCENE_ID::TITLE);

			// Release����񂾂��ʂ�Ȃ��悤�ɂ���t���O��true�ɂ���
			SceneManager::GetInstance().SetIsFirstRelease(true);

		}

		gameOverCnt_ += deltaTime;
	
	}

}

void GameScene::Draw(const float deltaTime)
{

	// �A�N�^�[�̊Ǘ��N���X�̍X�V
	actorManager_->Draw(deltaTime);

	// �A�N�^�[�̐����N���X�̕`��
	actorCreate_->Draw();

	// ��������̕`��
	DrawUserGuide();

	if (isPlayerDeath_)
	{
		// �Q�[���I�[�o�[�̕`��
		DrawGameOver();
	}

}

void GameScene::DrawUserGuide()
{

	// ����������X�L�b�v�����܂ŕ`��
	if (isViewUserGuide_)
	{

		// �������̍�����`����ʑS�̂ɕ`��
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, USER_GUIDE_ALPHA);
		
		DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, GetColor(0, 0, 0), TRUE);
		
		// �u�����h���[�h����
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		if (!SceneManager::GetInstance().GetGamePad())
		{
			// �L�[�{�[�h�̑�������̉摜��`��
			DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, USER_GUIDE_IMAGE_SCALE, 0.0f, keyboardUserGuideImg_, true);
		}
		else if (SceneManager::GetInstance().GetGamePad())
		{
			// �Q�[���p�b�h�̑�������̉摜��`��
			DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, USER_GUIDE_IMAGE_SCALE, 0.0f, gamePadUserGuideImg_, true);
		}

		return;
	}

	// �X�e�[�W�����邩�`�F�b�N
	if (!actorManager_->GetActiveActorData().contains(ActorType::STAGE))return;

	// �X�e�[�W�����o��
	auto& stages = actorManager_->GetActiveActorData().at(ActorType::STAGE);

	for (auto& stage : stages)
	{

		if (!SceneManager::GetInstance().GetGamePad())
		{

			// Area1Collision�ɃL���X�g
			auto area1Collision = std::dynamic_pointer_cast<Area1Collision>(stage);

			if (area1Collision && !area1Collision->GetIsCollision() && !area1Collision->GetIsDissolve())
			{
				// �ړ��̃`���[�g���A���摜��`��
				DrawRotaGraph(USER_GUIDE_POS_X, USER_GUIDE_POS_Y, USER_GUIDE_IMAGE_SCALE, 0.0, keyboardUserGuideMoveImg_, true);
			}
			else if (area1Collision && area1Collision->GetIsCollision())
			{
				// �U���̃`���[�g���A���摜��`��
				DrawRotaGraph(USER_GUIDE_POS_X, USER_GUIDE_POS_Y, USER_GUIDE_IMAGE_SCALE, 0.0, keyboardUserGuideAttackImg_, true);
			}

			// Area2Collision�ɃL���X�g
			auto area2Collision = std::dynamic_pointer_cast<Area2Collision>(stage);

			if (area2Collision && area2Collision->GetIsCollision())
			{
				// �K�E�Z�̃`���[�g���A���摜��`��
				DrawRotaGraph(USER_GUIDE_POS_X, USER_GUIDE_POS_Y, USER_GUIDE_IMAGE_SCALE, 0.0, keyboardUserGuideSpecialAttackImg_, true);
			}

		}
		else if (SceneManager::GetInstance().GetGamePad())
		{

			// Area1Collision�ɃL���X�g
			auto area1Collision = std::dynamic_pointer_cast<Area1Collision>(stage);

			if (area1Collision && !area1Collision->GetIsCollision() && !area1Collision->GetIsDissolve())
			{
				// �ړ��̃`���[�g���A���摜��`��
				DrawRotaGraph(USER_GUIDE_POS_X, USER_GUIDE_POS_Y, USER_GUIDE_IMAGE_SCALE, 0.0, gamePadUserGuideMoveImg_, true);
			}
			else if (area1Collision && area1Collision->GetIsCollision())
			{
				// �U���̃`���[�g���A���摜��`��
				DrawRotaGraph(USER_GUIDE_POS_X, USER_GUIDE_POS_Y, USER_GUIDE_IMAGE_SCALE, 0.0, gamePadUserGuideAttackImg_, true);
			}

			// Area2Collision�ɃL���X�g
			auto area2Collision = std::dynamic_pointer_cast<Area2Collision>(stage);

			if (area2Collision && area2Collision->GetIsCollision())
			{
				// �K�E�Z�̃`���[�g���A���摜��`��
				DrawRotaGraph(USER_GUIDE_POS_X, USER_GUIDE_POS_Y, USER_GUIDE_IMAGE_SCALE, 0.0, gamePadUserGuideSpecialAttackImg_, true);
			}

		}

	}

}

void GameScene::DrawGameOver()
{

	// �������̍�����`����ʑS�̂ɕ`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, GAME_OVER_ALPHA); // �������i128: 50%�̓����x�j
	DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, GetColor(0, 0, 0), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); // �u�����h���[�h����

	if (isGameOver_)
	{

		// �u�����h���[�h�̐ݒ�
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, img1Alpha_);

		// �Q�[���I�[�o�[�̕`��
		DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, GAME_OVER_IMG_SCALE, 0.0, gameOverImg_, true);

		// �u�����h���[�h������
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// �A���t�@�l�𑝉��i255������Ƃ���j
		img1Alpha_ += IMG1_ALPHA_INCREMENT;

		// �ŏ��l��255�ɂ���
		img1Alpha_ = std::min(img1Alpha_, IMG1_ALPHA_MAX);

		// �摜�̃X�P�[��������
		img1Scale_ -= IMG1_ALPHA_DECREASE;

		// �ő�l��1.0f�ɂ���
		img1Scale_ = std::max(img1Scale_, IMG1_SCALE_MAX);

		// 1���ڂ̉摜�̃X�P�[����1.0�ɂȂ������납��摜���o��
		if (img1Scale_ == IMG1_SCALE_MAX && img2Scale_ <= IMG2_SCALE_MAX)
		{

			// �u�����h���[�h�̐ݒ�
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, img2Alpha_);

			// �{�X�����ꂽ!!�p�̉摜�̕`��
			DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, img2Scale_, 0.0, gameOverImg2_, true);

			// �u�����h���[�h������
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

			// �A���t�@�l�𑝉��i255������Ƃ���j
			img2Alpha_ -= IMG2_ALPHA_DECREASE; // �A���t�@�l�𑝂₷���x�𒲐��\

			// �ŏ��l��0�ɂ���
			img2Alpha_ = std::max(img2Alpha_, 0);

			// �X�P�[���l�𑝉�
			img2Scale_ += IMG2_ALPHA_INCREMENT;

			// �ő�l��3.0f�ɂ���
			img1Scale_ = std::min(img1Scale_, IMG2_SCALE_MAX);

		}

	}
	else
	{

		// �R���e�B�j���[�摜�̕`��
		DrawRotaGraph(Application::SCREEN_SIZE_X / 2, CONTINUE_POS_Y, CONTINUE_IMAGE_SCALE, 0.0, continueImg_, true);

		float yesImgScale = YES_IMAGE_SCALE;
		float noImgScale = NO_IMAGE_SCALE;

		// �A�j���[�V�����̑��x
		const float animationSpeed = ANIM_SPEED;

		if (isContinue_)
		{
			// YES�摜�̃X�P�[�������Ԃɉ����ĕύX
			yesImgScale = YES_IMAGE_SCALE + SCLAE_VARIATION * std::sin(frameCount_ * animationSpeed);
		}
		else
		{
			// NO�摜�̃X�P�[�������Ԃɉ����ĕύX
			noImgScale = NO_IMAGE_SCALE + SCLAE_VARIATION * std::sin(frameCount_ * animationSpeed);
		}

		// YES�摜�̕`��
		DrawRotaGraph(Application::SCREEN_SIZE_X / 2 - YES_POS_X_OFFSET, YES_POS_Y, yesImgScale, 0.0, yesImg_, true);

		// NO�摜�̕`��
		DrawRotaGraph(Application::SCREEN_SIZE_X / 2 + NO_POS_X_OFFSET, NO_POS_Y, noImgScale, 0.0, noImg_, true);
	
		// �t���[���J�E���g���X�V
		frameCount_++;
	
	}

}