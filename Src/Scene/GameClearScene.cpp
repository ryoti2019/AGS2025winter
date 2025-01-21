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
	//SceneManager::GetInstance().GetCamera().lock()->ChangeMode(Camera::MODE::GAME_CLEAR);

	// �摜�̏�����
	//InitImage();

	// BGM��SE�̏�����
	//InitBGMAndSE();

}

void GameClearScene::InitImage()
{
}

void GameClearScene::InitBGMAndSE()
{
}

void GameClearScene::Update(const float deltaTime)
{

	// �A�N�^�[�̊Ǘ��N���X�̍X�V
	actorManager_->Update(deltaTime);

	// �A�N�^�[�̐����N���X�̍X�V
	actorCreate_->Update();

}

void GameClearScene::Draw(const float deltaTime)
{

	// �A�N�^�[�̊Ǘ��N���X�̍X�V
	actorManager_->Draw(deltaTime);

	// �^�C�g�����S�̉摜�̕`��
	DrawRotaGraph(850, 300, 1.0, 0.0, gameClearImg_, true);

	float scale = 1.0f;  // �g�嗦
	const float animationSpeed = 0.05f; // �A�j���[�V�����̑��x

	// �X�P�[�������Ԃɉ����ĕύX�isin�g�ŕω��j
	scale = 1.0 + 0.1f * std::sin(frameCount_ * animationSpeed);

	// Press A Button or Space�̉摜�̕`��
	DrawRotaGraph(850, 500, scale, 0.0, pressAButtonOrSpaceImg_, true);

	// �t���[���J�E���g���X�V
	frameCount_++;

}