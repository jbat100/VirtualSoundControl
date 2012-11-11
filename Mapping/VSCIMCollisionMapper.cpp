//
//  VSCCollisionReceiver.cpp
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 4/21/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#include "VSCIMCollisionMapper.h"

#include <boost/assert.hpp>
#include <boost/foreach.hpp>

void VSC::IM::CollisionMapper::setActionChainForCollisionStarted(CollisionEventChain::SPtr actionChain, OB::Scene::Element::SPtr element)
{
    mCollisionStartedEventChainMap[element] = actionChain;
}

void VSC::IM::CollisionMapper::setActionChainForCollisionEnded(CollisionEventChain::SPtr actionChain, OB::Scene::Element::SPtr element)
{
    mCollisionEndedEventChainMap[element] = actionChain;
}

VSC::IM::CollisionEventChain::SPtr VSC::IM::CollisionMapper::getActionChainForCollisionStarted(OB::Scene::Element::SPtr element)
{
    return mCollisionStartedEventChainMap[element];
}

VSC::IM::CollisionEventChain::SPtr VSC::IM::CollisionMapper::getActionChainForCollisionEnded(OB::Scene::Element::SPtr element)
{
    return mCollisionEndedEventChainMap[element];
}

void VSC::IM::CollisionMapper::collisionProspectDetected(OB::Scene::Collision::SPtr collision)
{
    //if (mTraceCollisions) std::cout << "Collision prospect detected: " << *collision << std::endl;
}

void VSC::IM::CollisionMapper::collisionProspectUpdated(OB::Scene::Collision::SPtr collision)
{
    //if (mTraceCollisions) std::cout << "Collision prospect updated: " << *collision << std::endl;
}

void VSC::IM::CollisionMapper::collisionProspectEnded(OB::Scene::Collision::SPtr collision)
{
    //if (mTraceCollisions) std::cout << "Collision prospect ended: " << *collision << std::endl;
}

void VSC::IM::CollisionMapper::collisionDetected(OB::Scene::Collision::SPtr collision)
{
    if (mTraceCollisions) std::cout << "Collision detected: " << *collision << std::endl;
    
    BOOST_ASSERT(collision);
    
    if (collision)
    {
        OB::Scene::Elements bothElements;
        bothElements.push_back(collision->getFirstElement());
        bothElements.push_back(collision->getSecondElement());
        
        BOOST_FOREACH(OB::Scene::Element::SPtr element, bothElements)
        {
            BOOST_ASSERT(element);
            if (element)
            {
                CollisionEventChain::SPtr actionChain = this->getActionChainForCollisionStarted(element);
                
                if (actionChain)
                {
                    actionChain->perform(element, collision);
                }
            }
        }
    }

}

void VSC::IM::CollisionMapper::collisionUpdated(OB::Scene::Collision::SPtr collision)
{
    //if (mTraceCollisions) std::cout << "Collision updated: " << *collision << std::endl;
}

void VSC::IM::CollisionMapper::collisionEnded(OB::Scene::Collision::SPtr collision)
{
    //if (mTraceCollisions) std::cout << "Collision ended: " << *collision << std::endl;
}

