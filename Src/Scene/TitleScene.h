#pragma once
#include "../Scene/SceneBase.h"

class TitleScene : public SceneBase
{

public:

	// コンストラクタ
	TitleScene();

	// デストラクタ
	~TitleScene() = default;

	void Init() override;
	void Update(const float deltaTime) override;
	void Draw() override;

private:

};

