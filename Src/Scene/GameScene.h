#pragma once
#include "../Scene/SceneBase.h"

class GameScene :public SceneBase
{

public:

	// コンストラクタ
	GameScene();

	// デストラクタ
	~GameScene();

	void Init() override;
	void Update(const float deltaTime) override;
	void Draw() override;

private:

};