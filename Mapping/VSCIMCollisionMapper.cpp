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

void VSC::IM::CollisionMapper::addActionChainForSceneElement(CollisionActionChain::SPtr actionChain, OB::Scene::Element::SPtr element)
{
    BOOST_ASSERT(actionChain);
    BOOST_ASSERT(element);
    if ((!actionChain) || (!element)) return;
    
    /* // trying prettier way of doing it
    CollisionActionChainMap::iterator it = mCollisionActionChainMap.find(element);
    if (it != mCollisionActionChainMap.end())
    {
        CollisionActionChains& actionChains = (*it).second;
        CollisionActionChains::iterator actionChainIt = std::find(actionChains.begin(), actionChains.end(), actionChain);
        BOOST_ASSERT_MSG(actionChainIt == actionChains.end(), "Action chain already registered for element");
        if (actionChainIt == actionChains.end()) actionChains.push_back(actionChain);
    }
    else
    {
        CollisionActionChains actionChains;
        actionChains.push_back(actionChain);
        mCollisionActionChainMap[element] = actionChains;
    }
     */
    
    CollisionActionChains& actionChains = mCollisionActionChainMap[element];
    CollisionActionChains::iterator actionChainIt = std::find(actionChains.begin(), actionChains.end(), actionChain);
    BOOST_ASSERT_MSG(actionChainIt == actionChains.end(), "Action chain already registered for element");
    if (actionChainIt == actionChains.end()) actionChains.push_back(actionChain);
    
}

void VSC::IM::CollisionMapper::removeActionChainForSceneElement(CollisionActionChain::SPtr actionChain, OB::Scene::Element::SPtr element)
{
    BOOST_ASSERT(actionChain);
    BOOST_ASSERT(element);
    if ((!actionChain) || (!element)) return;
    
    CollisionActionChains& actionChains = mCollisionActionChainMap[element];
    CollisionActionChains::iterator it = std::find(actionChains.begin(), actionChains.end(), actionChain);
    BOOST_ASSERT_MSG(it != actionChains.end(), "Action chain not registered for element");
    if (it != actionChains.end()) actionChains.erase(it);
}

const VSC::IM::CollisionActionChains& VSC::IM::CollisionMapper::getActionChainsForSceneElement(OB::Scene::Element::SPtr element)
{
    CollisionActionChains& actionChains = mCollisionActionChainMap[element];
    return actionChains;
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
                const CollisionActionChains& actionChains = this->getActionChainsForSceneElement(element);
                
                BOOST_FOREACH(CollisionActionChain::SPtr actionChain, actionChains)
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

