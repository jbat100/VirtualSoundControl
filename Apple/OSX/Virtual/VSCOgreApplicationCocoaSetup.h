

#ifndef _VSC_OGRE_APPLICATION_COCOA_SETUP_H_
#define _VSC_OGRE_APPLICATION_COCOA_SETUP_H_

#import <Ogre/Ogre.h>

class VSCOgreApplication;

class VSCOgreApplicationCocoaSetup
{
    
public:
    
    VSCOgreApplicationCocoaSetup(void* ogreView = 0);
    ~VSCOgreApplicationCocoaSetup();
    
    bool setup(VSCOgreApplication* ogreApplication);
    
    void setOgreView(void* ogreView);
    void* getOgreView(OgreView);
    
    Ogre::RenderWindow* getRenderWindow(void);

private:
    
    void* mOgreView;

};

#endif//_VSC_OGRE_APPLICATION_COCOA_SETUP_H_
