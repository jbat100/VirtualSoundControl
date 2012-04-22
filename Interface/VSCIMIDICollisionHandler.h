//
//  VSCCollissionReceiver.h
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 4/21/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#ifndef _VSCI_MIDI_COLLISION_HANDLER_H_
#define _VSCI_MIDI_COLLISION_HANDLER_H_

#include <BulletDynamics/btBulletDynamicsCommon.h>
#include <boost/shared_ptr.hpp>
#include <vector>

#include "VSCICollisionObserver.h"
#include "VSCICollisionMapping.h

class VSCIMIDICollisionHandler;
typedef boost::shared_ptr<VSCIMIDICollisionHandler> VSCIMIDICollisionHandlerPtr;

class VSCIMIDICollisionHandler : public VSCICollisionObserver {
    
public:
    
    /**
     *  VSCICollisionObserver
     */
    virtual void collisionStarted(void* sender, const VSCICollision& collision);    
    virtual void collisionEnded(void* sender, const VSCICollision& collision);
    
    void setVelocityMapper(VSCICollisionVelocityMapperPtr mapper);
    void setVelocityMapper(VSCICollisionVelocityMapperPtr mapper);
    
private:
    
    VSCICollisionVelocityMapperPtr              _velocityMapper;
    VSCICollisionChannelContributionMapperPtr   _channelContributionMapper;
    
    VSCMIDIOutputPtr                            _midiOutput;
};

#endif
