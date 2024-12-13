#pragma once
#include <memory>
#include "../Scene/SceneBase.h"

class ActorManager;
class ActorCreate;
class CollisionManager;

class GameScene :public SceneBase
{

public:

	// �R���X�g���N�^
	GameScene();

	// �f�X�g���N�^
	~GameScene();

	void Init() override;
	void Update(const float deltaTime) override;
	void Draw(const float deltaTime) override;

	// �A�N�^�[�̊Ǘ��N���X���擾
	const std::shared_ptr<ActorManager>& GetActorManager() const { return actorManager_; }

	// �Փ˔���̊Ǘ��N���X���擾
	const std::shared_ptr<CollisionManager>& GetCollisionManager() const { return collisionManager_; }

	// �A�N�^�[�̐����N���X���擾
	const std::shared_ptr<ActorCreate>& GetActorCreate() const { return actorCreate_; }

private:

	// �A�N�^�[�̊Ǘ��N���X
	std::shared_ptr<ActorManager> actorManager_;

	// �Փ˔���̊Ǘ��N���X
	std::shared_ptr<CollisionManager> collisionManager_;

	// �A�N�^�[�̐����N���X
	std::shared_ptr<ActorCreate> actorCreate_;

};