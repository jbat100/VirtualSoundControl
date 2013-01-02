
#include "VSCOBMouseBindings.h"
#include "OIS.h"

#include <boost/pointer_cast.hpp>

VSC::OB::MouseBindings::SPtr VSC::OB::MouseBindings::generateDefaultBindings(void)
{
    using namespace OIS;
    
    MouseBindings::SPtr mouseBindings = MouseBindings::SPtr(new MouseBindings);
    
    mouseBindings->setBinding(MouseAction::CameraMove,      VSC::Mouse::Combination(MB_Right));
    mouseBindings->setBinding(MouseAction::CameraMove,      VSC::Mouse::Combination(MB_Left, OIS::Keyboard::Ctrl));
    mouseBindings->setBinding(MouseAction::ObjectGrab,      VSC::Mouse::Combination(MB_Left));
    mouseBindings->setBinding(MouseAction::ObjectImpulse,   VSC::Mouse::Combination(MB_Middle));
    
    return mouseBindings;
}

void VSC::OB::MouseBound::setOBMouseBindings(VSC::OB::MouseBindings::SPtr bindings)
{
    BaseBindings::SPtr baseMouseBinding = boost::dynamic_pointer_cast<BaseBindings>(bindings);
    this->setBindings(baseMouseBinding);
}

VSC::OB::MouseBindings::SPtr VSC::OB::MouseBound::getOBMouseBindings(void)
{
    BaseBindings::SPtr baseMouseBinding = this->getBindings();
    return boost::dynamic_pointer_cast<VSC::OB::MouseBindings>(baseMouseBinding);
}

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



