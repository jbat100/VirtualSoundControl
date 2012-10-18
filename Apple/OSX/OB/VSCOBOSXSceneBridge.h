

#ifndef _VSC_OB_OSX_SCENE_BRIDGE_H_
#define _VSC_OB_OSX_SCENE_BRIDGE_H_

#import <Ogre/Ogre.h>

#include "VSCOBScene.h"
#include "VSCOBDisplay.h"

namespace VSC {
    
    namespace OB {

        class OSXSceneBridge : public Scene::Bridge
        {
            
        public:
            
            Display::SPtr createDisplayWithView(void* view);

        };
        
    }
}

#endif //_VSC_OB_OSX_SCENE_BRIDGE_H_
