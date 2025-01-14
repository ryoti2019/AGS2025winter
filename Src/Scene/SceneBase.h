#pragma once
#include <memory>
#include "../Manager/Camera.h"
#include "../Manager/ResourceManager.h"
#include "../Object/Common/InputController.h"

class ActorManager;
class CollisionManager;

class SceneBase
{

public:

	// �R���X�g���N�^
	SceneBase();

	// �f�X�g���N�^
	virtual ~SceneBase() = 0;

	// ����������
	virtual void Init() = 0;

	// �X�V�X�e�b�v
	virtual void Update(const float deltaTime) = 0;

	// �`�揈��
	virtual void Draw(const float deltaTime) = 0;

	// �A�N�^�[�̊Ǘ��N���X���擾
	const std::shared_ptr<ActorManager>& GetActorManager() const { return actorManager_; }

	// �Փ˔���̊Ǘ��N���X���擾
	const std::shared_ptr<CollisionManager>& GetCollisionManager() const { return collisionManager_; }

protected:

	// ���\�[�X�Ǘ�
	ResourceManager& resMng_;

	//�J����
	std::unique_ptr<Camera> camera_;

	// �A�N�^�[�̊Ǘ��N���X
	std::shared_ptr<ActorManager> actorManager_;

	// �Փ˔���̊Ǘ��N���X
	std::shared_ptr<CollisionManager> collisionManager_;

	// ���͗p�R���g���[���[
	std::unique_ptr<InputController> inputController_;

	// BGM
	int bgm_;

	// ����{�^����SE
	int decideSE_;

	// �摜�̏�����
	virtual void InitImage();

	// BGM��SE�̏�����
	virtual void InitBGMAndSE();

private:

};