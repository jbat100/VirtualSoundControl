

#ifndef _VSC_OB_OSX_APPLICATION_BRIDGE_H_
#define _VSC_OB_OSX_APPLICATION_BRIDGE_H_

#import <Ogre/Ogre.h>

#include "VSCOBApplication.h"

namespace VSC {
    
    namespace OB {

        class OSXApplicationBridge : public Application::Bridge {
            
            OSXApplicationBridge();
            
            virtual Ogre::Root* createOgreRoot();
            
        }
        
    }
}

#endif //_VSC_OB_OSX_APPLICATION_BRIDGE_H_
