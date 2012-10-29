

#include "VSCIMEvent.h"

#include "VSCException.h"
#include "VSCTask.h"
#include "VSCTaskQueue.h"

#include <boost/foreach.hpp>

unsigned int VSC::IM::EventChain::numberOfEvents(void)
{
    return (unsigned int) mEvents.size();
}

void VSC::IM::EventChain::appendEvent(Event::SPtr event)
{
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
    this->insertEventAtIndex(event, 0);
}

void VSC::IM::EventChain::insertEventAtIndex(Event::SPtr event, unsigned int index)
{
    if (index > numberOfEvents())
    {
        throw VSCInvalidArgumentException("Attempting to add event at invalid index");
    }
    
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

void VSC::IM::EventChain::swapEvents(Event::SPtr firstEvent, Event::SPtr secondEvent)
{
    
}

