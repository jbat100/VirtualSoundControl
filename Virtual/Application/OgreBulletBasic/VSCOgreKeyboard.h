
#ifndef _VSC_OGRE_KEYBOARD_H_
#define _VSC_OGRE_KEYBOARD_H_

class VSCOgreKeyboard 
{
    struct Combination {
        OIS::Keyboard::Modifier     modifier;
        OIS::KeyCode                keyCode;
    };
    
    enum Action 
    {
        ActionNone = 0,
        
        /*
         *  General
         */
        
        ActionQuit,
        ActionSaveScreenShot,
        
        /*
         *  Movement
         */
        
        ActionMoveCameraForward,
        ActionMoveCameraBackward,
        ActionMoveCameraRight,
        ActionMoveCameraLeft,
        
        /*
         *  Debug Display
         */
        
        ActionToggleDisplayWireFrame,
        ActionToggleDisplayAABB,
        ActionToggleDisplayContactPoints,
        ActionToggleDeactivation,
        ActionToggleHelpText,
        ActionToggleDrawText,
        ActionToggleProfileTimings,
        ActionToggleSatComparison,
        ActionToggleBulletLCP,
        ActionToggleCCD,
        
        /*
         *  Simulation
         */
        
        ActionToggleSimulationPause,
        ActionSimulationStep,
        
        /*
         *  Shooting
         */
        
        ActionIncrementShootSpeed,
        ActionDecrementShootSpeed,
        ActionShoot,
        ActionDrop
        
    };
    
private:
    
    
    
};

#endif//_VSC_OGRE_KEYBOARD_H_

