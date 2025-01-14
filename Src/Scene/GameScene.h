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
	~GameScene();

	void Init() override;
	void Update(const float deltaTime) override;
	void Draw(const float deltaTime) override;

	// �A�N�^�[�̐����N���X���擾
	const std::shared_ptr<ActorCreate>& GetActorCreate() const { return actorCreate_; }

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

	// ������������Ă��邩�̃t���O
	bool isViewUserGuide_;

	// �摜�̏�����
	void InitImage()override;

	// BGM��SE�̏�����
	void InitBGMAndSE()override;

};