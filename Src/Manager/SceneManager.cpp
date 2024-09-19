#include <chrono>
#include <DxLib.h>
#include "ResourceManager.h"
#include "SceneManager.h"
#include "../Scene/SceneBase.h"
#include "../Scene/TitleScene.h"
#include "../Scene/GameScene.h"

SceneManager* SceneManager::instance_ = nullptr;

void SceneManager::CreateInstance()
{
	if (instance_ == nullptr)
	{
		instance_ = new SceneManager();
	}
	instance_->Init();
}

void SceneManager::Init()
{

	// �V�[��ID
	sceneId_ = SCENE_ID::TITLE;
	waitSceneId_ = SCENE_ID::NONE;

	//�֐��|�C���^�̏�����
	sceneChange_.emplace(SCENE_ID::TITLE, std::bind(&SceneManager::ChangeTitleScene, this));
	sceneChange_.emplace(SCENE_ID::GAME, std::bind(&SceneManager::ChangeGameScene, this));
	sceneChange_.emplace(SCENE_ID::GAMEOVER, std::bind(&SceneManager::ChangeGameOverScene, this));

	// ������
	scene_ = std::make_unique<TitleScene>();
	scene_->Init();

	// �V�[���J�ڒ�����
	isSceneChanging_ = false;

	// �����V�[���̐ݒ�
	DoChangeScene(SCENE_ID::TITLE);

	// �Q�[���p�b�h���g���Ƃ�true
	isGamePad_ = false;

	// ��������̃t���O
	isOperation_ = true;

}

void SceneManager::Update(const float deltaTime)
{

	// null�̏ꍇ�͒ʂ�Ȃ�
	if (scene_ == nullptr)
	{
		return;
	}

	// �V�[���J�ڈȊO�͍X�V
	if (!isSceneChanging_)
	{
		scene_->Update(deltaTime);
	}

}

void SceneManager::Draw()
{

	// �`���O���t�B�b�N�̈�̎w��
	// (�R�c�`��Ŏg�p����J�����̐ݒ�Ȃǂ����Z�b�g�����)
	SetDrawScreen(DX_SCREEN_BACK);

	// ��ʂ�������
	ClearDrawScreen();

	// �`��
	scene_->Draw();

}

void SceneManager::Destroy()
{
	delete instance_;
}

SceneManager::SceneManager()
	:
	sceneId_(SCENE_ID::NONE),
	waitSceneId_(SCENE_ID::NONE),
	scene_(nullptr),
	isSceneChanging_(false),
	isGamePad_(false),
	isOperation_(false)
{
}

void SceneManager::DoChangeScene(const SCENE_ID& sceneId)
{

	// ���\�[�X�̉��
	ResourceManager::GetInstance().Release();

	// �V�[����ύX����
	sceneId_ = sceneId;

	//// ���݂̃V�[�������
	//if (scene_ != nullptr)
	//{
	//	scene_->Release();
	//}

	// �V�[����ύX
	sceneChange_[sceneId_]();

	// �V�[����������
	scene_->Init();

	// �ҋ@���̃V�[��ID��������
	waitSceneId_ = SCENE_ID::NONE;

}

void SceneManager::ChangeTitleScene()
{
	scene_ = std::make_unique<TitleScene>();
}

void SceneManager::ChangeGameScene()
{
	scene_ = std::make_unique<GameScene>();
}

void SceneManager::ChangeGameOverScene()
{
}
