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

private:

	// �A�N�^�[�̐����N���X
	std::shared_ptr<BossBattleActorCreate> actorCreate_;

	// BGM��SE�̏�����
	void InitBGMAndSE()override;

};

