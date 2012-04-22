//
//  VSCCollissionReceiver.h
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 4/21/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#ifndef _VSCI_COLLISION_H_
#define _VSCI_COLLISION_H_

#include <BulletDynamics/btBulletDynamicsCommon.h>
#include <boost/shared_ptr.hpp>
#include <list>

#include "VSCIObject.h"

class VSCICollision;
typedef boost::shared_ptr<VSCCollision> VSCCollisionPtr;

class VSCICollision {
    
public:
    
    //VSCCollision(btPersistentManifold* manifold);
    VSCCollision(const VSCIObject* effector, const VSCIObject* receptor, const btPersistentManifold* manifold);
    VSCCollision(const VSCCollision& col);
    VSCCollision& operator=(const VSCCollision& col);
    
    bool operator==(const VSCCollision& p) const;
    bool operator!=(const VSCCollision& p) const;
    bool operator<(const VSCCollision& p) const;
    
    const btCollisionObject* getReceptorObject() const;
    const btCollisionObject* getEffectorObject() const;
    
private:
    
    const VSCIObject* _effectorObject;
    const VSCIObject* _receptorObject;
    
    const btPersistentManifold* _persistentManifold;
    
};
 

#endif
