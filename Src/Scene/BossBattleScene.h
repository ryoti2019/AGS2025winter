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

	// �A�N�^�[�̐����N���X
	std::shared_ptr<BossBattleActorCreate> actorCreate_;

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

