
#ifndef _VSC_OB_KEY_BINDINGS_H_
#define _VSC_OB_KEY_BINDINGS_H_

#include "VSCUI.h"  
#include "VSCBindings.hpp"
#include "VSCBound.hpp"
#include "VSCOBKeyboardAction.h"

#include <boost/shared_ptr.hpp>

#include <set>

namespace VSC {
    
    namespace OB {

        class KeyBindings : public VSC::Bindings<KeyboardAction::Key, VSC::Keyboard::Combination>
        {
            
        public:
            
            typedef boost::shared_ptr<VSC::OB::KeyBindings> SPtr;
            
            KeyBindings() {}
            virtual ~KeyBindings() {}
            
        };


        class KeyBound : private VSC::Bound<KeyboardAction::Key, VSC::Keyboard::Combination> 
        {
            
        public:
            
            KeyBound() {}
            virtual ~KeyBound() {}
            
            void                    setOgreKeyBindings(KeyBindings::SPtr keyBindings);
            KeyBindings::SPtr       getOgreKeyBindings(void);
            
        private:
            
            typedef VSC::Bindings<KeyboardAction::Key, VSC::Keyboard::Combination> BaseBindings;
            
        };
        
    }
}
 
 

#endif//_VSC_OB_KEY_BINDINGS_H_

