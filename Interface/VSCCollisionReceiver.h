//
//  VSCCollissionReceiver.h
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 4/21/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#ifndef _VSC_COLLISION_RECEIVER_H_
#define _VSC_COLLISION_RECEIVER_H_

class VSCCollisionCreator;
class VSCCollision;

class VSCCollisionReceiver {
    
    
public:
    
    virtual bool collisionTest(VSCCollisionCreator* creator, VSCCollision** collision) = 0;
    
private:
    
    
};

#endif
