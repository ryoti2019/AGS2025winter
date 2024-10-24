#pragma once
#include <memory>
#include "../Scene/SceneBase.h"

class ActorManager;
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
	void Draw() override;

	// �A�N�^�[�̊Ǘ��N���X���擾
	const std::shared_ptr<ActorManager>& GetActorManager() const { return actorManager_; }

	// �Փ˔���̊Ǘ��N���X���擾
	const std::shared_ptr<CollisionManager>& GetCollisionManager() const { return collisionManager_; }

private:

	// �A�N�^�[�̊Ǘ��N���X
	std::shared_ptr<ActorManager> actorManager_;

	// �Փ˔���̊Ǘ��N���X
	std::shared_ptr<CollisionManager> collisionManager_;

};