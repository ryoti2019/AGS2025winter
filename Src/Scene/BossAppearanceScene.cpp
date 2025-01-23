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

	// �摜�̃X�P�[��
	scale_ = 2.0f;

	// �摜�̃X�P�[��
	scale2_ = 1.0f;

	// �����x
	alpha_ = 0;

	// �����x
	alpha2_ = 255;

}

void BossAppearanceScene::InitBGMAndSE()
{

	// BGM�̏�����
	bgm_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::SOUND_BOSS_APPEARANCE_SCENE_BGM)]).handleId_;

	// BGM�̃{�����[���̕ύX
	ChangeVolumeSoundMem(255 * 0.5, bgm_);

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

	if (SceneManager::GetInstance().GetCamera().lock()->GetElapsedTime() >= 6.0f && SceneManager::GetInstance().GetCamera().lock()->GetIsBossAppearanceCameraMove3())
	{

		// �u�����h���[�h�̐ݒ�
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_);

		// �{�X�����ꂽ!!�p�̉摜�̕`��
		DrawRotaGraph(Application::SCREEN_SIZE_X / 2, 500, scale_, 0.0, appearanceImg_, true);

		// �u�����h���[�h������
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// �A���t�@�l�𑝉��i255������Ƃ���j
		alpha_ += 2;

		// �ŏ��l��255�ɂ���
		alpha_ = std::min(alpha_,255);

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
			DrawRotaGraph(Application::SCREEN_SIZE_X / 2, 500, scale2_, 0.0, appearance2Img_, true);

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

}