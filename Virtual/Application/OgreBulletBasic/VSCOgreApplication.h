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

#include "Ogre.h"
#include "OgreConfigFile.h"

#include "VSCOgreFrameListener.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
class VSCOgreApplicationCocoaSetup;
#endif


using namespace Ogre;

/** Base class which manages the standard startup of an Ogre application.
    Designed to be subclassed for specific examples if required.
*/
class VSCOgreApplication
{
    
public:
    
    enum SetupType {
        SetupTypeNone = 0,
        SetupTypeDefault,
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
        SetupTypeAppleCocoa
#endif
    };
    
    /// Standard constructor
    VSCOgreApplication(SetupType setupType = SetupTypeDefault);
    /// Standard destructor
    virtual ~VSCOgreApplication();
    /// Start the example
    virtual void go(void);
    
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
    friend class VSCOgreApplicationCocoaSetup;
#endif

protected:
    
    Root                    *mRoot;
    Camera                  *mCamera;
    SceneManager            *mSceneMgr;
    ExampleFrameListener    *mFrameListener;
    RenderWindow            *mWindow;
	Ogre::String            mResourcePath;
    SetupType               mSetupType;
    
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
    VSCOgreApplicationCocoaSetup*   mCocoaSetup;
#endif

    // These internal methods package up the stages in the startup process
    /** Sets up the application - returns false if the user chooses to abandon configuration. */
    virtual bool setup(void);
    
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
