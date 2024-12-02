#pragma once
#include <memory>
#include <map>
class ActorBase;

class Component
{

public:

	Component();

	// �A�N�^�[���擾����
	const std::shared_ptr<ActorBase>& GetActor()const;

	virtual bool IsDead()const { return false; }
	virtual void Init() {};
	virtual void Update() {};
	virtual void Draw() {};

protected:

	std::shared_ptr<ActorBase> actor_;

};
