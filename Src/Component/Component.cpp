#include <memory>
#include "Component.h"

Component::Component()
{
}

const std::shared_ptr<ActorBase>& Component::GetActor() const
{
	return actor_;
}
