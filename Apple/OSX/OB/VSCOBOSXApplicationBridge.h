

#ifndef _VSC_OB_OSX_APPLICATION_BRIDGE_H_
#define _VSC_OB_OSX_APPLICATION_BRIDGE_H_

#import <Ogre/Ogre.h>

#include "VSCOBApplication.h"
#include "VSCOBDisplay.h"

namespace VSC {
    
    namespace OB {

        class OSXApplicationBridge : public Application::Bridge {
            
            friend class Application;
            
            OSXApplicationBridge(Application::SPtr application) : Application::Bridge(application) {}
            
            virtual Ogre::Root* createOgreRoot();
            
            virtual void setupDisplayWithView(Display::SPtr display, void* view);
            
        protected:
            
            OSXApplicationBridge();
            
        };
        
    }
}

#endif //_VSC_OB_OSX_APPLICATION_BRIDGE_H_
