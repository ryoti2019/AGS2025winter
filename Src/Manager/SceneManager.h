#pragma once
#include <functional>
#include <memory>
#include <unordered_map>

class Fader;
class Camera;
class SceneBase;
class GameScene;
class ActorManager;

// �V�[���Ǘ��p
enum class SCENE_ID
{
	NONE,
	TITLE,
	GAME,
	BOSS_APPEARANCE,
	GAMEOVER,
};

class SceneManager
{

public:

	// �C���X�^���X�̐���
	static void CreateInstance();

	// �C���X�^���X�̎擾
	static SceneManager& GetInstance() { return *instance_; };

	void Init();
	void Init3D();
	void Update(const float deltaTime);
	void Draw(const float deltaTime);

	// ���\�[�X�̔j��
	void Destroy();

	// ��ԑJ��
	void ChangeScene(const SCENE_ID& nextId);

	// �J�����̎擾
	std::weak_ptr<Camera> GetCamera(void) const { return camera_; };

	// �V�[��ID�̎擾
	const SCENE_ID& GetSceneID() const { return sceneId_; };

	// �V�[���J��
	void DoChangeScene(const SCENE_ID& sceneId);

	// ���̃V�[�����擾
	const std::shared_ptr<SceneBase>& GetNowScene() const { return scene_; };

	// �Q�[���p�b�h�ő��삷��t���O���擾����
	bool GetGamePad(void);

	// �Q�[���p�b�h�ő��삷��t���O���擾����
	void SetGamePad(bool isPad);

private:

	// �ÓI�C���X�^���X
	static SceneManager* instance_;

	// �V�[��ID 
	SCENE_ID sceneId_;

	// �ҋ@���̃V�[��ID
	SCENE_ID waitSceneId_;

	// �t�F�[�h
	std::unique_ptr<Fader> fader_;

	// �J����
	std::shared_ptr<Camera> camera_;

	// �V�[���̊��N���X
	std::shared_ptr<SceneBase> scene_;

	// ��ԑJ��
	std::unordered_map<SCENE_ID, std::function<void(void)>> sceneChange_;
	void ChangeTitleScene();
	void ChangeGameScene();
	void ChangeBossAppearanceScene();
	//void ChangeGameOverScene();

	// �V�[���J�ڒ�����
	bool isSceneChanging_;

	// �Q�[���p�b�h�ő��삷�邽�߂̃t���O
	bool isGamePad_;

	// ��������̃t���O
	bool isOperation_;

	// �f�t�H���g�R���X�g���N�^��private�ɂ��āA
	// �O�����琶���ł��Ȃ��l�ɂ���
	SceneManager();
	// �R�s�[�R���X�g���N�^�����l
	SceneManager(const SceneManager&);
	// �f�X�g���N�^�����l
	~SceneManager() = default;

	// �t�F�[�h
	void Fade(void);

};