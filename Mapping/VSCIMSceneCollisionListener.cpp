//
//  VSCCollisionReceiver.cpp
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 4/21/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#include "VSCIMSceneCollisionListener.h"

void VSC::IM::CollisionListener::collisionProspectDetected(OB::Scene::Collision::SPtr collision)
{
    if (mTraceCollisions) std::cout << "Collision prospect detected: " << *collision << std::endl;
}

void VSC::IM::CollisionListener::collisionProspectUpdated(OB::Scene::Collision::SPtr collision)
{
    //if (mTraceCollisions) std::cout << "Collision prospect updated: " << *collision << std::endl;
}

void VSC::IM::CollisionListener::collisionProspectEnded(OB::Scene::Collision::SPtr collision)
{
    if (mTraceCollisions) std::cout << "Collision prospect ended: " << *collision << std::endl;
}

void VSC::IM::CollisionListener::collisionDetected(OB::Scene::Collision::SPtr collision)
{
    if (mTraceCollisions) std::cout << "Collision detected: " << *collision << std::endl;
}

void VSC::IM::CollisionListener::collisionUpdated(OB::Scene::Collision::SPtr collision)
{
    //if (mTraceCollisions) std::cout << "Collision updated: " << *collision << std::endl;
}

void VSC::IM::CollisionListener::collisionEnded(OB::Scene::Collision::SPtr collision)
{
    if (mTraceCollisions) std::cout << "Collision ended: " << *collision << std::endl;
}

