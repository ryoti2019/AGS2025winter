#pragma once
#include <memory>
#include "../Scene/SceneBase.h"

class ActorCreate;

class GameScene :public SceneBase
{

public:

	// 1���ڂ̉摜�X�P�[��
	static constexpr float SCALE = 2.0f;

	// 2���ڂ̉摜�X�P�[��
	static constexpr float SCALE2 = 1.0f;

	// 2���ڂ̉摜�̓����x
	static constexpr int ALPHA_MAX = 255;

	// ���̍ő剹��
	static constexpr int SOUND_MAX = 255;

	// BGM�̉��ʂ̊���
	static constexpr float BGM_VOLUME = 0.5f;

	// �Q�[���I�[�o�[�ɑJ�ڂ���܂ł̎���
	static constexpr float GAME_OVER_DELAY = 5.0f;

	// ��������̎��̌��̉�ʂ̓����x
	static constexpr int USER_GUIDE_ALPHA = 200;

	// ��������摜�̃X�P�[��
	static constexpr float USER_GUIDE_IMAGE_SCALE = 0.5f;

	// ���������X���W
	static constexpr int USER_GUIDE_POS_X = 150;

	// ���������Y���W
	static constexpr int USER_GUIDE_POS_Y = 200;

	// �Q�[���I�[�o�[�̎��̌��̉�ʂ̓����x
	static constexpr int GAME_OVER_ALPHA = 128;

	// �摜1�̓����x�̑������
	static constexpr int ALPHA_INCREMENT = 2;

	// �摜1�̓����x�̌����
	static constexpr float ALPHA_DECREMENRT = -0.02f;

	// �摜1�̃X�P�[���̍ő�l
	static constexpr float SCALE_MAX = 1.0f;

	// �摜2�̃X�P�[���̍ő�l
	static constexpr float SCALE2_MAX = 3.0f;

	// �摜1�̓����x�̑������
	static constexpr float ALPHA2_INCREMENT = 0.05f;

	// �摜1�̓����x�̌����
	static constexpr int ALPHA2_DECREMENRT = -10;

	// �R���e�B�j���[�摜��Y���W
	static constexpr int CONTINUE_POS_Y = 200;

	// �R���e�B�j���[�摜�̃X�P�[��
	static constexpr float CONTINUE_IMAGE_SCALE = 0.8f;

	// YES�摜�̃X�P�[��
	static constexpr float YES_IMAGE_SCALE = 1.0f;

	// NO�摜�̃X�P�[��
	static constexpr float NO_IMAGE_SCALE = 1.0f;

	// YES�摜��X���W�̃I�t�Z�b�g
	static constexpr int YES_POS_X_OFFSET = 300;

	// NO�摜��X���W�̃I�t�Z�b�g
	static constexpr int NO_POS_X_OFFSET = 300;

	// YES�摜��Y���W
	static constexpr int YES_POS_Y = 500;

	// NO�摜��Y���W
	static constexpr int NO_POS_Y = 500;

	// YES��NO�摜�̃X�P�[���̕ϓ���
	static constexpr float SCLAE_VARIATION = 0.1f;

	// YES��NO�摜�̃A�j���[�V�������x
	static constexpr float ANIM_SPEED = 0.05f;

	// �R���X�g���N�^
	GameScene();

	// �f�X�g���N�^
	~GameScene()override = default;

	void Init() override;
	void Update(const float deltaTime) override;
	void Draw(const float deltaTime) override;

	// �A�N�^�[�̐����N���X���擾
	const std::shared_ptr<ActorCreate>& GetActorCreate() const { return actorCreate_; }

	// ������������Ă��邩�̃t���O
	const bool GetIsViewUserGuide()const { return isViewUserGuide_; }

	// �v���C���[�����񂾂Ƃ��̃t���O���擾
	const bool GetIsPlayerDeath() const { return isPlayerDeath_; }

	// �v���C���[�����񂾂Ƃ��̃t���O��ݒ�
	void SetIsPlayerDeath(const bool isPlayerDeath) { isPlayerDeath_ = isPlayerDeath; }

private:

	// �A�N�^�[�̐����N���X
	std::shared_ptr<ActorCreate> actorCreate_;

	// �L�[�{�[�h�̑������
	int keyboardUserGuideImg_;

	// �L�[�{�[�h�̈ړ��̑������
	int keyboardUserGuideMoveImg_;

	// �L�[�{�[�h�̍U���̑������
	int keyboardUserGuideAttackImg_;

	// �L�[�{�[�h�̕K�E�Z�̑������
	int keyboardUserGuideSpecialAttackImg_;

	// �Q�[���p�b�h�̑������
	int gamePadUserGuideImg_;

	// �Q�[���p�b�h�̈ړ��̑������
	int gamePadUserGuideMoveImg_;

	// �Q�[���p�b�h�̍U���̑������
	int gamePadUserGuideAttackImg_;

	// �Q�[���p�b�h�̕K�E�Z�̑������
	int gamePadUserGuideSpecialAttackImg_;

	// �Q�[���I�[�o�[�̉摜
	int gameOverImg_;

	// �Q�[���I�[�o�[�̉摜2
	int gameOverImg2_;

	// �摜�̓����x
	int alpha_;

	// 2���ڂ̉摜�̓����x
	int alpha2_;

	// �摜�̃X�P�[��
	float scale_;

	// 2���ڂ̉摜�̃X�P�[��
	float scale2_;

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

	// ������������Ă��邩�̃t���O
	bool isViewUserGuide_;

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

	// �{�X�̓o��V�[���ɑJ�ڂ��邩����
	void CheckTransitionBossAppearanceScene();

	// �R���e�B�j���[���Q�[���I�[�o�[��I�΂��鏈��
	void SelectContinueOrGameOver(const float deltaTime);

	// ��������̕`��
	void DrawUserGuide();
	
	// �Q�[���I�[�o�[�̕`��
	void DrawGameOver();

};