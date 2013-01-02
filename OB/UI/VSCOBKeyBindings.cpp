
#include "VSCOBKeyBindings.h"

#include <boost/pointer_cast.hpp>


VSC::OB::KeyBindings::SPtr VSC::OB::KeyBindings::generateDefaultBindings(void)
{
    using namespace OIS;
    
    KeyBindings::SPtr keyBindings = KeyBindings::SPtr(new KeyBindings);
    
    keyBindings->setBinding(KeyboardAction::Quit,                         VSC::Keyboard::Combination(KC_ESCAPE));
    keyBindings->setBinding(KeyboardAction::SaveScreenShot,               VSC::Keyboard::Combination(KC_SYSRQ));
    
    keyBindings->setBinding(KeyboardAction::ToggleDisplayWireFrame,       VSC::Keyboard::Combination(KC_T));
    keyBindings->setBinding(KeyboardAction::ToggleDisplayAABB,            VSC::Keyboard::Combination(KC_1));
    keyBindings->setBinding(KeyboardAction::ToggleDisplayContactPoints,   VSC::Keyboard::Combination(KC_3));
    keyBindings->setBinding(KeyboardAction::ToggleDeactivation,           VSC::Keyboard::Combination(KC_4));
    keyBindings->setBinding(KeyboardAction::ToggleHelpText,               VSC::Keyboard::Combination(KC_5));
    keyBindings->setBinding(KeyboardAction::ToggleDrawText,               VSC::Keyboard::Combination(KC_6));
    keyBindings->setBinding(KeyboardAction::ToggleProfileTimings,         VSC::Keyboard::Combination(KC_7));
    keyBindings->setBinding(KeyboardAction::ToggleSatComparison,          VSC::Keyboard::Combination(KC_8));
    keyBindings->setBinding(KeyboardAction::ToggleBulletLCP,              VSC::Keyboard::Combination(KC_9));
    keyBindings->setBinding(KeyboardAction::ToggleCCD,                    VSC::Keyboard::Combination(KC_0));
    
    keyBindings->setBinding(KeyboardAction::ToggleSimulationPause,        VSC::Keyboard::Combination(KC_P));
    keyBindings->setBinding(KeyboardAction::SimulationStep,               VSC::Keyboard::Combination(KC_M));
    
    keyBindings->setBinding(KeyboardAction::IncrementShootSpeed,          VSC::Keyboard::Combination(KC_ADD));
    keyBindings->setBinding(KeyboardAction::DecrementShootSpeed,          VSC::Keyboard::Combination(KC_MINUS));
    
    keyBindings->setBinding(KeyboardAction::ShootCube,                    VSC::Keyboard::Combination(KC_B));
    keyBindings->setBinding(KeyboardAction::ShootSphere,                  VSC::Keyboard::Combination(KC_N));
    keyBindings->setBinding(KeyboardAction::ShootCylinder,                VSC::Keyboard::Combination(KC_H));
    keyBindings->setBinding(KeyboardAction::ShootCone,                    VSC::Keyboard::Combination(KC_G));
    keyBindings->setBinding(KeyboardAction::DropCube,                     VSC::Keyboard::Combination(KC_J));
    keyBindings->setBinding(KeyboardAction::DropSphere,                   VSC::Keyboard::Combination(KC_K));
    keyBindings->setBinding(KeyboardAction::DropCylinder,                 VSC::Keyboard::Combination(KC_U));
    keyBindings->setBinding(KeyboardAction::DropCone,                     VSC::Keyboard::Combination(KC_I));
    
    keyBindings->setBinding(KeyboardAction::MoveCameraForward,            VSC::Keyboard::Combination(KC_W));
    keyBindings->setBinding(KeyboardAction::MoveCameraBackward,           VSC::Keyboard::Combination(KC_S));
    keyBindings->setBinding(KeyboardAction::MoveCameraRight,              VSC::Keyboard::Combination(KC_D));
    keyBindings->setBinding(KeyboardAction::MoveCameraLeft,               VSC::Keyboard::Combination(KC_A));
    
    return keyBindings;
    
}

void VSC::OB::KeyBound::setOBKeyBindings(VSC::OB::KeyBindings::SPtr keyBindings)
{
    BaseBindings::SPtr baseKeyBinding = boost::dynamic_pointer_cast<BaseBindings>(keyBindings);
    this->setBindings(baseKeyBinding);
}

VSC::OB::KeyBindings::SPtr VSC::OB::KeyBound::getOBKeyBindings(void)
{
    BaseBindings::SPtr baseKeyBinding = this->getBindings();
    return boost::dynamic_pointer_cast<VSC::OB::KeyBindings>(baseKeyBinding);
}

std::string VSC::OB::KeyboardAction::stringForKey(KeyboardAction::Key key)
{
    switch (key) {
            
        case MoveCameraForward:
            return "Move camera forward";
            break;
            
        default:
            break;
    }
    
    return "No key string yet, be patient...";
}

std::ostream& VSC::OB::operator << (std::ostream& stream, const KeyboardAction::Key& key)
{
    stream << VSC::OB::KeyboardAction::stringForKey(key);
    
    return stream;
}

bool VSC::OB::KeyboardAction::actionThrowsDynamicObject(Key key)
{
    switch (key)
    {
        case ShootCube:
        case ShootSphere:
        case ShootCylinder:
        case ShootCone:
            return true;
            
        default:
            break;
    }
    return false;
}

bool VSC::OB::KeyboardAction::actionDropsDynamicObject(Key key)
{
    switch (key)
    {
        case DropCube:
        case DropSphere:
        case DropCylinder:
        case DropCone:
            return true;
            
        default:
            break;
    }
    return false;
}

