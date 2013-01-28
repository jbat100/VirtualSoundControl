
#include "VSCIMCollisionMapper.h"

#include "VSCOBElement.h"
#include "VSCOBCollision.h"

#include <boost/assert.hpp>
#include <boost/foreach.hpp>

void VSC::IM::CollisionMapper::addEventChainForCollisionStarted(EventChain::SPtr eventChain, OB::Element::SPtr element)
{
    EventChains& eventChains = mCollisionStartedEventChainMap[element];
    EventChains::iterator it = std::find(eventChains.begin(), eventChains.end(), eventChain);
    if (it == eventChains.end())
    {
        eventChains.push_back(eventChain);
    }
}

void VSC::IM::CollisionMapper::addEventChainForCollisionEnded(EventChain::SPtr eventChain, OB::Element::SPtr element)
{
    EventChains& eventChains = mCollisionEndedEventChainMap[element];
    EventChains::iterator it = std::find(eventChains.begin(), eventChains.end(), eventChain);
    if (it == eventChains.end())
    {
        eventChains.push_back(eventChain);
    }
}

VSC::IM::EventChains& VSC::IM::CollisionMapper::getEventChainsForCollisionStarted(OB::Element::SPtr element)
{
    return mCollisionStartedEventChainMap[element];
}

VSC::IM::EventChains& VSC::IM::CollisionMapper::getEventChainsForCollisionEnded(OB::Element::SPtr element)
{
    return mCollisionEndedEventChainMap[element];
}

void VSC::IM::CollisionMapper::collisionProspectDetected(OB::Collision::SPtr collision)
{
    //if (mTraceCollisions) std::cout << "Collision prospect detected: " << *collision << std::endl;
}

void VSC::IM::CollisionMapper::collisionProspectUpdated(OB::Collision::SPtr collision)
{
    //if (mTraceCollisions) std::cout << "Collision prospect updated: " << *collision << std::endl;
}

void VSC::IM::CollisionMapper::collisionProspectEnded(OB::Collision::SPtr collision)
{
    //if (mTraceCollisions) std::cout << "Collision prospect ended: " << *collision << std::endl;
}

void VSC::IM::CollisionMapper::collisionDetected(OB::Collision::SPtr collision)
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

void VSC::IM::CollisionMapper::collisionUpdated(OB::Collision::SPtr collision)
{
    //if (mTraceCollisions) std::cout << "Collision updated: " << *collision << std::endl;
}

void VSC::IM::CollisionMapper::collisionEnded(OB::Collision::SPtr collision)
{
    //if (mTraceCollisions) std::cout << "Collision ended: " << *collision << std::endl;
}

