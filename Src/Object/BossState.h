#pragma once

enum class BossState
{
	NONE,
	IDLE,
	RUN,
	ATTACK_PUNCH,
	ATTACK_KICK,
	ATTACK_PROJECTILE,
	HIT_HEAD,
	HIT_BODY,
	HIT_FLY,
	HIT_FLINCH_UP,
	HIT_KNOCK_BACK,
	KIP_UP,
	JUMP_IDLE,
	LAND,
	CALL,
	DEATH,
	MAX
};