
#include "VSCIMEventChain.h"
#include "VSCIMEvent.h"
#include "VSCIMDelay.h"
#include "VSCIMAction.h"

#include "VSCOBCollision.h"

#include "VSCException.h"
#include "VSCTask.h"
#include "VSCTaskQueue.h"

#include <boost/foreach.hpp>

unsigned int VSC::IM::EventChain::getNumberOfEvents(void)
{
    return (unsigned int) mEvents.size();
}

VSC::IM::Event::SPtr VSC::IM::EventChain::getEventAtIndex(unsigned int index)
{
    BOOST_ASSERT(this->getNumberOfEvents() > index);
    
    if (this->getNumberOfEvents() > index)
    {
        return mEvents.at(index);
    }
    
    return Event::SPtr();
}

void VSC::IM::EventChain::appendEvent(Event::SPtr event)
{
    if (!this->checkEvent(event))        
    {
        throw VSCInvalidArgumentException("Attempting to event of invalid type");
    }
    
    Events::iterator it = std::find(mEvents.begin(), mEvents.end(), event);
    if (it == mEvents.end())
    {
        mEvents.push_back(event);
    }
    else
    {
        throw VSCInvalidArgumentException("Attempting to add event already in event chain");
    }
}

void VSC::IM::EventChain::prependEvent(Event::SPtr event)
{
    this->checkEvent(event);
    
    this->insertEventAtIndex(event, 0);
}

void VSC::IM::EventChain::insertEventAtIndex(Event::SPtr event, unsigned int index)
{
    if (index > getNumberOfEvents())
    {
        throw VSCInvalidArgumentException("Attempting to add event at invalid index");
    }
    
    this->checkEvent(event);
    
    Events::iterator it = std::find(mEvents.begin(), mEvents.end(), event);
    if (it == mEvents.end())
    {
        // http://stackoverflow.com/questions/2330551/c-insert-into-vector-at-known-position
        
        mEvents.insert(mEvents.begin()+index, event);
    }
    else
    {
        throw VSCInvalidArgumentException("Attempting to add action already in action chain");
    }
}

void VSC::IM::EventChain::insertEventAfterEvent(Event::SPtr insertedEvent, Event::SPtr event)
{
    this->checkEvent(insertedEvent);
    
    Events::iterator it = std::find(mEvents.begin(), mEvents.end(), event);
    BOOST_ASSERT(it != mEvents.end());
    if(it == mEvents.end());
    ++it;
    mEvents.insert(it, insertedEvent);
}

void VSC::IM::EventChain::insertEventBeforeEvent(Event::SPtr insertedEvent, Event::SPtr event)
{
    this->checkEvent(insertedEvent);
    
    Events::iterator it = std::find(mEvents.begin(), mEvents.end(), event);
    BOOST_ASSERT(it != mEvents.end());
    if(it == mEvents.end());
    mEvents.insert(it, insertedEvent);
}

void VSC::IM::EventChain::swapEvents(Event::SPtr firstEvent, Event::SPtr secondEvent)
{
    Events::iterator firstIt = std::find(mEvents.begin(), mEvents.end(), firstEvent);
    Events::iterator secondIt = std::find(mEvents.begin(), mEvents.end(), secondEvent);
    
    BOOST_ASSERT_MSG(firstIt != mEvents.end(), "First event is not in the chain");
    
    if (firstIt == mEvents.end()) return;
    
    this->checkEvent(firstEvent);
    this->checkEvent(secondEvent);
    
    (*firstIt) = secondEvent;
    
    if (secondIt != mEvents.end())
    {
        (*secondIt) = firstEvent;
    }
}

void VSC::IM::EventChain::removeEvent(Event::SPtr event)
{
    
}

bool VSC::IM::EventChain::checkEvent(Event::SPtr event)
{
    Delay::SPtr delay = boost::dynamic_pointer_cast<Delay>(event);
    if (delay) return true;
    
    Action::SPtr action = boost::dynamic_pointer_cast<Action>(event);
    if (action) return true;
    
    BOOST_ASSERT_MSG(false, "Event should be either Delay or CollisionAction");
    
    return false;
}

void VSC::IM::EventChain::performForCollision(OB::Collision::SPtr collision)
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
        Action::SPtr action = boost::dynamic_pointer_cast<Action>(event);
        if (action)
        {
            Tasks tasks = action->generateTasksForCollision(collision);
            
            BOOST_FOREACH(Task::SPtr task, tasks)
            {
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
            }
            
            continue;
        }
        BOOST_ASSERT_MSG(false, "Unexpected event type");
    }
}
