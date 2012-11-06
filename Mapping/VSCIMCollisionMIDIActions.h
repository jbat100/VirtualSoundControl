//
//  VSCIMAction.h
//
//  Created by Jonathan Thorpe on 4/21/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#ifndef _VSC_IM_COLLISION_MIDI_ACTION_H_
#define _VSC_IM_COLLISION_MIDI_ACTION_H_

#include "VSCOBScene.h"
#include "VSCIMCollisionMapping.h"
#include "VSCIMCollisionMapped.h"
#include "VSCIMCollisionAction.h"
#include "VSCMIDIOutputChannel.h"

#include <boost/shared_ptr.hpp>

#include <vector>

namespace VSC {
    
    namespace IM {
        
        
        
        /*
         *  A abstract class for all MIDI Actions
         */
        
        class CollisionMIDIAction : public CollisionAction {
            
        public:
            
            CollisionMIDIAction();
            
            MIDI::Output::SPtr getMIDIOuput(void) {return mMIDIOutput;}
            void setMIDIOutput(MIDI::Output::SPtr output) {mMIDIOutput = output;}
            
            unsigned int getChannel(void) {return mChannel;}
            void setChannel(unsigned int chan) {mChannel = chan;}
            
        private:
            
            MIDI::Output::SPtr  mMIDIOutput;
            
            unsigned int mChannel;
            
        };
        
        class CollisionMIDINoteOnAction : public CollisionMIDIAction
        {
            CollisionMIDINoteOnAction();
            
            virtual Task::SPtr createTaskForCollision(OB::Scene::Element::SPtr element,
                                                      OB::Scene::Collision::SPtr collision);
        };
        
        class CollisionMIDINoteOffAction : public CollisionMIDIAction
        {
            CollisionMIDINoteOffAction();
            
            virtual Task::SPtr createTaskForCollision(OB::Scene::Element::SPtr element,
                                                      OB::Scene::Collision::SPtr collision);
        };
        
        class CollisionMIDINoteOnAndOffAction : public CollisionMIDIAction
        {
            CollisionMIDINoteOnAndOffAction();
            
            virtual Task::SPtr createTaskForCollision(OB::Scene::Element::SPtr element,
                                                      OB::Scene::Collision::SPtr collision);
        };
        
        class CollisionMIDIControlChangeAction : public CollisionMIDIAction
        {
            CollisionMIDIControlChangeAction();
            
            virtual Task::SPtr createTaskForCollision(OB::Scene::Element::SPtr element,
                                                      OB::Scene::Collision::SPtr collision);
            
            MIDI::ControlNumber getControlNumber(void) {return mControlNumber;}
            void setControlNumber(MIDI::ControlNumber number) {mControlNumber = number;}
            
        private:
            
            MIDI::ControlNumber mControlNumber;
        };

    }
    
}

#endif // _VSC_IM_COLLISION_MIDI_ACTION_H_
