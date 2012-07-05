

#ifndef _VSC_OGRE_APPLICATION_COCOA_SETUP_H_
#define _VSC_OGRE_APPLICATION_COCOA_SETUP_H_

#import <Ogre/Ogre.h>

class VSCOgreApplication;
class VSCOgreInputListener;
class VSCOgreBulletCocoaInputAdapter;

class VSCOgreApplicationCocoaSetup
{
    
public:
    
    static bool setupApplicationWithOgreView(VSCOgreApplication* ogreApplication, void* ogreView);
    static Ogre::RenderWindow* getRenderWindow(void* ogreView);
    static VSCOgreBulletCocoaInputAdapter* createCocoaInputAdapter(void);
    static bool setupCocoaInputAdapter(VSCOgreBulletCocoaInputAdapter* adapter, void* ogreView, VSCOgreInputListener* inputListener);

};

#endif//_VSC_OGRE_APPLICATION_COCOA_SETUP_H_
