

#ifndef _VSC_OGRE_APPLICATION_COCOA_SETUP_H_
#define _VSC_OGRE_APPLICATION_COCOA_SETUP_H_

#import <Ogre/Ogre.h>

class VSCOgreApplication;
class VSCOgreBulletInputListener;
class VSCOgreBulletCocoaInputAdapter;

class VSCOgreApplicationCocoaSetup
{
    
public:
    
    static bool setup(VSCOgreApplication* ogreApplication, void* ogreView);
    static Ogre::RenderWindow* getRenderWindow(void* ogreView);
    static bool setupAdapter(VSCOgreBulletCocoaInputAdapter* adapter, void* ogreView, VSCOgreBulletInputListener* inputListener);

};

#endif//_VSC_OGRE_APPLICATION_COCOA_SETUP_H_
