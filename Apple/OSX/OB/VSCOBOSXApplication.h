

#ifndef _VSC_OGRE_APPLICATION_COCOA_SETUP_H_
#define _VSC_OGRE_APPLICATION_COCOA_SETUP_H_

#import <Ogre/Ogre.h>

#include "VSCOBApplicationBase.h"

namespace VSC {
    
    namespace OB {

        class ApplicationBase;
        class InputListener;
        class OSXInputAdapter;

        class OSXApplicationSetup
        {
            
        public:
            
            static bool setupApplicationWithOgreView(ApplicationBase::SPtr ogreApplication, void* ogreView);

        };
        
    }
}

#endif//_VSC_OGRE_APPLICATION_COCOA_SETUP_H_
