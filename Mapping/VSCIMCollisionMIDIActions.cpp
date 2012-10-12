
//
//  Created by Jonathan Thorpe on 4/21/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#include "VSCIMCollisionMIDIActions.h"

#include "VSCMIDITasks.h"

VSC::IM::CollisionMIDIAction::CollisionMIDIAction()
{
    this->setTaskQueue(MIDI::SingletonMIDITaskQueue());
}

VSC::IM::CollisionMIDIAction::CollisionMIDIAction(MIDI::OutputChannel::SPtr outputChannel) :
mOutputChannel(outputChannel)
{
    this->setTaskQueue(MIDI::SingletonMIDITaskQueue());
}


VSC::Task::SPtr VSC::IM::CollisionMIDINoteOnAction::createTaskForCollision(OB::Scene::Element::SPtr element,
                                                                           OB::Scene::Collision::SPtr collision)
{
    return Task::SPtr();
}

VSC::Task::SPtr VSC::IM::CollisionMIDINoteOffAction::createTaskForCollision(OB::Scene::Element::SPtr element,
                                                                            OB::Scene::Collision::SPtr collision)
{
    return Task::SPtr();
}

VSC::Task::SPtr VSC::IM::CollisionMIDINoteOnAndOffAction::createTaskForCollision(OB::Scene::Element::SPtr element,
                                                                                 OB::Scene::Collision::SPtr collision)
{
    return Task::SPtr();
}