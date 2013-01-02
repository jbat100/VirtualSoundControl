
#include "VSCOBMouseAction.h"

std::string VSC::OB::MouseAction::stringForKey(MouseAction::Key key)
{
    switch (key)
    {
        case CameraMove:
            return "Camera Move";
            
        default:
            break;
    }
    
    return "No key string yet, be patient...";
}

std::ostream& VSC::OB::operator << (std::ostream& stream, const MouseAction::Key& key)
{
    stream << VSC::OB::MouseAction::stringForKey(key);
    return stream;
}



