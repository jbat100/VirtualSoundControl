//
//  VSCCollisionReceiver.cpp
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 4/21/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#include "VSCIObject.h"
#include "VSCICollisionObserver.h"

void VSCIObject::setCollisionBehaviour(const CollisionBehaviour cBehaviour) 
{
    _collisionBehaviour = cBehaviour;
}

VSCIObject::CollisionBehaviour VSCIObject::getCollisionType(void) const 
{
    return _collisionBehaviour;
}

void VSCIObject::registerCollisionHandler(VSCICollisionObserver* handler) 
{
    if (std::find(_collisionHandlers.begin(), _collisionHandlers.end(), handler) == _collisionHandlers.end()) {
        _collisionHandlers.push_back(handler);
    }
}

void VSCIObject::unregisterCollisionHandler(VSCICollisionObserver* handler) 
{
    std::vector<VSCICollisionObserver*>::iterator it = std::find(_collisionHandlers.begin(), _collisionHandlers.end(), handler);
    if (it != _collisionHandlers.end()) {
        _collisionHandlers.erase(it);
    }
}

void VSCIObject::clearCollisionHandlers(void)
{
    _collisionHandlers.clear();
}

void VSCIObject::collisionStarted(void* sender, const VSCICollision& collision) 
{
    for (std::vector<VSCICollisionObserver*>::iterator it = _collisionHandlers.begin(); it != _collisionHandlers.end(); it++) {
        (*it)->collisionStarted(this, collision);
    }
}

void VSCIObject::collisionEnded(void* sender, const VSCICollision& collision) 
{
    for (std::vector<VSCICollisionObserver*>::iterator it = _collisionHandlers.begin(); it != _collisionHandlers.end(); it++) {
        (*it)->collisionEnded(this, collision);
    }
}