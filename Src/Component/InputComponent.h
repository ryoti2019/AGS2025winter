#pragma once
#include "Component.h"
#include "../Object/Common/InputController.h"

class InputComponent : public Component
{

public:

	InputComponent(std::shared_ptr<ActorBase> actor);
	virtual ~InputComponent();

	virtual void Update()override;

private:

	// ���͗p�R���g���[���[
	std::unique_ptr<InputController> inputController_;

};

