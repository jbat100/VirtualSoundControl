
#include "VSCIMCollisionMapper.h"

#include "VSCOBElement.h"
#include "VSCOBCollision.h"

#include <boost/assert.hpp>
#include <boost/foreach.hpp>

void VSC::IM::CollisionMapper::setActionChainForCollisionStarted(CollisionEventChain::SPtr actionChain, OB::Element::SPtr element)
{
    mCollisionStartedEventChainMap[element] = actionChain;
}

void VSC::IM::CollisionMapper::setActionChainForCollisionEnded(CollisionEventChain::SPtr actionChain, OB::Element::SPtr element)
{
    mCollisionEndedEventChainMap[element] = actionChain;
}

VSC::IM::CollisionEventChain::SPtr VSC::IM::CollisionMapper::getEventChainForCollisionStarted(OB::Element::SPtr element)
{
    VSC::IM::CollisionEventChain::SPtr chain = mCollisionStartedEventChainMap[element];
    if (chain) return chain;
    
    if (mTraceCollisions) std::cout << "CollisionMapper::getEventChainForCollisionStarted, creating action chain" << std::endl;
    chain = VSC::IM::CollisionEventChain::SPtr(new VSC::IM::CollisionEventChain);
    mCollisionStartedEventChainMap[element] = chain;
    return chain;
}

VSC::IM::CollisionEventChain::SPtr VSC::IM::CollisionMapper::getEventChainForCollisionEnded(OB::Element::SPtr element)
{
    VSC::IM::CollisionEventChain::SPtr chain = mCollisionEndedEventChainMap[element];
    if (chain) return chain;
    
    if (mTraceCollisions) std::cout << "CollisionMapper::getEventChainForCollisionEnded, creating action chain" << std::endl;
    chain = VSC::IM::CollisionEventChain::SPtr(new VSC::IM::CollisionEventChain);
    mCollisionEndedEventChainMap[element] = chain;
    return chain;
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
                CollisionEventChain::SPtr actionChain = this->getEventChainForCollisionStarted(element);
                
                if (actionChain)
                {
                    actionChain->perform(element, collision);
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

