
#include "VSCOBKeyboardAction.h"

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


