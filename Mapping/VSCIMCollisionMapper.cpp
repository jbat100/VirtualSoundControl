
#include "VSCIMCollisionMapper.h"

#include "VSCOBElement.h"
#include "VSCOBCollision.h"

#include <boost/assert.hpp>
#include <boost/foreach.hpp>

using namespace VSC;
using namespace VSC::IM;
using namespace VSC::OB;

void CollisionMapper::addEventChainForCollisionStarted(EventChain::SPtr eventChain, Element::SPtr element)
{
    EventChains& eventChains = mCollisionStartedEventChainMap[element];
    EventChains::iterator it = std::find(eventChains.begin(), eventChains.end(), eventChain);
    if (it == eventChains.end())
    {
        eventChains.push_back(eventChain);
    }
}

void CollisionMapper::addEventChainForCollisionEnded(EventChain::SPtr eventChain, Element::SPtr element)
{
    EventChains& eventChains = mCollisionEndedEventChainMap[element];
    EventChains::iterator it = std::find(eventChains.begin(), eventChains.end(), eventChain);
    if (it == eventChains.end())
    {
        eventChains.push_back(eventChain);
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
    
    if (collision)
    {
        /*
         *  Get event chains for first element and perform them with second element as effector
         */
        
        EventChains firstElementEventChains = this->getEventChainsForCollisionStarted(collision->getFirstElement());
        
        BOOST_FOREACH(EventChain::SPtr eventChain, firstElementEventChains)
        {
            BOOST_ASSERT(eventChain);
            if (eventChain)
            {
                eventChain->performForCollision(collision, collision->getSecondElement());
            }
        }
        
        /*
         *  Get event chains for second element and perform them with first element as effector
         */
        
        EventChains secondElementEventChains = this->getEventChainsForCollisionStarted(collision->getSecondElement());
        
        BOOST_FOREACH(EventChain::SPtr eventChain, secondElementEventChains)
        {
            BOOST_ASSERT(eventChain);
            if (eventChain)
            {
                eventChain->performForCollision(collision, collision->getFirstElement());
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

