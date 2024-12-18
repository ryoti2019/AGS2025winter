#include <DxLib.h>
#include "../Manager/ActorManager.h"
#include "../Manager/CollisionManager.h"
#include "../Manager/SceneManager.h"
#include "../Object/Common/ActorCreate.h"
#include "GameScene.h"
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

	SceneManager::GetInstance().GetCamera().lock()->ChangeMode(Camera::MODE::FOLLOW);

}

void GameScene::Update(const float deltaTime)
{

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

	DrawFormatString(0, 0, 0xff0000, "GameScene");
}