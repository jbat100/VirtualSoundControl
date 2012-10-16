
#ifndef _VSC_OGRE_KEYBOARD_ACTION_H_
#define _VSC_OGRE_KEYBOARD_ACTION_H_

#include <string>
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
        
    }
}

#endif//_VSC_OGRE_KEYBOARD_ACTION_H_

