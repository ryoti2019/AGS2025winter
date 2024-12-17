#pragma once
#include "SceneBase.h"
class BossAppearanceScene : public SceneBase
{

public:

	BossAppearanceScene();

	~BossAppearanceScene() = default;

	void Init() override;
	void Update(const float deltaTime) override;
	void Draw(const float deltaTime) override;

private:


};

