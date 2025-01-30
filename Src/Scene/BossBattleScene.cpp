#define NOMINMAX
#include "../Application.h"
#include "../Manager/ActorManager.h"
#include "../Manager/CollisionManager.h"
#include "../Manager/SceneManager.h"
#include "../Object/Common/BossBattleActorCreate.h"
#include "BossBattleScene.h"
#include "../Object/Boss.h"

BossBattleScene::BossBattleScene()
{
}

void BossBattleScene::Init()
{

	// �A�N�^�[�Ǘ��N���X
	actorManager_ = std::make_shared<ActorManager>();

	// �Փ˔���Ǘ��N���X
	collisionManager_ = std::make_shared<CollisionManager>();

	// �Փ˔���Ǘ��N���X�ɃJ�����̏���ݒ�
	collisionManager_->SetCamera(SceneManager::GetInstance().GetCamera());

	// �A�N�^�[�̐����N���X
	actorCreate_ = std::make_shared<BossBattleActorCreate>();

	// �v���C���[�ɒǏ]���郂�[�h�ɕύX
	SceneManager::GetInstance().GetCamera().lock()->ChangeMode(Camera::MODE::FOLLOW);

	// �摜�̏�����
	InitImage();

	// BGM��SE�̏�����
	InitBGMAndSE();

	// �Q�[���I�[�o�[��\���������邽�߂̃J�E���^
	gameOverCnt_ = 0.0f;

	// �v���C���[�����񂾂Ƃ��̃t���O
	isPlayerDeath_ = false;

	// �R���e�B�j���[���邩�̃t���t
	isContinue_ = true;

	// �Q�[���I�[�o�[�̃t���O
	isGameOver_ = false;

}

void BossBattleScene::InitImage()
{

	// �Q�[���I�[�o�[�摜�̏�����
	gameOverImg_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::IMAGE_GAME_OVER)]).handleId_;

	// �Q�[���I�[�o�[�摜2�̏�����
	gameOverImg2_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::IMAGE_GAME_OVER)]).handleId_;

	// �摜�̃X�P�[��
	scale_ = 2.0f;

	// �摜�̃X�P�[��
	scale2_ = 1.0f;

	// �����x
	alpha_ = 0;

	// �����x
	alpha2_ = 255;

	// �R���e�B�j���[�摜�̏�����
	continueImg_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::IMAGE_CONTINUE)]).handleId_;

	// Yes�̉摜�̏�����
	yesImg_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::IMAGE_YES)]).handleId_;

	// No�̉摜�̏�����
	noImg_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::IMAGE_NO)]).handleId_;

}

void BossBattleScene::InitBGMAndSE()
{

	// BGM�̏�����
	bgm_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::SOUND_BOSS_BATTLE_SCENE_BGM)]).handleId_;

	// BGM�̃{�����[���̕ύX
	ChangeVolumeSoundMem(255 * 0.5, bgm_);

	// BGM�Đ�
	PlaySoundMem(bgm_, DX_PLAYTYPE_LOOP, true);

	// �Q�[���I�[�o�[�{�C�X
	gameOverVoice_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::SOUND_GAME_OVER_SCENE_VOICE)]).handleId_;

	// ����{�^��SE�̏�����
	decideSE_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::SOUND_TITLE_SCENE_DECIDE_SE)]).handleId_;

	// �J�[�\���ړ���SE
	cursorMoveSE_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::SOUND_CURSOR_MOVE_SE)]).handleId_;

}

void BossBattleScene::Update(const float deltaTime)
{

	// �A�N�^�[�̊Ǘ��N���X�̍X�V
	actorManager_->Update(deltaTime);

	// �Փ˔���̊Ǘ��N���X�̍X�V
	collisionManager_->Update(deltaTime);

	// �A�N�^�[�̐����N���X�̍X�V
	actorCreate_->Update();

	// �Q�[���N���A�V�[���ɑJ�ڂ��邩����
	CheckTransitionGameClearScene();

	// �v���C���[�����񂾂�R���e�B�j���[���Q�[���I�[�o�[��I�΂���
	if (isPlayerDeath_)
	{
		SelectContinueOrGameOver(deltaTime);
	}

}

