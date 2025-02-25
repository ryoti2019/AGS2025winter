#pragma once
#include "SceneBase.h"

class ActorManager;
class BossAppearanceCreate;
class CollisionManager;

class BossAppearanceScene : public SceneBase
{

public:

	// �R���X�g���N�^
	BossAppearanceScene();

	// �f�X�g���N�^
	~BossAppearanceScene()override = default;

	void Init() override;
	void Update(const float deltaTime) override;
	void Draw(const float deltaTime) override;

	// �{�X�o��p�̃{�X�݂̂̐����N���X���擾
	const std::shared_ptr<BossAppearanceCreate>& GetActorCreate() const { return actorCreate_; }

private:

	// �T�E���h�̍ő剹��
	static constexpr int SOUND_MAX = 255;

	// BGM�̉��ʂ̊���
	static constexpr float SOUND_BGM_VOLUME = 0.5f;

	// �{�X�̖��O��`�悪�n�܂鎞��
	static constexpr float BOSS_NAME_DRAW_START_TIME = 6.0f;

	// �{�X�����ꂽ!!�p�̉摜�̃X�N���[��Y���W
	static constexpr int BOSS_APPEAR_TEXT_Y = 500;

	// 1���ڂ̉摜�̃A���t�@�l�̑�����
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

	// �{�X�o��p�̃{�X�݂̂̐����N���X
	std::shared_ptr<BossAppearanceCreate> actorCreate_;

	// �{�X�����ꂽ!!�p�̉摜
	int appearanceImg_;

	// �{�X�����ꂽ!!�p�̌�납��o�Ă���摜2
	int appearance2Img_;

	// 1���ڂ̉摜�̓����x
	int img1Alpha_;

	// 2���ڂ̉摜�̓����x
	int img2Alpha_;

	// 1���ڂ̉摜�̃X�P�[��
	float img1Scale_;

	// 2���ڂ̉摜�̃X�P�[��
	float img2Scale_;

	// �摜�̏�����
	void InitImage()override;

	// BGM��SE�̏�����
	void InitBGMAndSE()override;

};

