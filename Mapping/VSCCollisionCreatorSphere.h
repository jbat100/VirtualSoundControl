//
//  VSCCollissionReceiver.h
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 4/21/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#ifndef _VSC_COLLISION_CREATOR_SPHERE_H_
#define _VSC_COLLISION_CREATOR_SPHERE_H_

#include "VSCCollisionCreator.h"
#include <BulletCollision/CollisionShapes/btSphereShape.h>
#include <LinearMath/btScalar.h>

class VSCCollisionCreatorSphere : public VSCCollisionCreator, public btSphereShape {
    
    
public:
    
    VSCCollisionCreatorSphere(btScalar radius) : btSphereShape(radius) {}
    
    
private:
    
    
    
};

#endif
