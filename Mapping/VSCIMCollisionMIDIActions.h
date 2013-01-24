
#ifndef _VSC_IM_COLLISION_MIDI_ACTION_H_
#define _VSC_IM_COLLISION_MIDI_ACTION_H_

#include "VSCOB.h"
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
            
            typedef boost::shared_ptr<CollisionMIDIAction> SPtr;
            
            CollisionMIDIAction();
            
            MIDI::Output::SPtr getMIDIOutput(void) {return mMIDIOutput;}
            void setMIDIOutput(MIDI::Output::SPtr output) {mMIDIOutput = output;}
            
            unsigned int getChannel(void) {return mChannel;}
            void setChannel(unsigned int chan) {mChannel = chan;}
            
        private:
            
            MIDI::Output::SPtr  mMIDIOutput;
            
            unsigned int mChannel;
            
        };
        
        /*
         *  A abstract class for MIDI Control Actions
         */
        
        class CollisionMIDIControlAction : public CollisionMIDIAction {
            
        public:
            
            typedef boost::shared_ptr<CollisionMIDIControlAction> SPtr;
            
            CollisionMIDIControlAction();
            
            MIDI::ControlNumber getControlNumber(void) {return mControlNumber;}
            void setControlNumber(MIDI::ControlNumber number) {mControlNumber = number;}
            
        private:
            
            MIDI::ControlNumber mControlNumber;
            
        };
        
        /*
         *  Generates a note on task
         */
        
        class CollisionMIDINoteOnAction : public CollisionMIDIAction
        {
            
        public:
            
            CollisionMIDINoteOnAction();
            
            virtual void createDefaultMappings();
            
            virtual Tasks generateTasksForCollision(OB::Element_SPtr element, OB::Collision_SPtr collision);
        };
        
        /*
         *  Generates a note off task
         */
        
        class CollisionMIDINoteOffAction : public CollisionMIDIAction
        {
            
        public:
            
            CollisionMIDINoteOffAction();
            
            virtual void createDefaultMappings();
            
            virtual Tasks generateTasksForCollision(OB::Element_SPtr element, OB::Collision_SPtr collision);
        };
        
        /*
         *  Generates a note on task followed by a note off task after a mapping derived duration
         */
        
        class CollisionMIDINoteOnAndOffAction : public CollisionMIDIAction
        {
            
        public:
            
            CollisionMIDINoteOnAndOffAction();
            
            virtual void createDefaultMappings();
            
            virtual Tasks generateTasksForCollision(OB::Element_SPtr element, OB::Collision_SPtr collision);
            
        };
        
        class CollisionMIDIControlChangeAction : public CollisionMIDIControlAction
        {
            
        public:
            
            CollisionMIDIControlChangeAction();
            
            virtual void createDefaultMappings();
            
            virtual Tasks generateTasksForCollision(OB::Element_SPtr element, OB::Collision_SPtr collision);
            
        };
        
        /*
         *  Utilies and shortcuts
         */
        
        bool collisionActionIsMIDI(CollisionAction::SPtr collisionAction);
    
        bool collisionActionIsMIDIControl(CollisionAction::SPtr collisionAction);

    }
    
}

#endif // _VSC_IM_COLLISION_MIDI_ACTION_H_
