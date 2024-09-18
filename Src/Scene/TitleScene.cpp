#include <DxLib.h>
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "TitleScene.h"

TitleScene::TitleScene()
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::Init()
{
}

void TitleScene::Update(const float deltaTime)
{
	if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().DoChangeScene(SCENE_ID::GAME);
	}
}

void TitleScene::Draw()
{
	DrawFormatString(0, 0, 0xff0000, "TitleScene");
}