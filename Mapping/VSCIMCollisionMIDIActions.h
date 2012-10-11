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
            
            class PitchMapped {
                
            public:
                
                CollisionMapping::SPtr setPitchMapping() {return mPitchMapping;}
                void setPitchMapping(CollisionMapping::SPtr mapping) {mPitchMapping = mapping;}
                
            private:

                CollisionMapping::SPtr  mPitchMapping;
                
            };
            
            class VelocityMapped {
                
            public:
                
                CollisionMapping::SPtr getVelocityMapping() {return mVelocityMapping;}
                void setVelocityMapping(CollisionMapping::SPtr mapping) {mVelocityMapping = mapping;}
                
            private:
                
                CollisionMapping::SPtr  mVelocityMapping;
                
            };
            
            class DurationMapped {
                
            public:
                
                CollisionMapping::SPtr getDurationMapping() {return mDurationMapping;}
                void setDurationMapping(CollisionMapping::SPtr mapping) {mDurationMapping = mapping;}
                
            private:
                
                CollisionMapping::SPtr  mDurationMapping;
                
            };
            
            CollisionMIDIAction();
            CollisionMIDIAction(MIDI::OutputChannel::SPtr outputChannel);
            
            MIDI::OutputChannel::SPtr getOuputChannel(void) {return mOutputChannel;}
            void setOutpitChannel(MIDI::OutputChannel::SPtr outputChannel) {mOutputChannel = outputChannel;}
            
        private:
            
            MIDI::OutputChannel::SPtr  mOutputChannel;
            
        };
        
        class CollisionMIDINoteAction : public CollisionMIDIAction,
                                        public CollisionMIDIAction::PitchMapped,
                                        public CollisionMIDIAction::VelocityMapped
        {
            
        };
        
        class CollisionMIDINoteOnAction : public CollisionMIDINoteAction
        {
            virtual Task::SPtr createTaskForCollision(OB::Scene::Element::SPtr element,
                                                      OB::Scene::Collision::SPtr collision);
        };
        
        class CollisionMIDINoteOffAction : public CollisionMIDINoteAction
        {
            virtual Task::SPtr createTaskForCollision(OB::Scene::Element::SPtr element,
                                                      OB::Scene::Collision::SPtr collision);
        };
        
        class CollisionMIDINoteOnAndOffAction : public CollisionMIDINoteAction,
                                                public CollisionMIDIAction::DurationMapped
        {
            virtual Task::SPtr createTaskForCollision(OB::Scene::Element::SPtr element,
                                                      OB::Scene::Collision::SPtr collision);
        };

    }
    
}

#endif // _VSC_IM_COLLISION_MIDI_ACTION_H_
