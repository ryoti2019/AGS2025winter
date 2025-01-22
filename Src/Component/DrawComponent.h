#pragma once
#include "Component.h"

class DrawComponent : public Component
{

public:

	DrawComponent(std::shared_ptr<ActorBase> actor);

	void Update()override;

private:

	// �f�o�b�O�`��
	void DrawDebug();

};

