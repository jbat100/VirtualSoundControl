
#include "VSCIMCollisionAction.h"

#include "VSCOBScene.h"
#include "VSCOBElement.h"
#include "VSCOBCollision.h"

VSC::Task::SPtr VSC::IM::CollisionVoidAction::createTaskForCollision(OB::Element::SPtr element, OB::Collision::SPtr collision)
{
    return Task::SPtr();
}