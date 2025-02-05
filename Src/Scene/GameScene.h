#pragma once
#include <memory>
#include "../Scene/SceneBase.h"

class ActorCreate;

class GameScene :public SceneBase
{

public:

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