#pragma once
#include "SceneBase.h"

class ActorManager;
class BossAppearanceCreate;
class CollisionManager;

class BossAppearanceScene : public SceneBase
{

public:

	// �R���X�g���N�^
	BossAppearanceScene();

	// �f�X�g���N�^
	~BossAppearanceScene()override = default;

	void Init() override;
	void Update(const float deltaTime) override;
	void Draw(const float deltaTime) override;

	// �{�X�o��p�̃{�X�݂̂̐����N���X���擾
	const std::shared_ptr<BossAppearanceCreate>& GetActorCreate() const { return actorCreate_; }

private:

	// �{�X�o��p�̃{�X�݂̂̐����N���X
	std::shared_ptr<BossAppearanceCreate> actorCreate_;

	// BGM��SE�̏�����
	void InitBGMAndSE()override;

};

