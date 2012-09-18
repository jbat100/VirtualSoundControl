

#ifndef _VSC_OGRE_APPLICATION_COCOA_SETUP_H_
#define _VSC_OGRE_APPLICATION_COCOA_SETUP_H_

#import <Ogre/Ogre.h>

class VSC::OB::ApplicationBase;
class VSC::OB::InputListener;
class VSC::OB::OSXInputAdapter;

class VSC::OB::OSXApplicationSetup
{
    
public:
    
    static bool setupApplicationWithOgreView(VSC::OB::ApplicationBase* ogreApplication, void* ogreView);
    static Ogre::RenderWindow* getRenderWindow(void* ogreView);
    static VSC::OB::OSXInputAdapter* createCocoaInputAdapter(void);

};

#endif//_VSC_OGRE_APPLICATION_COCOA_SETUP_H_
