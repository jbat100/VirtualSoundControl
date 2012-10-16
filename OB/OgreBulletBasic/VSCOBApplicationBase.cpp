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

#include <Ogre/Ogre.h>
#include <Ogre/OgreConfigFile.h>

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
#include <Ogre/OSX/macUtils.h>
#endif

#include "VSCOBApplicationBase.h"
#include "VSCOBInputAdapter.h"

using namespace Ogre;

VSC::OB::ApplicationBase::ApplicationBase() : 
mRoot(0),
mCamera(0),
mSceneMgr(0),
mWindow(0)
{
    
    mResourcePath = Ogre::macBundlePath() + "/Contents/Resources/";

    
    /*
     *  Create a default keyboard manager and set bindings to default
     */
    
    mKeyboardManager = VSC::OB::KeyboardManager::SPtr(new VSC::OB::KeyboardManager());
    this->setOgreKeyBindings(mKeyboardManager->generateDefaultBindings());
    
}

/// Standard destructor
VSC::OB::ApplicationBase::~ApplicationBase()
{
    if (mRoot)
        OGRE_DELETE mRoot;
}

bool VSC::OB::ApplicationBase::setupWithOgreView(void* ogreView) {
    
    VSC::OB::OSXApplicationSetup::setupApplicationWithOgreView(shared_from_this(), ogreView);
    
    this->setupResources();
    this->chooseSceneManager();
    this->createCamera();
    this->createViewports();
    // Set default mipmap level (NB some APIs ignore this)
    TextureManager::getSingleton().setDefaultNumMipmaps(5);
    this->createResourceListener(); // Create any resource listeners (for loading screens)
    this->loadResources();
    
    return true;
    
}



void VSC::OB::ApplicationBase::createSceneManager(void)
{
    // Create the SceneManager, in this case a generic one
    mSceneMgr = mRoot->createSceneManager(ST_GENERIC, "ExampleSMInstance");
}

void VSC::OB::ApplicationBase::createCamera(void)
{
    // Create the camera
    mCamera = mSceneMgr->createCamera("PlayerCam");
    // Position it at 500 in Z direction
    mCamera->setPosition(Vector3(0,0,500));
    // Look back along -Z
    mCamera->lookAt(Vector3(0,0,-300));
    mCamera->setNearClipDistance(5);
}

void VSC::OB::ApplicationBase::createViewports(void)
{
    // Create one viewport, entire window
    Viewport* vp = mWindow->addViewport(mCamera);
    vp->setBackgroundColour(ColourValue(0,0,0));
    // Alter the camera aspect ratio to match the viewport
    mCamera->setAspectRatio(Real(vp->getActualWidth()) / Real(vp->getActualHeight()));
}

/// Method which will define the source of resources (other than current folder)
void VSC::OB::ApplicationBase::setupResources(void)
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
            try {
                std::cout << "typeName : " << typeName << "; archName : " << archName << "; secName : " << secName << ";" << std::endl; 
                //ResourceGroupManager::getSingleton().addResourceLocation(String(Ogre::macBundlePath() + "/" + archName), typeName, secName);
                //Changed by jbat100
                ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
            } catch (Ogre::Exception& e) {
                std::cout << "Exception : " << e.getFullDescription() << std::endl;
            }
#else
            ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
#endif
        }
    }
}

void VSC::OB::ApplicationBase::createResourceListener(void)
{
    
}


/// Optional override method where you can perform resource group loading
/// Must at least do ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
void VSC::OB::ApplicationBase::loadResources(void)
{
    // Initialise, parse scripts etc
    ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
    
}

bool VSC::OB::ApplicationBase::keyPressed(Ogre::RenderWindow* renderWindow, OIS::KeyCode key)
{
    if (mTraceUI) std::cout << "VSC::OB::ApplicationBase::keyPressed " << key << std::endl;
    
    return VSC::OB::InputListener::keyPressed(renderWindow, key);
}


