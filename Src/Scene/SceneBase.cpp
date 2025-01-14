#include "SceneBase.h"

SceneBase::SceneBase()
	:
	resMng_(ResourceManager::GetInstance())
{
}

SceneBase::~SceneBase()
{
}

void SceneBase::Init()
{
}

void SceneBase::Update(const float deltaTime)
{
}

void SceneBase::Draw(const float deltaTime)
{
}

void SceneBase::InitImage()
{
}

void SceneBase::InitBGMAndSE()
{
}
