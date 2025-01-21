#pragma once
#include "SceneBase.h"

class GameClearActorCreate;

class GameClearScene : public SceneBase
{

public:

	// �R���X�g���N�^
	GameClearScene();

	// �f�X�g���N�^
	//~GameClearScene() override = default;
	~GameClearScene() override
	{
		int a = actorManager_.use_count();
		int b = actorCreate_.use_count();
	}

	void Init() override;
	void Update(const float deltaTime) override;
	void Draw(const float deltaTime) override;

	// �A�N�^�[�̊Ǘ��N���X���擾
	const std::shared_ptr<ActorManager>& GetActorManager() const { return actorManager_; }

	// �{�X�o��p�̃{�X�݂̂̐����N���X���擾
	const std::shared_ptr<GameClearActorCreate>& GetActorCreate() const { return actorCreate_; }

private:

	// �A�N�^�[�̊Ǘ��N���X
	std::shared_ptr<ActorManager> actorManager_;

	// �^�C�g���V�[���̃A�N�^�[�����N���X
	std::shared_ptr<GameClearActorCreate> actorCreate_;

	// �Q�[���N���A�̉摜
	int gameClearImg_;

	// Press A Button or Space�̉摜
	int pressAButtonOrSpaceImg_;

	// Press A Button or Space�p�̃J�E���^
	int frameCount_;

	// �摜�̏�����
	void InitImage()override;

	// BGM��SE�̏�����
	void InitBGMAndSE()override;


};

