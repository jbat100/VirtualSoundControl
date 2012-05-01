//
//  VSCCollissionReceiver.h
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 4/21/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#ifndef _VSCI_ENVIRONMENT_H_
#define _VSCI_ENVIRONMENT_H_

#include <BulletDynamics/btBulletDynamicsCommon.h>
#include <boost/shared_ptr.hpp>
#include <map>

#include "VSCIObject.h"
#include "VSCICollision.h"

class VSCIEnvironment;
typedef boost::shared_ptr<VSCIObject> VSCIObjectPtr;

class VSCIEnvironment : public VSCICollisionObserver {
    
public:
    
    VSCIObjectPtr objectForBTObject(btCollisionObject* object);
    
    /**
     *  VSCICollisionObserver methods
     */
    virtual void collisionStarted(void* sender, VSCICollisionPtr collision);
    virtual void collisionEnded(void* sender, VSCICollisionPtr collision);
    
private:
    
    typedef std::vector<VSCIObjectPtr>                      ObjectGroup;
    //typedef std::vector<VSCICollisionHandler*>              CollisionHandlerGroup;
    
    ObjectGroup             _allObjects;
    //CollisionHandlerGroup   _collisionHandlers;
    
    /*
     *  We need a fast way to link (non-intrusively) btCollisionObjects to their corresponding VSCIObject container.
     */
    
    typedef std::map<VSCIObjectPtr, btCollisionObject*>     VSCBTObjectMap;
    typedef std::pair<VSCIObjectPtr, btCollisionObject*>    VSCBTObjectPair;
    
};

#endif
