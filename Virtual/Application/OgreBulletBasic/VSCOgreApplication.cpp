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

#ifndef _VSC_EXAMPLE_APPLICATION_H_
#define _VSC_EXAMPLE_APPLICATION_H_

#include <Ogre/Ogre.h>
#include <Ogre/OgreConfigFile.h>

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
#include <OSX/macUtils.h>
#endif

#include "VSCOgreApplication.h"
#include "VSCOgreFrameListener.h"

using namespace Ogre;

VSCOgreApplication::VSCOgreApplication(SetupType setupType) :
mSetupType(setupType)
{
    mFrameListener = 0;
    mRoot = 0;
    mCocoaSetup = 0;
    // Provide a nice cross platform solution for locating the configuration files
    // On windows files are searched for in the current working directory, on OS X however
    // you must provide the full path, the helper function macBundlePath does this for us.
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
    mResourcePath = Ogre::macBundlePath() + "/Contents/Resources/";
#else
    mResourcePath = "";
#endif
}

/// Standard destructor
virtual ~VSCOgreApplication::VSCOgreApplication()
{
    if (mFrameListener)
        delete mFrameListener;
    if (mRoot)
        OGRE_DELETE mRoot;
}

/// Start the example
void VSCOgreApplication::go(void)
{
    
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
    if (mSetupType == SetupTypeAppleCocoa && mCocoaSetup != 0) {
        if (mCocoaSetup) {
            mCocoaSetup->setup(this);
        }
    }
    else if (!setup())
    {
        return;
    }
#else
    if (!setup())
        return;
#endif
    
    mRoot->startRendering();
    // clean up
    destroyScene();
}


// These internal methods package up the stages in the startup process
/** Sets up the application - returns false if the user chooses to abandon configuration. */
bool VSCOgreApplication::setup(void)
{
    
    
    if (mSetupType == SetupTypeDefault) {
        String pluginsPath;
        String pluginsName = "plugins.cfg";
        // only use plugins.cfg if not static
#ifndef OGRE_STATIC_LIB
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#ifdef _DEBUG
        pluginsName = "plugins_d.cfg";
#else
        pluginsName = "plugins.cfg";
#endif
#endif
        pluginsPath = mResourcePath + pluginsName;
#endif
        mRoot = OGRE_NEW Root(pluginsPath, mResourcePath + "ogre.cfg", mResourcePath + "Ogre.log");
        setupResources();
        bool carryOn = configure();
        if (!carryOn) return false;
        
        chooseSceneManager();
        createCamera();
        createViewports();
        
        // Set default mipmap level (NB some APIs ignore this)
        TextureManager::getSingleton().setDefaultNumMipmaps(5);
        
        createResourceListener(); // Create any resource listeners (for loading screens)
        loadResources();
        createScene();
        createFrameListener();
    }
        
    
    return true;
    
}

/*
 *  NOTE: This is probably where the fun is in terms of making the application run on Cocoa
 */

/** Configures the application - returns false if the user chooses to abandon configuration. */
bool VSCOgreApplication::configure(void)
{
    // Show the configuration dialog and initialise the system
    // You can skip this and use root.restoreConfig() to load configuration
    // settings if you were sure there are valid ones saved in ogre.cfg
    if(mRoot->showConfigDialog())
    {
        // If returned true, user clicked OK so initialise
        // Here we choose to let the system create a default rendering window by passing 'true'
        mWindow = mRoot->initialise(true);
        return true;
    }
    else
    {
        return false;
    }
}

void VSCOgreApplication::chooseSceneManager(void)
{
    // Create the SceneManager, in this case a generic one
    mSceneMgr = mRoot->createSceneManager(ST_GENERIC, "ExampleSMInstance");
}

void VSCOgreApplication::createCamera(void)
{
    // Create the camera
    mCamera = mSceneMgr->createCamera("PlayerCam");
    
    // Position it at 500 in Z direction
    mCamera->setPosition(Vector3(0,0,500));
    // Look back along -Z
    mCamera->lookAt(Vector3(0,0,-300));
    mCamera->setNearClipDistance(5);
    
}

void VSCOgreApplication::createFrameListener(void)
{
    mFrameListener= new VSCOgreFrameListener(mWindow, mCamera);
    mFrameListener->showDebugOverlay(true);
    mRoot->addFrameListener(mFrameListener);
}

void VSCOgreApplication::createViewports(void)
{
    // Create one viewport, entire window
    Viewport* vp = mWindow->addViewport(mCamera);
    vp->setBackgroundColour(ColourValue(0,0,0));
    // Alter the camera aspect ratio to match the viewport
    mCamera->setAspectRatio(Real(vp->getActualWidth()) / Real(vp->getActualHeight()));
}

/// Method which will define the source of resources (other than current folder)
void VSCOgreApplication::setupResources(void)
{
    // Load resource paths from config file
    ConfigFile cf;
    cf.load(mResourcePath + "resources.cfg");
    
    // Go through all sections & settings in the file
    ConfigFile::SectionIterator seci = cf.getSectionIterator();
    
    String secName, typeName, archName;
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        ConfigFile::SettingsMultiMap *settings = seci.getNext();
        ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
            // OS X does not set the working directory relative to the app,
            // In order to make things portable on OS X we need to provide
            // the loading with it's own bundle path location
            ResourceGroupManager::getSingleton().addResourceLocation(String(macBundlePath() + "/" + archName), typeName, secName);
#else
            ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
#endif
        }
    }
}



/// Optional override method where you can perform resource group loading
/// Must at least do ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
void VSCOgreApplication::loadResources(void)
{
    // Initialise, parse scripts etc
    ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
    
}




