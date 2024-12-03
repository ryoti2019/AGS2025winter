#pragma once
#include "Component.h"
#include "../Object/Common/Transform.h"

class Player;

class MoveComponent : public Component
{

public:

	MoveComponent(std::shared_ptr<ActorBase> actor);

	// ‘–‚éˆÚ“®
	void Move();

	// ‚ä‚Á‚­‚èˆÚ“®
	void Lerp();

	// ã•ûŒü‚É”ò‚ñ‚Å‚¢‚­ˆÚ“®
	void HitMove();

private:

};

