/***************************************************************************
This source file is part of OGREBULLET
(Object-oriented Graphics Rendering Engine Bullet Wrapper)
For the latest info, see http://www.ogre3d.org/phpBB2addons/viewforum.php?f=10
Copyright (c) 2007 tuan.kuranes@gmail.com (Use it Freely, even Statically, but have to contribute any changes)
This source file is not LGPL, it's public source code that you can reuse.
-----------------------------------------------------------------------------*/
/*
VSCOgreBulletApplication.h runs the OgreBullet
demo scenes and switch between them.
*/
#ifndef _VSC_OGRE_BULLET_APPLICATION_H_
#define _VSC_OGRE_BULLET_APPLICATION_H_

// Include the OgreBullet interface which includes Ogre itself
#include "OgreBulletCollisions.h"
#include "OgreBulletDynamics.h"

#include "VSCOgreBulletListener.h"
#include "VSCOgreApplication.h"

#include <vector>

#if !(OGRE_VERSION <  ((1 << 16) | (3 << 8) | 0))
    using namespace OIS;
#endif //OGRE_VERSION eihort


class VSCOgreBulletApplication;


/**
 *  The test application, based on the Ogre example application for consistency
 */
class VSCOgreBulletApplication: public VSCOgreApplication,  public FrameListener
{
public:
	// Standard constructor/destructor
    VSCOgreBulletApplication(std::vector <OgreBulletListener *> *bulletListeners);
    ~VSCOgreBulletApplication();

    std::vector <VSCOgreBulletListener *> *getScenesList(){return mBulletListeners;};

protected:
    
	// Override stuff from the base class
    void createScene(void){};	
    void chooseSceneManager(void){};
    void createCamera(void){};
    void createViewports(void){};

    void createFrameListener(void);
	// Add the standard resources, plus our own pack
    void setupResources(void);
	void loadResources(void);

    bool frameStarted(const FrameEvent& evt);
    bool frameEnded(const FrameEvent& evt);

    bool switchListener(OgreBulletListener *newListener);

protected:
    
    VSCOgreBulletListener *mBulletListener;
    std::vector <VSCOgreBulletListener*> *mBulletListeners;

#if (OGRE_VERSION <  ((1 << 16) | (3 << 8) | 0))
    Ogre::InputReader       *mInput; 
    Ogre::EventProcessor    *mInputSystem; 
#else
    OIS::Keyboard           *mInput;
    OIS::Mouse              *mMouse;
    OIS::InputManager       *mInputSystem;
#endif

};

#endif //_VSC_OGRE_BULLET_APPLICATION_H_

