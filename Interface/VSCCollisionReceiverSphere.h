//
//  VSCCollissionReceiver.h
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 4/21/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#ifndef _VSC_COLLISION_RECEIVER_H_
#define _VSC_COLLISION_RECEIVER_H_

#include "VSCCollisionCreator.h"
#include <BulletCollision/CollisionShapes/btSphereShape.h>

class VSCCollisionReceiverSphere : public VSCCollisionCreator, public btSphereShape {
    
    
public:
    
    bool collisionTest(VSCCollisionCreator* creator, VSCCollision** collision);
    
    
private:
    
    
};

#endif
