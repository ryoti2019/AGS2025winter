#include <chrono>
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "ResourceManager.h"
#include "../Common/Fader.h"
#include "Camera.h"
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
	//sceneChange_.emplace(SCENE_ID::GAMEOVER, std::bind(&SceneManager::ChangeGameOverScene, this));

	// �t�F�[�_�[�̏�����
	fader_ = std::make_unique<Fader>();
	
	// �J�����̏�����
	camera_ = std::make_shared<Camera>();

	// �V�[���̏�����
	scene_ = std::make_unique<TitleScene>();
	scene_->Init();

	// �V�[���J�ڒ�����
	isSceneChanging_ = false;

	// 3D�p�̐ݒ�
	Init3D();

	// �����V�[���̐ݒ�
	DoChangeScene(SCENE_ID::TITLE);

	// �Q�[���p�b�h���g���Ƃ�true
	isGamePad_ = false;

	// ��������̃t���O
	isOperation_ = true;

}

void SceneManager::Init3D()
{

	// �w�i�F�ݒ�
	SetBackgroundColor(255, 255, 255);

	// Z�o�b�t�@��L���ɂ���
	SetUseZBuffer3D(true);

	// Z�o�b�t�@�ւ̏������݂�L���ɂ���
	SetWriteZBuffer3D(true);

	// �o�b�N�J�����O��L���ɂ���
	SetUseBackCulling(true);

	// ���C�g�̐ݒ�
	SetUseLighting(true);

	// ���ʂ���΂߉��Ɍ����������C�g
	ChangeLightTypeDir({ 0.0f, -1.0f, 1.0f });

}

void SceneManager::Update(const float deltaTime)
{

	// null�̏ꍇ�͒ʂ�Ȃ�
	if (scene_ == nullptr)return;
	if (fader_ == nullptr)return;

	// �V�[���J�ڈȊO�͍X�V
	fader_->Update();
	if (isSceneChanging_)
	{
		Fade();
	}
	else
	{
 		scene_->Update(deltaTime);
	}

	// �J�����X�V
	camera_->Update();

	// Effekseer�ɂ��Đ����̃G�t�F�N�g���X�V����B
	UpdateEffekseer3D();

}

void SceneManager::Draw(const float deltaTime)
{

	// �`���O���t�B�b�N�̈�̎w��
	// (�R�c�`��Ŏg�p����J�����̐ݒ�Ȃǂ����Z�b�g�����)
	SetDrawScreen(DX_SCREEN_BACK);

	// ��ʂ�������
	ClearDrawScreen();

	// �J�����ݒ�
	camera_->SetBeforeDraw(deltaTime);

	// �`��
	scene_->Draw(deltaTime);

	// Effekseer�ɂ��Đ����̃G�t�F�N�g��`�悷��B
	DrawEffekseer3D();

	// �Ó]�E���]
	fader_->Draw();

}

void SceneManager::Destroy()
{
	delete instance_;
}

void SceneManager::ChangeScene(const SCENE_ID& nextId)
{

	// �t�F�[�h�������I����Ă���V�[����ς���ꍇ�����邽�߁A
	// �J�ڐ�V�[���������o�ϐ��ɕێ�
	waitSceneId_ = nextId;

	// �t�F�[�h�A�E�g(�Ó])���J�n����
	fader_->SetFade(Fader::STATE::FADE_OUT);
	isSceneChanging_ = true;

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

void SceneManager::Fade(void)
{

	Fader::STATE fState = fader_->GetState();
	switch (fState)
	{
	case Fader::STATE::FADE_IN:
		// ���]��
		if (fader_->IsEnd())
		{
			// ���]���I��������A�t�F�[�h�����I��
			fader_->SetFade(Fader::STATE::NONE);
			isSceneChanging_ = false;
		}
		break;
	case Fader::STATE::FADE_OUT:
		// �Ó]��
		if (fader_->IsEnd())
		{
			// ���S�ɈÓ]���Ă���V�[���J��
			DoChangeScene(waitSceneId_);
			// �Ó]���疾�]��
			fader_->SetFade(Fader::STATE::FADE_IN);
		}
		break;
	}

}

void SceneManager::DoChangeScene(const SCENE_ID& sceneId)
{

	// ���\�[�X�̉��
	ResourceManager::GetInstance().Release();

	// �V�[����ύX����
	sceneId_ = sceneId;

	// �V�[����ύX
	sceneChange_[sceneId_]();

	// �V�[����������
	scene_->Init();

	// �ҋ@���̃V�[��ID��������
	waitSceneId_ = SCENE_ID::NONE;

}

bool SceneManager::GetGamePad(void)
{
	return isGamePad_;
}

void SceneManager::SetGamePad(bool isPad)
{
	isGamePad_ = isPad;
}

void SceneManager::ChangeTitleScene()
{
	scene_ = std::make_unique<TitleScene>();
}

void SceneManager::ChangeGameScene()
{
	scene_ = std::make_unique<GameScene>();
}