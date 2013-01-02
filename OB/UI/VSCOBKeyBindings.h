
#ifndef _VSC_OB_KEY_BINDINGS_H_
#define _VSC_OB_KEY_BINDINGS_H_

#include "VSCUI.h"  
#include "VSCBindings.hpp"
#include "VSCBound.hpp"

#include <boost/shared_ptr.hpp>

#include <set>

namespace VSC {
    
    namespace OB {
        
        class KeyboardAction
        {
            
        public:
            
            enum Key
            {
                None = 0,
                
                /*
                 *  General
                 */
                
                Quit,
                SaveScreenShot,
                
                /*
                 *  Movement
                 */
                
                MoveCameraForward,
                MoveCameraBackward,
                MoveCameraRight,
                MoveCameraLeft,
                
                /*
                 *  Debug Display
                 */
                
                ToggleDisplayWireFrame,
                ToggleDisplayAABB,
                ToggleFeaturesText,
                ToggleDisplayContactPoints,
                ToggleDeactivation,
                ToggleHelpText,
                ToggleDrawText,
                ToggleProfileTimings,
                ToggleSatComparison,
                ToggleBulletLCP,
                ToggleCCD,
                
                /*
                 *  Simulation
                 */
                
                ToggleSimulationPause,
                SimulationStep,
                
                /*
                 *  Shooting
                 */
                
                IncrementShootSpeed,
                DecrementShootSpeed,
                ShootCube,
                ShootSphere,
                ShootCylinder,
                ShootCone,
                DropCube,
                DropSphere,
                DropCylinder,
                DropCone,
                
                /*
                 *  Vehicle
                 */
                
                VehicleSteerLeft,
                VehicleSteerRight,
                VehicleMoveForward,
                VehicleMoveBackward,
                VehicleIncrementEngineStyle,
                VehicleDecrementEngineStyle,
                VehicleIncrementSteeringStyle,
                VehicleDecrementSteeringStyle,
                
                
            };
            
            typedef std::set<Key>   KeySet;
            
            static std::string stringForKey(Key key);
            
            static bool actionThrowsDynamicObject(Key key);
            static bool actionDropsDynamicObject(Key key);
            
        };
        
        std::ostream& operator << (std::ostream& stream, const KeyboardAction::Key& key);

        class KeyBindings : public VSC::Bindings<KeyboardAction::Key, VSC::Keyboard::Combination>
        {
            
        public:
            
            typedef boost::shared_ptr<VSC::OB::KeyBindings> SPtr;
            
            KeyBindings() {}
            virtual ~KeyBindings() {}
            
            static KeyBindings::SPtr generateDefaultBindings(void);
            
        };


        class KeyBound : private VSC::Bound<KeyboardAction::Key, VSC::Keyboard::Combination> 
        {
            
        public:
            
            KeyBound() {}
            virtual ~KeyBound() {}
            
            void                    setOBKeyBindings(KeyBindings::SPtr keyBindings);
            KeyBindings::SPtr       getOBKeyBindings(void);
            
        private:
            
            typedef VSC::Bindings<KeyboardAction::Key, VSC::Keyboard::Combination> BaseBindings;
            
        };
        
    }
}
 
 

#endif//_VSC_OB_KEY_BINDINGS_H_

