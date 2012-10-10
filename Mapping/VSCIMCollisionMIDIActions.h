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
            CollisionMIDIAction(OutputChannel::SPtr outputChannel);
            
            unsigned int getOuputChannel(void) {return mOutputChannel;}
            
        private:
            
            OutputChannel::SPtr  mOutputChannel;
            
        };
        
        /*
         *  MIDI note on and off actions are linked so that pitch can be 
         *  transmitted 
         */
        
        class CollisionMIDINoteOnAction : public CollisionMIDIAction {
            
        public:
            
            CollisionMapping::SPtr getNoteVelocityMapping() {return mNoteVelocityMapping;}
            void setNoteVelocityMapping(CollisionMapping::SPtr mapping) {mNoteVelocityMapping = mapping;}
            
        protected:
            
            virtual void internalPerform(OB::Scene::Element::SPtr element, OB::Scene::Collision::SPtr collision);
            
        private:
            
            CollisionMapping::SPtr  mNoteVelocityMapping;
            CollisionMapping::SPtr  mNotePitchMapping;
            
            unsigned int            mPerformedNotePitch;
            unsigned int            mPerformedNoteVelocity;
            
        }

    }
    
}

#endif // _VSC_IM_COLLISION_MIDI_ACTION_H_
