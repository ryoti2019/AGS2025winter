#define NOMINMAX
#include <cmath>
#include "../Application.h"
#include "../Manager/ActorManager.h"
#include "../Manager/CollisionManager.h"
#include "../Manager/SceneManager.h"
#include "../Object/Common/BossAppearanceCreate.h"
#include "BossAppearanceScene.h"

BossAppearanceScene::BossAppearanceScene()
{

}

void BossAppearanceScene::Init()
{

	// �A�N�^�[�Ǘ��N���X
	actorManager_ = std::make_shared<ActorManager>();

	// �Փ˔���Ǘ��N���X
	collisionManager_ = std::make_shared<CollisionManager>();

	// �Փ˔���Ǘ��N���X�ɃJ�����̏���ݒ�
	collisionManager_->SetCamera(SceneManager::GetInstance().GetCamera());

	// �A�N�^�[�̐����N���X
	actorCreate_ = std::make_shared<BossAppearanceCreate>();
	
	// �J�������[�h���{�X�̓o��p�̃��[�h�ɕύX
	SceneManager::GetInstance().GetCamera().lock()->ChangeMode(Camera::MODE::APPEARANCE);

	// �摜�̏�����
	InitImage();

	// BGM��SE�̏�����
	InitBGMAndSE();

}

void BossAppearanceScene::InitImage()
{

	// �{�X�����ꂽ!!�p�̉摜
	appearanceImg_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::IMAGE_BOSS_APPEARANCE)]).handleId_;

	// �{�X�����ꂽ!!�p�̉摜
	appearance2Img_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::IMAGE_BOSS_APPEARANCE)]).handleId_;

	// 1���ڂ̉摜�̃X�P�[��
	img1Scale_ = 2.0f;

	// 2���ڂ̉摜�̃X�P�[��
	img2Scale_ = 1.0f;

	// 1���ڂ̓����x
	img1Alpha_ = 0;

	// 2���ڂ̓����x
	img2Alpha_ = 255;

}

void BossAppearanceScene::InitBGMAndSE()
{

	// BGM�̏�����
	bgm_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::SOUND_BOSS_APPEARANCE_SCENE_BGM)]).handleId_;

	// BGM�̃{�����[���̕ύX
	ChangeVolumeSoundMem(SOUND_MAX * SOUND_BGM_VOLUME, bgm_);

	// BGM�Đ�
	PlaySoundMem(bgm_, DX_PLAYTYPE_LOOP, true);

}

void BossAppearanceScene::Update(const float deltaTime)
{

	// �A�N�^�[�̊Ǘ��N���X�̍X�V
	actorManager_->Update(deltaTime);

	// �Փ˔���̊Ǘ��N���X�̍X�V
	collisionManager_->Update(deltaTime);

	// �A�N�^�[�̐����N���X�̍X�V
	actorCreate_->Update();

	// �J�����𐶐�
	std::weak_ptr<Camera> camera = SceneManager::GetInstance().GetCamera();

	// �{�X�̓o��V�[�����I������Ă��邩
	if (camera.lock()->GetIsEndBossAppearanceScene())
	{
		SceneManager::GetInstance().ChangeScene(SCENE_ID::BOSS_BATTLE);
	}

}

void BossAppearanceScene::Draw(const float deltaTime)
{

	// �A�N�^�[�̊Ǘ��N���X�̍X�V
	actorManager_->Draw(deltaTime);

	if (SceneManager::GetInstance().GetCamera().lock()->GetElapsedTime() >= BOSS_NAME_DRAW_START_TIME && SceneManager::GetInstance().GetCamera().lock()->GetIsBossAppearanceCameraMove3())
	{

		// �u�����h���[�h�̐ݒ�
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, img1Alpha_);

		// �{�X�����ꂽ!!�p�̉摜�̕`��
		DrawRotaGraph(Application::SCREEN_SIZE_X / 2, BOSS_APPEAR_TEXT_Y, img1Scale_, 0.0, appearanceImg_, true);

		// �u�����h���[�h������
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// �A���t�@�l�𑝉��i255������Ƃ���j
		img1Alpha_ += IMG1_ALPHA_INCREMENT;

		// �ő�l��255�ɂ���
		img1Alpha_ = std::min(img1Alpha_, IMG1_ALPHA_MAX);

		// �摜�̃X�P�[��������
		img1Scale_ -= IMG1_SCALE_DECREASE;
		
		// �ŏ��l��1.0f�ɂ���
		img1Scale_ = std::max(img1Scale_, IMG1_SCALE_MIN);

		// 1���ڂ̉摜�̃X�P�[����1.0�ɂȂ������납��摜���o��
		if (img1Scale_ == IMG1_SCALE_MIN && img2Scale_ <= IMG2_SCALE_MAX)
		{

			// �u�����h���[�h�̐ݒ�
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, img2Alpha_);

			// �{�X�����ꂽ!!�p�̉摜�̕`��
			DrawRotaGraph(Application::SCREEN_SIZE_X / 2, BOSS_APPEAR_TEXT_Y, img2Scale_, 0.0, appearance2Img_, true);

			// �u�����h���[�h������
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

			// �A���t�@�l�������i255������Ƃ���j
			img2Alpha_ -= IMG2_ALPHA_DECREASE; // �A���t�@�l�𑝂₷���x�𒲐��\
			
			// �ŏ��l��0�ɂ���
			img2Alpha_ = std::max(img2Alpha_, 0);

			// �X�P�[���l�𑝉�
			img2Scale_ += IMG2_SCALE_INCREMENT;

			// �ő�l��3.0f�ɂ���
			img1Scale_ = std::min(img1Scale_, IMG2_SCALE_MAX);

		}

	}

}