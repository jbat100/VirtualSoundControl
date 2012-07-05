/*
-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2006 Torus Knot Software Ltd
Also see acknowledgements in Readme.html

You may use this sample code for anything you like, it is not covered by the
LGPL like the rest of the engine.
-----------------------------------------------------------------------------
*/
/*
-----------------------------------------------------------------------------
Filename:    VSCOgreApplication.h
Description: Base class for all the OGRE examples
-----------------------------------------------------------------------------
*/

#ifndef _VSC_OGRE_APPLICATION_H_
#define _VSC_OGRE_APPLICATION_H_

#include <Ogre/Ogre.h>
#include <Ogre/OgreConfigFile.h>
#include "VSCOgreInputListener.h"

class VSCOgreInputAdapter;
class VSCOgreApplicationCocoaSetup;

/** Base class which manages the standard startup of an Ogre application.
    Designed to be subclassed for specific examples if required.
*/
class VSCOgreApplication : public VSCOgreInputListener
{
    
public:
    
    /// Standard constructor
    VSCOgreApplication();
    /// Standard destructor
    virtual ~VSCOgreApplication();

    VSCOgreInputAdapter* getInputAdapter(void) {return mInputAdapter;}
    void setInputAdapter(VSCOgreInputAdapter* adapter) {mInputAdapter = adapter;}
    
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
    friend class VSCOgreApplicationCocoaSetup;
    virtual bool setupWithOgreView(void* ogreView);
#else   
    /// Start the example
    virtual void go(void);
    // These internal methods package up the stages in the startup process
    /** Sets up the application - returns false if the user chooses to abandon configuration. */
    virtual bool setup(void);
#endif
    
    Ogre::Root* getRoot(void);

protected:
    
    Ogre::Root                      *mRoot;
    Ogre::Camera                    *mCamera;
    Ogre::SceneManager              *mSceneMgr;
    Ogre::RenderWindow              *mWindow;
	Ogre::String                    mResourcePath;
    VSCOgreInputAdapter             *mInputAdapter;
    
    /*
     *  NOTE: This is probably where the fun is in terms of making the application run on Cocoa
     */
    
    /** Configures the application - returns false if the user chooses to abandon configuration. */
    virtual bool configure(void);
    virtual void chooseSceneManager(void);
    virtual void createCamera(void);
    virtual void createFrameListener(void);

    virtual void createScene(void) = 0;    // pure virtual - this has to be overridden
    virtual void destroyScene(void){}    // Optional to override this
    virtual void createViewports(void);

    /// Method which will define the source of resources (other than current folder)
    virtual void setupResources(void);
	/// Optional override method where you can create resource listeners (e.g. for loading screens)
	virtual void createResourceListener(void);

	/// Optional override method where you can perform resource group loading
	/// Must at least do ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	virtual void loadResources(void);

};


#endif//_VSC_EXAMPLE_APPLICATION_H_
