//
//  VSCCollissionReceiver.h
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 4/21/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#ifndef _VSCI_OBJECT_H_
#define _VSCI_OBJECT_H_

#include <BulletDynamics/btBulletDynamicsCommon.h>
#include <boost/shared_ptr.hpp>
#include <vector>

#include "VSCICollisionObserver.h"

class VSCIObject;
typedef boost::shared_ptr<VSCIObject> VSCIObjectPtr;

class VSCIObject : public VSCICollisionObserver {
    
public:
    
    VSCIObject(const btCollisionObject* btObj);
    
    /*
     *  Determines the sonic behaviour
     */
    enum CollisionBehaviour {
        CollisionBehaviourNeutral = 0,
        CollisionBehaviourEffector = 1 << 0,
        CollisionBehaviourReceptor = 1 << 1,
        CollisionBehaviourReceptorAndEffector = (CollisionBehaviourEffector | CollisionBehaviourReceptor)
    };
    
    void setCollisionBehaviour(const CollisionBehaviour cBehaviour);
    CollisionBehaviour getCollisionType(void) const;
    
    void registerCollisionHandler(VSCICollisionObserver* handler);
    void unregisterCollisionHandler(VSCICollisionObserver* handler);
    void clearCollisionHandlers(void);
    
    /*
     *  VSCICollisionObserver
     */
    virtual void collisionStarted(void* sender, const VSCICollision& collision);    
    virtual void collisionEnded(void* sender, const VSCICollision& collision);
    
private:
    
    CollisionBehaviour _collisionBehaviour;
    
    /*
     *  Use containment not inheritance!
     *  Otherwise we won't be able to use bt subclasses of btCollisionObject
     */
    const btCollisionObject* _btObject; 
    
    std::vector<VSCICollisionObserver*> _collisionHandlers;
    
};

#endif
