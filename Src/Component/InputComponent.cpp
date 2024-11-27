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

	// �q�b�g���͍s���ł��Ȃ�
	if (actor_->GetHitState())return;

	// ���͕���
	VECTOR dir = inputController_->Dir();

	// �U�����͈ړ��ł��Ȃ�
	if (!actor_->GetAttackState())
	{
		// ���͂��Ă�����ړ�����
		if (!Utility::EqualsVZero(dir))
		{
			// �������X�V
			actor_->SetDir(dir);

			actor_->ChangeState(static_cast<int>(PlayerState::RUN));
		}
		// ���͂��Ă��Ȃ���Αҋ@��Ԃɂ���
		else if (Utility::EqualsVZero(dir))
		{
			actor_->ChangeState(static_cast<int>(PlayerState::IDLE));
		}
	}

}
