
#include "VSCCollisionMapper.h"

#include "VSCOB.h"
#include "VSCIM.h"

#include "VSCOBElement.h"
#include "VSCOBCollision.h"

#include "VSCIMEvent.h"
#include "VSCIMEventChain.h"

#include <boost/assert.hpp>
#include <boost/foreach.hpp>

using namespace VSC;
using namespace VSC::IM;
using namespace VSC::OB;

void CollisionMapper::addEventChainForCollisionStarted(EventChain::SPtr eventChain, Element::SPtr element)
{
    BOOST_ASSERT(eventChain);
    BOOST_ASSERT(element);
    if (eventChain && element)
    {
        EventChains& eventChains = mCollisionStartedEventChainMap[element];
        EventChains::iterator it = std::find(eventChains.begin(), eventChains.end(), eventChain);
        BOOST_ASSERT(it == eventChains.end());
        if (it == eventChains.end())
        {
            eventChains.push_back(eventChain);
        }
    }
}

void CollisionMapper::addEventChainForCollisionEnded(EventChain::SPtr eventChain, Element::SPtr element)
{
    BOOST_ASSERT(eventChain);
    BOOST_ASSERT(element);
    if (eventChain && element)
    {
        EventChains& eventChains = mCollisionEndedEventChainMap[element];
        EventChains::iterator it = std::find(eventChains.begin(), eventChains.end(), eventChain);
        BOOST_ASSERT(it == eventChains.end());
        if (it == eventChains.end())
        {
            eventChains.push_back(eventChain);
        }
    }
}

void CollisionMapper::removeEventChainForCollisionStarted(EventChain::SPtr eventChain, Element::SPtr element)
{
    BOOST_ASSERT(eventChain);
    BOOST_ASSERT(element);
    if (eventChain && element)
    {
        EventChains& eventChains = mCollisionStartedEventChainMap[element];
        EventChains::iterator it = std::find(eventChains.begin(), eventChains.end(), eventChain);
        BOOST_ASSERT(it != eventChains.end());
        if (it != eventChains.end())
        {
            eventChains.erase(it);
        }
    }
}

void CollisionMapper::removeEventChainForCollisionEnded(EventChain::SPtr eventChain, Element::SPtr element)
{
    BOOST_ASSERT(eventChain);
    BOOST_ASSERT(element);
    if (eventChain && element)
    {
        EventChains& eventChains = mCollisionEndedEventChainMap[element];
        EventChains::iterator it = std::find(eventChains.begin(), eventChains.end(), eventChain);
        BOOST_ASSERT(it != eventChains.end());
        if (it != eventChains.end())
        {
            eventChains.erase(it);
        }
    }
}

void CollisionMapper::clearEventChain(IM::EventChain_SPtr eventChain)
{
    BOOST_ASSERT(eventChain);
    if (eventChain)
    {
        BOOST_FOREACH(ElementEventChainMap::value_type elementEventChains, mCollisionStartedEventChainMap)
        {
            IM::EventChains& eventChains = elementEventChains.second;
            IM::EventChains::iterator it = std::find(eventChains.begin(), eventChains.end(), eventChains);
            if (it != eventChains.end()) eventChains.erase(it);
        }
        BOOST_FOREACH(ElementEventChainMap::value_type elementEventChains, mCollisionEndedEventChainMap)
        {
            IM::EventChains& eventChains = elementEventChains.second;
            IM::EventChains::iterator it = std::find(eventChains.begin(), eventChains.end(), eventChains);
            if (it != eventChains.end()) eventChains.erase(it);
        }
    }
}

const EventChains& CollisionMapper::getEventChainsForCollisionStarted(Element::SPtr element)
{
    return mCollisionStartedEventChainMap[element];
}

const EventChains& CollisionMapper::getEventChainsForCollisionEnded(Element::SPtr element)
{
    return mCollisionEndedEventChainMap[element];
}

void CollisionMapper::collisionProspectDetected(Collision::SPtr collision)
{
    //if (mTraceCollisions) std::cout << "Collision prospect detected: " << *collision << std::endl;
}

void CollisionMapper::collisionProspectUpdated(Collision::SPtr collision)
{
    //if (mTraceCollisions) std::cout << "Collision prospect updated: " << *collision << std::endl;
}

void CollisionMapper::collisionProspectEnded(Collision::SPtr collision)
{
    //if (mTraceCollisions) std::cout << "Collision prospect ended: " << *collision << std::endl;
}

void CollisionMapper::collisionDetected(Collision::SPtr collision)
{
    if (mTraceCollisions) std::cout << "Collision detected: " << *collision << std::endl;
    
    BOOST_ASSERT(collision);
    BOOST_ASSERT(collision->getFirstElement());
    BOOST_ASSERT(collision->getSecondElement());
    
    if (collision && collision->getFirstElement() && collision->getSecondElement())
    {
        /*
         *  Get event chains for first element and perform them with second element as effector
         */
        
        EventChains firstElementEventChains = this->getEventChainsForCollisionStarted(collision->getFirstElement());
        
        TriggerCollisionPayload::SPtr collisionPayload(new TriggerCollisionPayload);
        collisionPayload->collision = collision;
        collisionPayload->effector = collision->getSecondElement();
        
        BOOST_FOREACH(EventChain::SPtr eventChain, firstElementEventChains)
        {
            BOOST_ASSERT(eventChain);
            if (eventChain)
            {
                eventChain->perform(TriggerCollision, collisionPayload);
            }
        }
        
        /*
         *  Get event chains for second element and perform them with first element as effector
         */
        
        EventChains secondElementEventChains = this->getEventChainsForCollisionStarted(collision->getSecondElement());
        
        collisionPayload->effector = collision->getFirstElement();
        
        BOOST_FOREACH(EventChain::SPtr eventChain, secondElementEventChains)
        {
            BOOST_ASSERT(eventChain);
            if (eventChain)
            {
                eventChain->perform(TriggerCollision, collisionPayload);
            }
        }
    }
    
}

void CollisionMapper::collisionUpdated(Collision::SPtr collision)
{
    //if (mTraceCollisions) std::cout << "Collision updated: " << *collision << std::endl;
}

void CollisionMapper::collisionEnded(Collision::SPtr collision)
{
    //if (mTraceCollisions) std::cout << "Collision ended: " << *collision << std::endl;
}

