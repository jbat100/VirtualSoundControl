
#ifndef _VSC_OB_MOUSE_BINDINGS_H_
#define _VSC_OB_MOUSE_BINDINGS_H_

#include "VSCUI.h"  
#include "VSCBindings.hpp"
#include "VSCBound.hpp"

#include <boost/shared_ptr.hpp>

#include <set>

namespace VSC {
    
    namespace OB {
        
        class MouseAction
        {
            
        public:
            
            enum Key
            {
                None = 0,
                
                CameraMove,
                
                ObjectGrab,
                ObjectImpulse,
                ObjectSelect,
                ObjectTranslate,
                ObjectRotate,
                
            };
            
            typedef std::set<Key>   KeySet;
            
            static std::string stringForKey(Key key);
            
        };
        
        std::ostream& operator << (std::ostream& stream, const MouseAction::Key& key);

        class MouseBindings : public VSC::Bindings<MouseAction::Key, VSC::Mouse::Combination>
        {
            
        public:
            
            typedef boost::shared_ptr<VSC::OB::MouseBindings> SPtr;
            
            MouseBindings() {}
            virtual ~MouseBindings() {}
            
            static MouseBindings::SPtr generateDefaultBindings(void);
            
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

