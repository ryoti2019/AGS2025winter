#include <memory>
#include "Component.h"

Component::Component()
{
}

const std::weak_ptr<ActorBase>& Component::GetActor() const
{
	return actor_;
}
