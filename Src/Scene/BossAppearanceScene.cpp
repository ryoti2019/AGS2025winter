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

	DrawFormatString(0, 0, 0xff0000, "BossAppearanceScene");

}
