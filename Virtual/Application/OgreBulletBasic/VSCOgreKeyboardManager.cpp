
#include "VSCOgreKeyboardManager.h"
#include "VSCUI.h"
#include "VSCBindings.hpp"
#include "VSCOgreKeyboardAction.h"

#include <boost/assert.hpp>

VSC::OB::KeyboardManager::VSC::OB::KeyboardManager()
{
    
}

VSC::OB::KeyboardManager::~VSC::OB::KeyboardManager()
{
    
}

VSC::OB::KeyBindings::SPtr VSC::OB::KeyboardManager::generateDefaultBindings(void)
{
    using namespace OIS;
    
    VSC::OB::KeyBindings::SPtr keyBindings = VSC::OB::KeyBindings::SPtr(new VSC::OB::KeyBindings);
    
    keyBindings->setBinding(VSC::OB::KeyboardAction::Quit,                         VSC::Keyboard::Combination(KC_ESCAPE));
    keyBindings->setBinding(VSC::OB::KeyboardAction::SaveScreenShot,               VSC::Keyboard::Combination(KC_SYSRQ));
    
    keyBindings->setBinding(VSC::OB::KeyboardAction::ToggleDisplayWireFrame,       VSC::Keyboard::Combination(KC_T));
    keyBindings->setBinding(VSC::OB::KeyboardAction::ToggleDisplayAABB,            VSC::Keyboard::Combination(KC_1));
    keyBindings->setBinding(VSC::OB::KeyboardAction::ToggleDisplayContactPoints,   VSC::Keyboard::Combination(KC_3));
    keyBindings->setBinding(VSC::OB::KeyboardAction::ToggleDeactivation,           VSC::Keyboard::Combination(KC_4));
    keyBindings->setBinding(VSC::OB::KeyboardAction::ToggleHelpText,               VSC::Keyboard::Combination(KC_5));
    keyBindings->setBinding(VSC::OB::KeyboardAction::ToggleDrawText,               VSC::Keyboard::Combination(KC_6));
    keyBindings->setBinding(VSC::OB::KeyboardAction::ToggleProfileTimings,         VSC::Keyboard::Combination(KC_7));
    keyBindings->setBinding(VSC::OB::KeyboardAction::ToggleSatComparison,          VSC::Keyboard::Combination(KC_8));
    keyBindings->setBinding(VSC::OB::KeyboardAction::ToggleBulletLCP,              VSC::Keyboard::Combination(KC_9));
    keyBindings->setBinding(VSC::OB::KeyboardAction::ToggleCCD,                    VSC::Keyboard::Combination(KC_0));
    
    keyBindings->setBinding(VSC::OB::KeyboardAction::ToggleSimulationPause,        VSC::Keyboard::Combination(KC_P));
    keyBindings->setBinding(VSC::OB::KeyboardAction::SimulationStep,               VSC::Keyboard::Combination(KC_M));
    
    keyBindings->setBinding(VSC::OB::KeyboardAction::IncrementShootSpeed,          VSC::Keyboard::Combination(KC_ADD));
    keyBindings->setBinding(VSC::OB::KeyboardAction::DecrementShootSpeed,          VSC::Keyboard::Combination(KC_MINUS));
    
    keyBindings->setBinding(VSC::OB::KeyboardAction::ShootCube,                    VSC::Keyboard::Combination(KC_B));
    keyBindings->setBinding(VSC::OB::KeyboardAction::ShootSphere,                  VSC::Keyboard::Combination(KC_N));
    keyBindings->setBinding(VSC::OB::KeyboardAction::ShootCylinder,                VSC::Keyboard::Combination(KC_H));
    keyBindings->setBinding(VSC::OB::KeyboardAction::ShootCone,                    VSC::Keyboard::Combination(KC_G));
    keyBindings->setBinding(VSC::OB::KeyboardAction::DropCube,                     VSC::Keyboard::Combination(KC_J));
    keyBindings->setBinding(VSC::OB::KeyboardAction::DropSphere,                   VSC::Keyboard::Combination(KC_K));
    keyBindings->setBinding(VSC::OB::KeyboardAction::DropCylinder,                 VSC::Keyboard::Combination(KC_U));
    keyBindings->setBinding(VSC::OB::KeyboardAction::DropCone,                     VSC::Keyboard::Combination(KC_I));
    
    keyBindings->setBinding(VSC::OB::KeyboardAction::MoveCameraForward,            VSC::Keyboard::Combination(KC_W));
    keyBindings->setBinding(VSC::OB::KeyboardAction::MoveCameraBackward,           VSC::Keyboard::Combination(KC_S));
    keyBindings->setBinding(VSC::OB::KeyboardAction::MoveCameraRight,              VSC::Keyboard::Combination(KC_D));
    keyBindings->setBinding(VSC::OB::KeyboardAction::MoveCameraLeft,               VSC::Keyboard::Combination(KC_A));
    
    return keyBindings;
    
}


