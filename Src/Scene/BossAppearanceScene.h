#pragma once
#include "SceneBase.h"

class ActorManager;
class BossAppearanceCreate;
class CollisionManager;

class BossAppearanceScene : public SceneBase
{

public:

	BossAppearanceScene();

	~BossAppearanceScene() = default;

	void Init() override;
	void Update(const float deltaTime) override;
	void Draw(const float deltaTime) override;

	// �A�N�^�[�̊Ǘ��N���X���擾
	const std::shared_ptr<ActorManager>& GetActorManager() const { return actorManager_; }

	// �Փ˔���̊Ǘ��N���X���擾
	const std::shared_ptr<CollisionManager>& GetCollisionManager() const { return collisionManager_; }

	// �{�X�o��p�̃{�X�݂̂̐����N���X���擾
	const std::shared_ptr<BossAppearanceCreate>& GetActorCreate() const { return actorCreate_; }

private:

	// �A�N�^�[�̊Ǘ��N���X
	std::shared_ptr<ActorManager> actorManager_;

	// �Փ˔���̊Ǘ��N���X
	std::shared_ptr<CollisionManager> collisionManager_;

	// �{�X�o��p�̃{�X�݂̂̐����N���X
	std::shared_ptr<BossAppearanceCreate> actorCreate_;

};

