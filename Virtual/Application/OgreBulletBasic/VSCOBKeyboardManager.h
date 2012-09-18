
#ifndef _VSC_OGRE_KEYBOARD_MANAGER_H_
#define _VSC_OGRE_KEYBOARD_MANAGER_H_

#include "VSCUI.h"
#include "VSCBindings.hpp"
#include "VSCOgreKeyboardAction.h"
#include "VSCOgreKeyBindings.h"

#include <boost/shared_ptr.hpp>

class VSC::OB::KeyboardManager
{
    
public:
    
    typedef boost::shared_ptr<VSC::OB::KeyboardManager> SPtr;
    
    VSC::OB::KeyboardManager();
    ~VSC::OB::KeyboardManager();
    
    VSC::OB::KeyBindings::SPtr generateDefaultBindings(void);
    
private:
    
    
};

#endif//_VSC_OGRE_KEYBOARD_MANAGER_H_

