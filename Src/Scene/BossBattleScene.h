#pragma once
#include "SceneBase.h"

class BossBattleActorCreate;

class BossBattleScene : public SceneBase
{

public:

	// �R���X�g���N�^
	BossBattleScene();

	// �f�X�g���N�^
	~BossBattleScene()override = default;

	void Init() override;
	void Update(const float deltaTime) override;
	void Draw(const float deltaTime) override;

	// �A�N�^�[�̐����N���X���擾
	const std::shared_ptr<BossBattleActorCreate>& GetActorCreate() const { return actorCreate_; }

	// �v���C���[�����񂾂Ƃ��̃t���O���擾
	const bool GetIsPlayerDeath() const { return isPlayerDeath_; }

	// �v���C���[�����񂾂Ƃ��̃t���O��ݒ�
	void SetIsPlayerDeath(const bool isPlayerDeath) { isPlayerDeath_ = isPlayerDeath; }

private:

	// �T�E���h�̍ő剹��
	static constexpr int SOUND_MAX = 255;

	// BGM�̉��ʂ̊���
	static constexpr float SOUND_BGM_VOLUME = 0.5f;

	// �Q�[���I�[�o�[�ɑJ�ڂ���܂ł̎���
	static constexpr float GAME_OVER_TRANSITION_TIME = 5.0f;

	// �Q�[���I�[�o�[�̎��̌��̉�ʂ̓����x
	static constexpr int BOSS_BATTLE_ALPHA = 128;

	// �Q�[���I�[�o�[�̉摜�̃X�P�[��
	static constexpr double BOSS_BATTLE_IMG_SCALE = 1.0;

	// 1���ڂ̉摜�̓����x�̑�����
	static constexpr int IMG1_ALPHA_INCREMENT = 2;

	// 1���ڂ̉摜�̃A���t�@�̍ő�l
	static constexpr int IMG1_ALPHA_MAX = 255;

	// 1���ڂ̉摜�̃X�P�[���̌�����
	static constexpr float IMG1_SCALE_DECREASE = 0.02f;

	// 1���ڂ̉摜�̃X�P�[���̍ŏ��l
	static constexpr float IMG1_SCALE_MIN = 1.0f;

	// 2���ڂ̉摜�̃X�P�[���̍ő�l
	static constexpr float IMG2_SCALE_MAX = 3.0f;

	// 2���ڂ̉摜�̃A���t�@�l�̌�����
	static constexpr int IMG2_ALPHA_DECREASE = 10;

	// 2���ڂ̉摜�̃X�P�[���̌�����
	static constexpr float IMG2_SCALE_INCREMENT = 0.05f;

	// �R���e�B�j���[�摜�̃X�N���[��Y���W
	static constexpr int CONTINUE_IMG_Y = 200;

	// �R���e�B�j���[�摜�̃X�P�[��
	static constexpr double CONTINUE_SCALE = 0.8;

	// YES��NO�摜�̒ʏ�X�P�[��
	static constexpr float YES_NO_IMG_SCALE = 1.0f;

	// YES��NO�摜�̃A�j���[�V�����X�s�[�h
	static constexpr float YES_NO_IMG_ANIM_SPEED = 0.05f;

	// YES��NO�摜�̃X�P�[���ω���
	static constexpr float SCALE_AMPLITUDE = 0.1f;

	// YES��NO�摜�̃X�N���[��X���W�̃I�t�Z�b�g
	static constexpr int YES_NO_IMG_X_OFFSET = 300;

	// YES��NO�摜�̃X�N���[��Y���W
	static constexpr int YES_NO_IMG_Y = 500;

	// �A�N�^�[�̐����N���X
	std::shared_ptr<BossBattleActorCreate> actorCreate_;

	// �Q�[���I�[�o�[�̉摜
	int gameOverImg_;

	// �Q�[���I�[�o�[�̉摜2
	int gameOverImg2_;

	// �摜�̓����x
	int img1Alpha_;

	// 2���ڂ̉摜�̓����x
	int img2Alpha_;

	// �摜�̃X�P�[��
	float img1Scale_;

	// 2���ڂ̉摜�̃X�P�[��
	float img2Scale_;

	// �R���e�B�j���[�̉摜
	int continueImg_;

	// Yes�̉摜
	int yesImg_;

	// No�̉摜
	int noImg_;

	// �摜�̃J�E���^
	int frameCount_;

	// �Q�[���I�[�o�[�{�C�X
	int gameOverVoice_;

	// �J�[�\���ړ���SE
	int cursorMoveSE_;

	// �Q�[���I�[�o�[��\���������邽�߂̃J�E���^
	float gameOverCnt_;

	// �v���C���[�����񂾂Ƃ��̃t���O
	bool isPlayerDeath_;

	// �R���e�B�j���[���邩�̃t���O
	bool isContinue_;

	// �Q�[���I�[�o�[�̃t���O
	bool isGameOver_;

	// �摜�̏�����
	void InitImage()override;

	// BGM��SE�̏�����
	void InitBGMAndSE()override;

	// �Q�[���N���A�V�[���ɑJ�ڂ��邩����
	void CheckTransitionGameClearScene();

	// �R���e�B�j���[���Q�[���I�[�o�[��I�΂��鏈��
	void SelectContinueOrGameOver(const float deltaTime);

	// �Q�[���I�[�o�[�̕`��
	void DrawGameOver();

};

