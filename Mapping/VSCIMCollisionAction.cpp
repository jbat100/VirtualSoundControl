//
//  VSCCollisionReceiver.cpp
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 4/21/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#include "VSCIMCollisionAction.h"


VSC::Task::SPtr VSC::IM::CollisionVoidAction::createTaskForCollision(OB::Scene::Element::SPtr element, OB::Scene::Collision::SPtr collision)
{
    return Task::SPtr();
}