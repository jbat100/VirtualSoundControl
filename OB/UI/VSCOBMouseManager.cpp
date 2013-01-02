
#include "VSCOBMouseManager.h"
#include "VSCUI.h"
#include "VSCBindings.hpp"
#include "VSCOBMouseAction.h"

#include "OIS.h"

#include <boost/assert.hpp>

VSC::OB::MouseManager::MouseManager()
{
    
}

VSC::OB::MouseManager::~MouseManager()
{
    
}

VSC::OB::MouseBindings::SPtr VSC::OB::MouseManager::generateDefaultBindings(void)
{
    using namespace OIS;
    
    MouseBindings::SPtr mouseBindings = MouseBindings::SPtr(new MouseBindings);
    
    mouseBindings->setBinding(MouseAction::CameraMove,      VSC::Mouse::Combination(MB_Right));
    mouseBindings->setBinding(MouseAction::CameraMove,      VSC::Mouse::Combination(MB_Left, Keyboard::Ctrl));
    mouseBindings->setBinding(MouseAction::ObjectGrab,      VSC::Mouse::Combination(MB_Left));
    
    return mouseBindings;
    
}


