#include <DxLib.h>
#include "../Application.h"
#include "../Manager/ActorManager.h"
#include "../Manager/CollisionManager.h"
#include "../Manager/SceneManager.h"
#include "../Object/Common/ActorCreate.h"
#include "../Object/Common/InputController.h"
#include "GameScene.h"
#include "../Object/Area1Collision.h"
#include "../Object/Area2Collision.h"
#include "../Object/Area5Collision.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
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

	// ������������Ă��邩�̃t���O
	isViewUserGuide_ = true;

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

}

void GameScene::InitBGMAndSE()
{

	// BGM�̏�����
	bgm_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::SOUND_GAME_SCENE_BGM)]).handleId_;

	// BGM�Đ�
	PlaySoundMem(bgm_, DX_PLAYTYPE_LOOP, true);

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

	// �X�e�[�W�����邩�`�F�b�N
	if (!actorManager_->GetActiveActorData().contains(ActorType::STAGE))return;

	// �X�e�[�W�����o��
	auto& stages = actorManager_->GetActiveActorData().at(ActorType::STAGE);

	// Area5Collision��T���Ă���Α������
	auto area5Collision = std::find_if(stages.begin(), stages.end(), [](const std::shared_ptr<ActorBase>& ptr)
		{
			return ptr == std::dynamic_pointer_cast<Area5Collision>(ptr);
		});

	// Area5�ɃL���X�g
	auto area5 = std::dynamic_pointer_cast<Area5Collision>(*area5Collision);

	// �Փ˂��Ă���΃{�X�̓o��V�[���ɑJ��
	if (area5->GetIsCollision())
	{
		SceneManager::GetInstance().ChangeScene(SCENE_ID::BOSS_APPEARANCE);
	}

}

void GameScene::Draw(const float deltaTime)
{

	// �A�N�^�[�̊Ǘ��N���X�̍X�V
	actorManager_->Draw(deltaTime);

	// �A�N�^�[�̐����N���X�̕`��
	actorCreate_->Draw();

	// ����������X�L�b�v�����܂ŕ`��
	if (isViewUserGuide_)
	{

		// �������̍�����`����ʑS�̂ɕ`��
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200); // �������i128: 50%�̓����x�j
		DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, GetColor(0, 0, 0), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); // �u�����h���[�h����
		
		if (!SceneManager::GetInstance().GetGamePad())
		{
			// �L�[�{�[�h�̑�������̉摜��`��
			DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, 0.5f, 0.0f, keyboardUserGuideImg_, true);
		}
		else if (SceneManager::GetInstance().GetGamePad())
		{
			// �Q�[���p�b�h�̑�������̉摜��`��
			DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, 0.5f, 0.0f, gamePadUserGuideImg_, true);
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
				DrawRotaGraph(150, 200, 0.5, 0.0, keyboardUserGuideMoveImg_, true);
			}
			else if (area1Collision && area1Collision->GetIsCollision())
			{
				// �U���̃`���[�g���A���摜��`��
				DrawRotaGraph(150, 200, 0.5, 0.0, keyboardUserGuideAttackImg_, true);
			}

			// Area2Collision�ɃL���X�g
			auto area2Collision = std::dynamic_pointer_cast<Area2Collision>(stage);

			if (area2Collision && area2Collision->GetIsCollision())
			{
				// �K�E�Z�̃`���[�g���A���摜��`��
				DrawRotaGraph(150, 200, 0.5, 0.0, keyboardUserGuideSpecialAttackImg_, true);
			}

		}
		else if (SceneManager::GetInstance().GetGamePad())
		{

			// Area1Collision�ɃL���X�g
			auto area1Collision = std::dynamic_pointer_cast<Area1Collision>(stage);

			if (area1Collision && !area1Collision->GetIsCollision() && !area1Collision->GetIsDissolve())
			{
				// �ړ��̃`���[�g���A���摜��`��
				DrawRotaGraph(150, 200, 0.5, 0.0, gamePadUserGuideMoveImg_, true);
			}
			else if (area1Collision && area1Collision->GetIsCollision())
			{
				// �U���̃`���[�g���A���摜��`��
				DrawRotaGraph(150, 200, 0.5, 0.0, gamePadUserGuideAttackImg_, true);
			}

			// Area2Collision�ɃL���X�g
			auto area2Collision = std::dynamic_pointer_cast<Area2Collision>(stage);

			if (area2Collision && area2Collision->GetIsCollision())
			{
				// �K�E�Z�̃`���[�g���A���摜��`��
				DrawRotaGraph(150, 200, 0.5, 0.0, gamePadUserGuideSpecialAttackImg_, true);
			}

		}

	}

}