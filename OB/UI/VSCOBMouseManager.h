
#ifndef _VSC_OGRE_MOUSE_MANAGER_H_
#define _VSC_OGRE_MOUSE_MANAGER_H_

#include "VSCUI.h"
#include "VSCBindings.hpp"
#include "VSCOBMouseAction.h"
#include "VSCOBMouseBindings.h"

#include <boost/shared_ptr.hpp>

namespace VSC {
    
    namespace OB {

        class MouseManager
        {
            
        public:
            
            typedef boost::shared_ptr<KeyboardManager> SPtr;
            
            MouseManager();
            ~MouseManager();
            
            MouseBindings::SPtr generateDefaultBindings(void);
            
        };
        
    }
}

#endif//_VSC_OGRE_KEYBOARD_MANAGER_H_

