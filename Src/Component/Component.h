#pragma once
#include <memory>
class ActorBase;

class Component
{

public:

	Component(std::shared_ptr<ActorBase> actor);

	// �A�N�^�[���擾����
	const std::shared_ptr<ActorBase>& GetActor()const;

	virtual bool IsDead()const { return false; }
	virtual void Init() {};
	virtual void Update() = 0;
	virtual void Draw() {};

protected:

	std::shared_ptr<ActorBase> actor_;

};