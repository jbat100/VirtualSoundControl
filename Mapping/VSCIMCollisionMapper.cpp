
#include "VSCIMCollisionMapper.h"

#include "VSCOBElement.h"
#include "VSCOBCollision.h"

#include <boost/assert.hpp>
#include <boost/foreach.hpp>

void VSC::IM::CollisionMapper::addActionChainForCollisionStarted(CollisionEventChain::SPtr actionChain, OB::Element::SPtr element)
{
    CollisionEventChains& actionChains = mCollisionStartedEventChainMap[element];
    CollisionEventChains::iterator it = std::find(actionChains.begin(), actionChains.end(), actionChain);
    if (it == actionChains.end())
    {
        actionChains.push_back(actionChain);
    }
}

void VSC::IM::CollisionMapper::addActionChainForCollisionEnded(CollisionEventChain::SPtr actionChain, OB::Element::SPtr element)
{
    CollisionEventChains& actionChains = mCollisionEndedEventChainMap[element];
    CollisionEventChains::iterator it = std::find(actionChains.begin(), actionChains.end(), actionChain);
    if (it == actionChains.end())
    {
        actionChains.push_back(actionChain);
    }
}

VSC::IM::CollisionEventChains& VSC::IM::CollisionMapper::getEventChainsForCollisionStarted(OB::Element::SPtr element)
{
    return mCollisionStartedEventChainMap[element];
}

VSC::IM::CollisionEventChains& VSC::IM::CollisionMapper::getEventChainsForCollisionEnded(OB::Element::SPtr element)
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
        OB::Elements bothElements;
        bothElements.push_back(collision->getFirstElement());
        bothElements.push_back(collision->getSecondElement());
        
        BOOST_FOREACH(OB::Element::SPtr element, bothElements)
        {
            BOOST_ASSERT(element);
            if (element)
            {
                CollisionEventChains actionChains = this->getEventChainsForCollisionStarted(element);
                
                BOOST_FOREACH(CollisionEventChain::SPtr actionChain, actionChains)
                {
                    BOOST_ASSERT(actionChain);
                    if (actionChain)
                    {
                        actionChain->perform(element, collision);
                    }
                }
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

