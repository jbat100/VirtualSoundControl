
#ifndef _VSC_OGRE_MOUSE_ACTION_H_
#define _VSC_OGRE_MOUSE_ACTION_H_

#include <string>
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
                ObjectSelect,
                ObjectTranslate,
                ObjectRotate
                
            };
            
            typedef std::set<Key>   KeySet;
            
            static std::string stringForKey(Key key);
            
        };
        
        std::ostream& operator << (std::ostream& stream, const MouseAction::Key& key);
        
    }
}

#endif//_VSC_OGRE_MOUSE_ACTION_H_

