
#ifndef _VSC_OGRE_KEYBOARD_MANAGER_H_
#define _VSC_OGRE_KEYBOARD_MANAGER_H_

#include "VSCUI.h"
#include "VSCBindings.hpp"
#include "VSCOBKeyboardAction.h"
#include "VSCOBKeyBindings.h"

#include <boost/shared_ptr.hpp>

namespace VSC {
    
    namespace OB {

        class KeyboardManager
        {
            
        public:
            
            typedef boost::shared_ptr<KeyboardManager> SPtr;
            
            KeyboardManager();
            ~KeyboardManager();
            
            KeyBindings::SPtr generateDefaultBindings(void);
            
        private:
            
            
        };
        
    }
}

#endif//_VSC_OGRE_KEYBOARD_MANAGER_H_

