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

#import "VSCOgreApplicationCocoaSetup.h"

#import <Cocoa/Cocoa.h>

#include <Ogre/Ogre.h>
#include <Ogre/OgreConfigFile.h>
#include <Ogre/OSX/macUtils.h>

//#include "OgreOSXCocoaView.h"
#include "VSCOgreView.h"
#include "OgreOSXCocoaWindow.h"

#include "VSCOgreApplication.h"
#include "VSCException.h"
#include "VSCOgreInputListener.h"
#include "VSCOgreBulletCocoaInputAdapter.h"

#include <boost/assert.hpp>


bool VSCOgreApplicationCocoaSetup::setupApplicationWithOgreView(VSCOgreApplication* ogreApplication, void* rawOgreView)
{
    VSCOgreView* ogreView = (__bridge VSCOgreView*)rawOgreView;
    BOOST_ASSERT_MSG( [ogreView isKindOfClass:[VSCOgreView class]], "Expected ogreView to be of class OgreView" ); 
    
    // get platform-specific working directory
    Ogre::String workDir = Ogre::StringUtil::BLANK;
    Ogre::String pluginsPath = Ogre::StringUtil::BLANK;
    workDir = Ogre::macBundlePath() + "/Contents/Resources/";
    pluginsPath = workDir;
    
    /*
     *  IMPORTANT: http://www.ogre3d.org/docs/api/html/classOgre_1_1Root.html#_details
     *
     *  The Ogre::Root class represents a starting point for the client application. From here, the application can gain access to 
     *  the fundamentals of the system, namely the rendering systems available, management of saved configurations, logging, and access 
     *  to other classes in the system. Acts as a hub from which all other objects may be reached. An instance of Root must be created
     *  before any other Ogre operations are called. Once an instance has been created, the same instance is accessible throughout the 
     *  life of that object by using Root::getSingleton (as a reference) or Root::getSingletonPtr (as a pointer). 
     */
    
    // get the ogre root
    ogreApplication->mRoot = OGRE_NEW Ogre::Root(pluginsPath + "plugins.cfg", workDir + "ogre.cfg", workDir + "ogre.log");
    
    // set up the render system. Since this is running on Mac, our only option is OpenGL.
    ogreApplication->mRoot->setRenderSystem(ogreApplication->mRoot->getRenderSystemByName("OpenGL Rendering Subsystem"));
    
    // Initialise without an automatically created window
    ogreApplication->mRoot->initialise(false);
    
    // Ask for a new renderwindow passing in the ogreView in our nib file
    Ogre::NameValuePairList misc;
    // Pass the handle to the ogreView in our nib
    misc["externalWindowHandle"] = Ogre::StringConverter::toString((size_t)ogreView);
    // Tell OGRE that we're using cocoa, so it doesn't need to make a window for us
    misc["macAPI"] = "cocoa";
    
    // Actually create the render window
    NSRect frame = [ogreView frame];
    ogreApplication->mRoot->createRenderWindow("ogre window", frame.size.width, frame.size.height, false, &misc);
    
    ogreApplication->mWindow = VSCOgreApplicationCocoaSetup::getRenderWindow(rawOgreView);
    // This cast works so we do actually have a Ogre::OSXCocoaWindow here
    std::cout << "mWindow is " << (void*)ogreApplication->mWindow;
    
    VSCOgreBulletCocoaInputAdapter* adapter = VSCOgreApplicationCocoaSetup::createCocoaInputAdapter();
    ogreApplication->setInputAdapter(adapter);
    adapter->setCocoaView(ogreView);
    adapter->addInputListener(ogreApplication);
    ogreView.inputAdapter = adapter;
    
    return true;
}

Ogre::RenderWindow* VSCOgreApplicationCocoaSetup::getRenderWindow(void* rawOgreView) 
{
    OgreView* ogreView = (__bridge OgreView*)rawOgreView;
    BOOST_ASSERT_MSG( [ogreView isKindOfClass:[OgreView class]], "Expected ogreView to be of class OgreView" ); 
    //Ogre::RenderWindow *mWindow = [ogreView ogreWindow];
    return [ogreView ogreWindow];
}

VSCOgreBulletCocoaInputAdapter* VSCOgreApplicationCocoaSetup::createCocoaInputAdapter(void)
{
    return new VSCOgreBulletCocoaInputAdapter();
}
