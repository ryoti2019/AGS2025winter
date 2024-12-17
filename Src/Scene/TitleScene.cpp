#include <DxLib.h>
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "TitleScene.h"

TitleScene::TitleScene()
{
}

void TitleScene::Init()
{
}

void TitleScene::Update(const float deltaTime)
{

	InputManager& ins = InputManager::GetInstance();

	// ÉVÅ[ÉìëJà⁄
	if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SCENE_ID::GAME);
	}
	else if (ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT))
	{
		SceneManager::GetInstance().ChangeScene(SCENE_ID::GAME);
		SceneManager::GetInstance().SetGamePad(true);
	}

}

void TitleScene::Draw(const float deltaTime)
{
	DrawFormatString(0, 0, 0xff0000, "TitleScene");
}