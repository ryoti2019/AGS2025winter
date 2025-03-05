#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "Manager/ResourceManager.h"
#include "Manager/InputManager.h"
#include "Manager/SceneManager.h"
#include "../Lib/ImGuiWrapper.h"
#include "../Common/FpsControl.h"
#include "Application.h"

Application* Application::instance_ = nullptr;

const std::string Application::PATH_IMAGE = "Data/Image/";
const std::string Application::PATH_MODEL = "Data/Model/";
const std::string Application::PATH_EFFECT = "Data/Effect/";
const std::string Application::PATH_SOUND = "Data/Sound/";
const std::string Application::PATH_SHADER = "Data/Shader/";
const std::string Application::PATH_JSON = "Data/Json/";

void Application::CreateInstance()
{
	if (instance_ == nullptr)
	{
		instance_ = new Application();
	}
	//instance_->Init();
}

Application& Application::GetInstance()
{
	return *instance_;
}

void Application::Init()
{
}

void Application::Run()
{

	auto& inputManager = InputManager::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();
	auto& imGuiWrapper = ImGuiWrapper::GetInstance();
	auto& fpsControl = FpsControl::GetInstance();

	// �Q�[�����[�v
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{

		//�X�V
		fpsControl.Update();

		// �{�Ԃ͏�ɂ���
		// �f���^�^�C��
		auto nowTime = std::chrono::system_clock::now();
		deltaTime_ = static_cast<float>(
			std::chrono::duration_cast<std::chrono::nanoseconds>(nowTime - preTime_).count() / 1000000000.0);
			
		preTime_ = nowTime;

		inputManager.Update();
		imGuiWrapper.Update();
		sceneManager.Update(deltaTime_);

		sceneManager.Draw(deltaTime_);

		// ���_�o�b�t�@�ɗ��܂������_�f�[�^��`�悷��
		// DxLib�͓�������̂��ƁA�܂Ƃ߂ĕ`�悷��@�\������
		// ���̂��߁AScreenFlip�֐��ŗ��܂��Ă���DxLib���̕`�悪���s����A
		// imGui�̕`�悪�㏑������Ă��܂��ꍇ������
		// ����𖳂������߂ɁARenderVertex�֐��ŁA���ߕ���`�悵�Ă���
		// �������x�I�ɂ͂�����ƌ��O
		RenderVertex();

		imGuiWrapper.Draw();

		ScreenFlip();

		//�ҋ@
		fpsControl.Wait();
	}

}

void Application::Destroy()
{

	InputManager::GetInstance().Destroy();
	ResourceManager::GetInstance().Destroy();
	SceneManager::GetInstance().Destroy();
	ImGuiWrapper::GetInstance().Destroy();

	Effkseer_End();

	// DxLib�I��
	if (DxLib_End() == -1)
	{
		isReleaseFail_ = true;
	}

	delete instance_;

}

Application::Application()
	:
	isInitFail_(false),
	isReleaseFail_(false),
	preTime_(std::chrono::system_clock::now()),
	deltaTime_(0.0f)
{

	// �A�v���P�[�V�����̏����ݒ�
	SetWindowText("AGS2025winter");

	// �E�B���h�E�T�C�Y
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, COLOR_BIT_DEPTH);
	ChangeWindowMode(true);

	// DxLib�̏�����
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	isInitFail_ = false;
	if (DxLib_Init() == -1)
	{
		isInitFail_ = true;
		return;
	}

	// Effekseer�̏�����
	if (Effekseer_Init(EFFEKSEER_MAX_EFFECTS) == -1)
	{
		DxLib_End();
	}
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

	// �L�[���䏉����
	SetUseDirectInputFlag(true);
	InputManager::CreateInstance();

	// ���\�[�X�Ǘ�������
	ResourceManager::CreateInstance();

	// �V�[���Ǘ�������
	SceneManager::CreateInstance();

	// �f�o�b�O�`�揉����
	ImGuiWrapper::CreateInstance();

	//fps����
	FpsControl::CreateInstance();

}