

#ifndef _VSC_OGRE_APPLICATION_COCOA_SETUP_H_
#define _VSC_OGRE_APPLICATION_COCOA_SETUP_H_

#import <Ogre/Ogre.h>

namespace VSC {
    
    namespace OB {

        class ApplicationBase;
        class InputListener;
        class OSXInputAdapter;

        class OSXApplicationSetup
        {
            
        public:
            
            static bool setupApplicationWithOgreView(ApplicationBase* ogreApplication, void* ogreView);
            static Ogre::RenderWindow* getRenderWindow(void* ogreView);
            static OSXInputAdapter* createCocoaInputAdapter(void);

        };
        
    }
}

#endif//_VSC_OGRE_APPLICATION_COCOA_SETUP_H_
