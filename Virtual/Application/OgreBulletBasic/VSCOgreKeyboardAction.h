
#ifndef _VSC_OGRE_KEYBOARD_ACTION_H_
#define _VSC_OGRE_KEYBOARD_ACTION_H_

#include <string>

class VSCOgreKeyboardAction
{
    
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
        Shoot,
        Drop
        
    };
    
    static std::string stringForKey(Key key);
    
};

#endif//_VSC_OGRE_KEYBOARD_H_

