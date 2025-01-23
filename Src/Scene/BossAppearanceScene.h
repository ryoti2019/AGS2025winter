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

	// �{�X�����ꂽ!!�p�̉摜
	int appearanceImg_;

	// �{�X�����ꂽ!!�p�̌�납��o�Ă���摜2
	int appearance2Img_;

	// �摜�̓����x
	int alpha_;

	// 2���ڂ̉摜�̓����x
	int alpha2_;

	// �摜�̃X�P�[��
	float scale_;

	// 2���ڂ̉摜�̃X�P�[��
	float scale2_;

	// �摜�̏�����
	void InitImage()override;

	// BGM��SE�̏�����
	void InitBGMAndSE()override;

};

