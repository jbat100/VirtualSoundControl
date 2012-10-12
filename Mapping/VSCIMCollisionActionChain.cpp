//
//  VSCCollisionReceiver.cpp
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 4/21/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#include "VSCIMCollisionActionChain.h"
#include "VSCException.h"
#include "VSCTask.h"
#include "VSCTaskQueue.h"

#include <boost/foreach.hpp>


void VSC::IM::CollisionActionChain::perform(OB::Scene::Element::SPtr element, OB::Scene::Collision::SPtr collision)
{
    BOOST_FOREACH(CollisionAction::SPtr action, mActions)
    {
        if (action->getTaskQueue())
        {
            Task::SPtr task = action->createTaskForCollision(element, collision);
            if (task)
            {
                action->getTaskQueue()->enqueueTask(task);
            }
        }
        else
        {
            // throw ?
        }
        
    }
}

unsigned int VSC::IM::CollisionActionChain::numberOfActions(void)
{
    return (unsigned int)mActions.size();
}

void VSC::IM::CollisionActionChain::appendAction(CollisionAction::SPtr action)
{
    CollisionActions::iterator it = std::find(mActions.begin(), mActions.end(), action);
    if (it == mActions.end())
    {
        mActions.push_back(action);
    }
    else
    {
        throw VSCInvalidArgumentException("Attempting to add action already in action chain");
    }
}

void VSC::IM::CollisionActionChain::prependAction(CollisionAction::SPtr action)
{
    this->insertActionAtIndex(action, 0);
}

void VSC::IM::CollisionActionChain::insertActionAtIndex(CollisionAction::SPtr action, unsigned int index)
{
    if (index > numberOfActions())
    {
        throw VSCInvalidArgumentException("Attempting to add action at invalid index");
    }
    
    CollisionActions::iterator it = std::find(mActions.begin(), mActions.end(), action);
    if (it == mActions.end())
    {
        // http://stackoverflow.com/questions/2330551/c-insert-into-vector-at-known-position
        
        mActions.insert(mActions.begin()+index, action);
    }
    else
    {
        throw VSCInvalidArgumentException("Attempting to add action already in action chain");
    }
}

