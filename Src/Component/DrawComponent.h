#pragma once
#include "Component.h"

class DrawComponent : public Component
{

public:

	DrawComponent(std::weak_ptr<ActorBase> actor);

	void Update()override;

private:

	// デバッグ描画
	void DrawDebug();

};