void BossBattleScene::CheckTransitionGameClearScene()
{

	// �{�X�����邩�`�F�b�N
	if (!actorManager_->GetActiveActorData().contains(ActorType::BOSS))return;

	// �X�e�[�W�����o��
	auto& stages = actorManager_->GetActiveActorData().at(ActorType::BOSS);

	// Area5Collision��T���Ă���Α������
	auto boss = std::find_if(stages.begin(), stages.end(), [](const std::shared_ptr<ActorBase>& ptr)
		{
			return ptr == std::dynamic_pointer_cast<Boss>(ptr);
		});

	// Area5�ɃL���X�g
	auto b = std::dynamic_pointer_cast<Boss>(*boss);

	// �Փ˂��Ă���΃Q�[���N���A�V�[���ɑJ��
	if (!b->GetIsActive())
	{
		SceneManager::GetInstance().ChangeScene(SCENE_ID::GAME_CLEAR);
	}

}

void BossBattleScene::SelectContinueOrGameOver(const float deltaTime)
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
		if (gameOverCnt_ >= 5.0f)
		{

			SceneManager::GetInstance().ChangeScene(SCENE_ID::TITLE);

			// Release����񂾂��ʂ�Ȃ��悤�ɂ���t���O��true�ɂ���
			SceneManager::GetInstance().SetIsFirstRelease(true);

		}

		gameOverCnt_ += deltaTime;

	}

}

void BossBattleScene::Draw(const float deltaTime)
{

	// �A�N�^�[�̊Ǘ��N���X�̍X�V
	actorManager_->Draw(deltaTime);

	// �A�N�^�[�̐����N���X�̕`��
	actorCreate_->Draw();

	if (isPlayerDeath_)
	{
		// �Q�[���I�[�o�[�̕`��
		DrawGameOver();
	}

}

void BossBattleScene::DrawGameOver()
{

	// �������̍�����`����ʑS�̂ɕ`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128); // �������i128: 50%�̓����x�j
	DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, GetColor(0, 0, 0), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); // �u�����h���[�h����

	if (isGameOver_)
	{

		// �u�����h���[�h�̐ݒ�
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_);

		// �Q�[���I�[�o�[�̕`��
		DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, 1.0, 0.0, gameOverImg_, true);

		// �u�����h���[�h������
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// �A���t�@�l�𑝉��i255������Ƃ���j
		alpha_ += 2;

		// �ŏ��l��255�ɂ���
		alpha_ = std::min(alpha_, 255);

		// �摜�̃X�P�[��������
		scale_ -= 0.02f;

		// �ő�l��1.0f�ɂ���
		scale_ = std::max(scale_, 1.0f);

		// 1���ڂ̉摜�̃X�P�[����1.0�ɂȂ������납��摜���o��
		if (scale_ == 1.0f && scale2_ <= 3.0f)
		{

			// �u�����h���[�h�̐ݒ�
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha2_);

			// �{�X�����ꂽ!!�p�̉摜�̕`��
			DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, scale2_, 0.0, gameOverImg2_, true);

			// �u�����h���[�h������
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

			// �A���t�@�l�𑝉��i255������Ƃ���j
			alpha2_ -= 10; // �A���t�@�l�𑝂₷���x�𒲐��\

			// �ŏ��l��0�ɂ���
			alpha2_ = std::max(alpha2_, 0);

			// �X�P�[���l�𑝉�
			scale2_ += 0.05f;

			// �ő�l��3.0f�ɂ���
			scale_ = std::min(scale_, 3.0f);

		}

	}
	else
	{

		// �R���e�B�j���[�摜�̕`��
		DrawRotaGraph(Application::SCREEN_SIZE_X / 2, 200, 0.8, 0.0, continueImg_, true);

		float yesImgScale = 1.0f;
		float noImgScale = 1.0f;

		// �A�j���[�V�����̑��x
		const float animationSpeed = 0.05f;

		if (isContinue_)
		{
			// YES�摜�̃X�P�[�������Ԃɉ����ĕύX
			yesImgScale = 1.0 + 0.1f * std::sin(frameCount_ * animationSpeed);
		}
		else
		{
			// NO�摜�̃X�P�[�������Ԃɉ����ĕύX
			noImgScale = 1.0 + 0.1f * std::sin(frameCount_ * animationSpeed);
		}

		// YES�摜�̕`��
		DrawRotaGraph(Application::SCREEN_SIZE_X / 2 - 300, 500, yesImgScale, 0.0, yesImg_, true);

		// NO�摜�̕`��
		DrawRotaGraph(Application::SCREEN_SIZE_X / 2 + 300, 500, noImgScale, 0.0, noImg_, true);

		// �t���[���J�E���g���X�V
		frameCount_++;

	}

}
