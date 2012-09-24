
#include "VSCOBKeyboardAction.h"

std::string VSC::OB::KeyboardAction::stringForKey(Key key)
{
    return "No key string yet, be patient...";
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


