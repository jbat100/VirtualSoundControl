
#include "VSCIMCollisionAction.h"

#include "VSCOBScene.h"
#include "VSCOBElement.h"
#include "VSCOBCollision.h"

VSC::Tasks VSC::IM::CollisionAction::generateTasks(void)
{
    return this->generateTasksForCollision(OB::Element::SPtr(), OB::Collision::SPtr());
}

VSC::Tasks VSC::IM::CollisionVoidAction::generateTasksForCollision(OB::Element::SPtr element, OB::Collision::SPtr collision)
{
    Tasks tasks;
    return tasks;
}
