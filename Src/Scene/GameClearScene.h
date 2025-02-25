#pragma once
#include "SceneBase.h"

class GameClearActorCreate;

class GameClearScene : public SceneBase
{

public:

	// �R���X�g���N�^
	GameClearScene();

	// �f�X�g���N�^
	~GameClearScene()override = default;

	void Init() override;
	void Update(const float deltaTime) override;
	void Draw(const float deltaTime) override;

	// �A�N�^�[�̊Ǘ��N���X���擾
	const std::shared_ptr<ActorManager>& GetActorManager() const { return actorManager_; }

	// �{�X�o��p�̃{�X�݂̂̐����N���X���擾
	const std::shared_ptr<GameClearActorCreate>& GetActorCreate() const { return actorCreate_; }

private:

	// �T�E���h�̍ő剹��
	static constexpr int SOUND_MAX = 255;

	// BGM�̉��ʂ̊���
	static constexpr float SOUND_BGM_VOLUME = 0.5f;

	// �Q�[���N���A�摜�̃X�N���[��X���W
	static constexpr int GAME_CLEAR_X = 850;

	// �Q�[���N���A�摜�̃X�N���[��X���W
	static constexpr int GAME_CLEAR_Y = 200;
	
	// �Q�[���N���A�摜�̃X�P�[��
	static constexpr double GAME_CLEAR_SCALE = 2.0;

	// �Q�[���N���A�摜�̃A�j���[�V�����X�s�[�h
	static constexpr float GAME_CLEAR_ANIM_SPEED = 0.05f;

	// �Q�[���N���A�摜�̒ʏ�X�P�[��
	static constexpr float GAME_CLEAR_IMG_SCALE = 1.0f;

	// �Q�[���N���A�摜�̃X�P�[���ω���
	static constexpr float SCALE_AMPLITUDE = 0.1f;

	// Press A Button or Space�̉摜�̃X�N���[����X���W
	static constexpr float PRESS_A_BUTTON_OR_SPACE_X = 850;

	// Press A Button or Space�̉摜�̃X�N���[����Y���W
	static constexpr float PRESS_A_BUTTON_OR_SPACE_Y = 500;

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

