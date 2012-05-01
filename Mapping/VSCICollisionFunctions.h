//
//  VSCCollissionReceiver.h
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 4/21/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#ifndef _VSC_COLLISION_FUNCTIONS_H_
#define _VSC_COLLISION_FUNCTIONS_H_

class VSCCollisionReceiver;
class VSCCollisionCreator;
class VSCCollision;

bool VSCCollisionTest(VSCCollisionReceiver* receiver, VSCCollisionCreator* creator, VSCCollision** collision);

#endif
