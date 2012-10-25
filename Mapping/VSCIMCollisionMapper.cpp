//
//  VSCCollisionReceiver.cpp
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 4/21/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#include "VSCIMCollisionMapper.h"

void VSC::IM::CollisionMapper::addActionChainForSceneElement(CollisionActionChain::SPtr actionChain, OB::Scene::Element::SPtr element)
{
    
}

void VSC::IM::CollisionMapper::removeActionChainForSceneElement(CollisionActionChain::SPtr actionChain, OB::Scene::Element::SPtr element)
{
    
}

const VSC::IM::CollisionActionChains& VSC::IM::CollisionMapper::getActionChainsForSceneElement(OB::Scene::Element::SPtr element)
{
    
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
}

void VSC::IM::CollisionMapper::collisionUpdated(OB::Scene::Collision::SPtr collision)
{
    //if (mTraceCollisions) std::cout << "Collision updated: " << *collision << std::endl;
}

void VSC::IM::CollisionMapper::collisionEnded(OB::Scene::Collision::SPtr collision)
{
    //if (mTraceCollisions) std::cout << "Collision ended: " << *collision << std::endl;
}

