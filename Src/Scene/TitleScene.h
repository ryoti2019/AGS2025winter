#pragma once
#include "../Scene/SceneBase.h"

class TitleActorCreate;

class TitleScene : public SceneBase
{

public:

	// �R���X�g���N�^
	TitleScene();

	// �f�X�g���N�^
	~TitleScene() = default;

	void Init() override;
	void Update(const float deltaTime) override;
	void Draw(const float deltaTime) override;

	// �A�N�^�[�̊Ǘ��N���X���擾
	const std::shared_ptr<ActorManager>& GetActorManager() const { return actorManager_; }

	// �{�X�o��p�̃{�X�݂̂̐����N���X���擾
	const std::shared_ptr<TitleActorCreate>& GetActorCreate() const { return actorCreate_; }

private:

	// �A�N�^�[�̊Ǘ��N���X
	std::shared_ptr<ActorManager> actorManager_;

	// �^�C�g���V�[���̃A�N�^�[�����N���X
	std::shared_ptr<TitleActorCreate> actorCreate_;

	// �^�C�g�����S�̉摜
	int titleLogoImg_;

	// Press A Botton�̉摜
	int pressABottonImg_;

	// Press A Botton�p�̃J�E���^
	int frameCount_;

	// �摜�̏�����
	void InitImage()override;

	// BGM��SE�̏�����
	void InitBGMAndSE()override;

};

