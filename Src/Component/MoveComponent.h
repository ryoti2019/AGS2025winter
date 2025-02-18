#pragma once
#include "Component.h"

class MoveComponent : public Component
{

public:

	MoveComponent(std::weak_ptr<ActorBase> actor);

	// ‘–‚éˆÚ“®
	void Move();

	// ‚ä‚Á‚­‚èˆÚ“®
	void Lerp();

	// ã•ûŒü‚É”ò‚ñ‚Å‚¢‚­ˆÚ“®
	void HitMove();

private:

	// ˆÚ“®—Ê
	static constexpr float MOVE_SMOOTHNESS = 0.1f;

};

