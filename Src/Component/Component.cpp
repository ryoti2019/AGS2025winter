#include <memory>
#include "Component.h"

Component::Component(std::shared_ptr<ActorBase> actor)
	:actor_(actor)
{
}

const std::shared_ptr<ActorBase>& Component::GetActor() const
{
	return actor_;
}
