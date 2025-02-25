#pragma once
#include "../Scene/SceneBase.h"

class TitleActorCreate;

class TitleScene : public SceneBase
{

public:

	// �R���X�g���N�^
	TitleScene();

	// �f�X�g���N�^
	~TitleScene()override = default;

	void Init() override;
	void Update(const float deltaTime) override;
	void Draw(const float deltaTime) override;

	// �A�N�^�[�̊Ǘ��N���X���擾
	const std::shared_ptr<ActorManager>& GetActorManager() const { return actorManager_; }

	// �{�X�o��p�̃{�X�݂̂̐����N���X���擾
	const std::shared_ptr<TitleActorCreate>& GetActorCreate() const { return actorCreate_; }

private:

	// �T�E���h�̍ő剹��
	static constexpr int SOUND_MAX = 255;

	// BGM�̉��ʂ̊���
	static constexpr float SOUND_BGM_VOLUME = 0.5f;

	// �^�C�g�����S�̃X�N���[��X���W
	static constexpr int TITLE_LOGO_X = 850;

	// �^�C�g�����S�̃X�N���[��Y���W
	static constexpr int TITLE_LOGO_Y = 300;

	// �^�C�g�����S�̃X�P�[��
	static constexpr float TITLE_LOGO_SCALE = 1.0f;

	// YES��NO�摜�̃X�P�[���ω���
	static constexpr float SCALE_AMPLITUDE = 0.1f;

	// Press A Button or Space�̉摜�̃X�N���[��X���W
	static constexpr float PRESS_A_BUTTON_OR_SPACE_X = 850;

	// Press A Button or Space�̉摜�̃X�N���[��Y���W
	static constexpr float PRESS_A_BUTTON_OR_SPACE_Y = 500;

	// �A�N�^�[�̊Ǘ��N���X
	std::shared_ptr<ActorManager> actorManager_;

	// �^�C�g���V�[���̃A�N�^�[�����N���X
	std::shared_ptr<TitleActorCreate> actorCreate_;

	// �^�C�g�����S�̉摜
	int titleLogoImg_;

	// Press A Button or Space�̉摜
	int pressAButtonOrSpaceImg_;

	// Press A Button or Space�̃J�E���^
	int frameCount_;

	// �摜�̏�����
	void InitImage()override;

	// BGM��SE�̏�����
	void InitBGMAndSE()override;

};

