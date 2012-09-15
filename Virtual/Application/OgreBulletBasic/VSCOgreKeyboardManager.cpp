
#include "VSCOgreKeyboardManager.h"
#include "VSCUI.h"
#include "VSCBindings.hpp"
#include "VSCOgreKeyboardAction.h"

#include <boost/assert.hpp>

VSCOgreKeyboardManager::VSCOgreKeyboardManager()
{
    
}

VSCOgreKeyboardManager::~VSCOgreKeyboardManager()
{
    
}

VSCOgreKeyBindings::SPtr VSCOgreKeyboardManager::generateDefaultBindings(void)
{
    using namespace OIS;
    
    VSCOgreKeyBindings::SPtr keyBindings = VSCOgreKeyBindings::SPtr(new VSCOgreKeyBindings);
    
    keyBindings->setBinding(VSCOgreKeyboardAction::Quit,                         VSCKeyboard::Combination(KC_ESCAPE));
    keyBindings->setBinding(VSCOgreKeyboardAction::SaveScreenShot,               VSCKeyboard::Combination(KC_SYSRQ));
    
    keyBindings->setBinding(VSCOgreKeyboardAction::ToggleDisplayWireFrame,       VSCKeyboard::Combination(KC_T));
    keyBindings->setBinding(VSCOgreKeyboardAction::ToggleDisplayAABB,            VSCKeyboard::Combination(KC_1));
    keyBindings->setBinding(VSCOgreKeyboardAction::ToggleDisplayContactPoints,   VSCKeyboard::Combination(KC_3));
    keyBindings->setBinding(VSCOgreKeyboardAction::ToggleDeactivation,           VSCKeyboard::Combination(KC_4));
    keyBindings->setBinding(VSCOgreKeyboardAction::ToggleHelpText,               VSCKeyboard::Combination(KC_5));
    keyBindings->setBinding(VSCOgreKeyboardAction::ToggleDrawText,               VSCKeyboard::Combination(KC_6));
    keyBindings->setBinding(VSCOgreKeyboardAction::ToggleProfileTimings,         VSCKeyboard::Combination(KC_7));
    keyBindings->setBinding(VSCOgreKeyboardAction::ToggleSatComparison,          VSCKeyboard::Combination(KC_8));
    keyBindings->setBinding(VSCOgreKeyboardAction::ToggleBulletLCP,              VSCKeyboard::Combination(KC_9));
    keyBindings->setBinding(VSCOgreKeyboardAction::ToggleCCD,                    VSCKeyboard::Combination(KC_0));
    
    keyBindings->setBinding(VSCOgreKeyboardAction::ToggleSimulationPause,        VSCKeyboard::Combination(KC_P));
    keyBindings->setBinding(VSCOgreKeyboardAction::SimulationStep,               VSCKeyboard::Combination(KC_M));
    
    keyBindings->setBinding(VSCOgreKeyboardAction::IncrementShootSpeed,          VSCKeyboard::Combination(KC_ADD));
    keyBindings->setBinding(VSCOgreKeyboardAction::DecrementShootSpeed,          VSCKeyboard::Combination(KC_MINUS));
    
    keyBindings->setBinding(VSCOgreKeyboardAction::ShootCube,                    VSCKeyboard::Combination(KC_B));
    keyBindings->setBinding(VSCOgreKeyboardAction::ShootSphere,                  VSCKeyboard::Combination(KC_N));
    keyBindings->setBinding(VSCOgreKeyboardAction::ShootCylinder,                VSCKeyboard::Combination(KC_H));
    keyBindings->setBinding(VSCOgreKeyboardAction::ShootCone,                    VSCKeyboard::Combination(KC_G));
    keyBindings->setBinding(VSCOgreKeyboardAction::DropCube,                     VSCKeyboard::Combination(KC_J));
    keyBindings->setBinding(VSCOgreKeyboardAction::DropSphere,                   VSCKeyboard::Combination(KC_K));
    keyBindings->setBinding(VSCOgreKeyboardAction::DropCylinder,                 VSCKeyboard::Combination(KC_U));
    keyBindings->setBinding(VSCOgreKeyboardAction::DropCone,                     VSCKeyboard::Combination(KC_I));
    
    keyBindings->setBinding(VSCOgreKeyboardAction::MoveCameraForward,            VSCKeyboard::Combination(KC_W));
    keyBindings->setBinding(VSCOgreKeyboardAction::MoveCameraBackward,           VSCKeyboard::Combination(KC_S));
    keyBindings->setBinding(VSCOgreKeyboardAction::MoveCameraRight,              VSCKeyboard::Combination(KC_D));
    keyBindings->setBinding(VSCOgreKeyboardAction::MoveCameraLeft,               VSCKeyboard::Combination(KC_A));
    
    return keyBindings;
    
}


