
#ifndef _VSC_OB_MOUSE_BINDINGS_H_
#define _VSC_OB_MOUSE_BINDINGS_H_

#include "VSCUI.h"  
#include "VSCBindings.hpp"
#include "VSCBound.hpp"
#include "VSCOBMouseAction.h"

#include <boost/shared_ptr.hpp>

#include <set>

namespace VSC {
    
    namespace OB {

        class MouseBindings : public VSC::Bindings<MouseAction::Key, VSC::Mouse::Combination>
        {
            
        public:
            
            typedef boost::shared_ptr<VSC::OB::MouseBindings> SPtr;
            
            MouseBindings() {}
            virtual ~MouseBindings() {}
            
        };


        class MouseBound : private VSC::Bound<MouseAction::Key, VSC::Mouse::Combination>
        {
            
        public:
            
            MouseBound() {}
            virtual ~MouseBound() {}
            
            void                    setOBMouseBindings(MouseBindings::SPtr bindings);
            MouseBindings::SPtr     getOBMouseBindings(void);
            
        private:
            
            typedef VSC::Bindings<MouseAction::Key, VSC::Mouse::Combination> BaseBindings;
            
        };
        
    }
}
 
 

#endif//_VSC_OB_KEY_BINDINGS_H_

