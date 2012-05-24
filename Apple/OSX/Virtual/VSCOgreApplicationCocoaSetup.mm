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

#import <Ogre/Ogre.h>
#import <Ogre/OgreConfigFile.h>
#import <Ogre/OSX/macUtils.h>

#import "OgreOSXCocoaView.h"
#import "OgreOSXCocoaWindow.h"

#import "VSCOgreApplication.h"
#import "VSCOgreFrameListener.h"

#import "VSCException.h"

using namespace Ogre;

VSCOgreApplicationCocoaSetup::VSCOgreApplicationCocoaSetup(void* view)
{
    //OgreView* ogreView = (OgreView*)ogreView;
    this->setOgreView(view);
}

VSCOgreApplicationCocoaSetup::~VSCOgreApplicationCocoaSetup()
{
    
}


bool VSCOgreApplicationCocoaSetup::setup(VSCOgreApplication* ogreApplication)
{
    OgreView* ogreView = (__bridge OgreView*)mOgreView;
    
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
    
    // And then get a pointer to it.
    Ogre::RenderWindow *mWindow = [ogreView ogreWindow];
    
    // This cast works so we do actually have a Ogre::OSXCocoaWindow here
    //std::cout << "mWindow is " << (void*)mWindow;
    //Ogre::OSXCocoaWindow* cocoaWindow = dynamic_cast<Ogre::OSXCocoaWindow*> (mWindow);
    //std::cout << "cocoaWindow is " << (void*)cocoaWindow;
    
    return true;
}


void VSCOgreApplicationCocoaSetup::setOgreView(void* ogreView)
{
    if ([(__bridge OgreView*)ogreView isKindOfClass:[OgreView class]]) 
    {
        mOgreView = ogreView;
    }
    else if (ogreView != NULL) // only throw an exception if the view is not NULL
    {
        throw VSCInvalidArgumentException("Expected argument view to b objective-c++ kind of class OgreView");
    }
}

void* VSCOgreApplicationCocoaSetup::getOgreView(void)
{
    return mOgreView;
}

Ogre::RenderWindow* VSCOgreApplicationCocoaSetup::getRenderWindow(void) 
{

    //OgreView* ogreView = (__bridge OgreView*)mOgreView;
    return [(__bridge OgreView*)mOgreView ogreWindow];
    
}

