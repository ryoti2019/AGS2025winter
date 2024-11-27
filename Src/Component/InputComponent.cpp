#include "InputComponent.h"
#include "../Object/ActorBase.h"
#include "../Object/Common/InputController.h"
#include "../Object/PlayerState.h"

InputComponent::InputComponent(std::shared_ptr<ActorBase> actor)
	:Component(actor)
{
	inputController_ = std::make_unique<InputController>();
}

InputComponent::~InputComponent()
{
}

void InputComponent::Update()
{

	// ヒット中は行動できない
	if (actor_->GetHitState())return;

	// 入力方向
	VECTOR dir = inputController_->Dir();

	// 攻撃中は移動できない
	if (!actor_->GetAttackState())
	{
		// 入力していたら移動する
		if (!Utility::EqualsVZero(dir))
		{
			// 方向を更新
			actor_->SetDir(dir);

			actor_->ChangeState(static_cast<int>(PlayerState::RUN));
		}
		// 入力していなければ待機状態にする
		else if (Utility::EqualsVZero(dir))
		{
			actor_->ChangeState(static_cast<int>(PlayerState::IDLE));
		}
	}

}
