
#ifndef _VSC_OGRE_KEYBOARD_ACTION_H_
#define _VSC_OGRE_KEYBOARD_ACTION_H_

#include <string>

class VSCOgreKeyboardAction
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
        DropCone
        
        /*
         *  Vehicle
         */
        
        VehicleSteerLeft,
        VehicleSteerRight,
        VehicleMoveForward,
        VehicleMoveBackward,
        VehicleIncrementEngineStyle,
        VehicleDecrementEngineStyle,
        
        
    };
    
    static std::string stringForKey(Key key);
    
};

#endif//_VSC_OGRE_KEYBOARD_H_

