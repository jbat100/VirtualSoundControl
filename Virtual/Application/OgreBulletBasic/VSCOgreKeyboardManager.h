
#ifndef _VSC_OGRE_KEYBOARD_MANAGER_H_
#define _VSC_OGRE_KEYBOARD_MANAGER_H_

#include "VSCUI.h"
#include "VSCBindings.h"
#include "VSCOgreKeyboardAction.h"
#include "VSCOgreKeyBindings.h"

#include <boost/shared_ptr.hpp>

class VSCOgreKeyboardManager
{
    
public:
    
    typedef boost::shared_ptr<VSCOgreKeyboardManager> SPtr;
    
    VSCOgreKeyboardManager();
    ~VSCOgreKeyboardManager();
    
    VSCOgreKeyBindings::SPtr generateDefaultBindings(void);
    
private:
    
    
};

#endif//_VSC_OGRE_KEYBOARD_MANAGER_H_

