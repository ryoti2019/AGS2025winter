#pragma once
#include <DxLib.h>
class ActorBase
{

public:

	ActorBase() = default;

	virtual ~ActorBase() = default;

	virtual void Init(const VECTOR& pos);
	virtual void Create(const VECTOR& pos);
	virtual void Update(const float deltaTime);
	virtual void Draw();

protected:

private:

};