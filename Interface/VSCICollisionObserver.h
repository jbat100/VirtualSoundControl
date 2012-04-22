//
//  VSCCollissionReceiver.h
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 4/21/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#ifndef _VSCI_COLLISION_OBSERVER_H_
#define _VSCI_COLLISION_OBSERVER_H_

class VSCICollision;

class VSCICollisionObserver {
    
public:
    
    virtual void collisionStarted(void* sender, const VSCICollision& collision) = 0;    
    virtual void collisionEnded(void* sender, const VSCICollision& collision) = 0;
};
 

#endif
