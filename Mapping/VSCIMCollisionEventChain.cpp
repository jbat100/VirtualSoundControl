
#include "VSCIMCollisionEventChain.h"
#include "VSCIMDelay.h"
#include "VSCOBScene.h"
#include "VSCOBElement.h"
#include "VSCOBCollision.h"
#include "VSCException.h"
#include "VSCTask.h"
#include "VSCTaskQueue.h"

#include <boost/shared_ptr.hpp>
#include <boost/foreach.hpp>

unsigned int VSC::IM::CollisionEventChain::getNumberOfActions(void)
{
    return 0; // NOT IMPLEMEMTED !!
}

bool VSC::IM::CollisionEventChain::checkEvent(Event::SPtr event)
{
    Delay::SPtr delay = boost::dynamic_pointer_cast<Delay>(event);
    if (delay) return true;
    
    CollisionAction::SPtr action = boost::dynamic_pointer_cast<CollisionAction>(event);
    if (action) return true;
    
    BOOST_ASSERT_MSG(false, "Event should be either Delay or CollisionAction");
    
    return false;
}

void VSC::IM::CollisionEventChain::perform(OB::Element::SPtr element, OB::Collision::SPtr collision)
{
    Time executionTime = CurrentTime();
    
    BOOST_FOREACH(Event::SPtr event, this->getEvents())
    {
        Delay::SPtr delay = boost::dynamic_pointer_cast<Delay>(event);
        if (delay)
        {
            executionTime += delay->getDelay();
            continue;
        }
        CollisionAction::SPtr action = boost::dynamic_pointer_cast<CollisionAction>(event);
        if (action)
        {
            Task::SPtr task = action->createTaskForCollision(element, collision);
            BOOST_ASSERT(task);
            if (task)
            {
                task->setExecutionStartTime(executionTime);
                BOOST_ASSERT(action->getTaskQueue());
                if (action->getTaskQueue())
                {
                    action->getTaskQueue()->enqueueTask(task);
                }
            }
            continue;
        }
        BOOST_ASSERT_MSG(false, "Unexpected event type");
    }
}
