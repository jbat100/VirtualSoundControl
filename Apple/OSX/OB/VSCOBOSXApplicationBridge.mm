

#import "VSCOBOSXApplication.h"

#import <Cocoa/Cocoa.h>

#include <Ogre/Ogre.h>
#include <Ogre/OgreConfigFile.h>
#include <Ogre/OSX/macUtils.h>


#include "VSCOBApplicationBase.h"
#include "VSCException.h"
#include "VSCOBInputListener.h"
#include "VSCOBOSXInputAdapter.h"

#include <boost/assert.hpp>

#include "OgreOSXCocoaWindow.h"

#import "VSCOBOSXSceneView.h"







bool VSC::OB::OSXApplicationSetup::setupApplicationWithOgreView(ApplicationBase::SPtr obApplication, void* rawOgreView)
{
    VSCOBOSXSceneView* ogreView = (__bridge VSCOBOSXSceneView*)rawOgreView;
    
    BOOST_ASSERT_MSG( [ogreView isKindOfClass:[VSCOBOSXSceneView class]], "Expected ogreView to be of class OgreView" ); 
    
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
    obApplication->mRoot = OGRE_NEW Ogre::Root(pluginsPath + "plugins.cfg", workDir + "ogre.cfg", workDir + "ogre.log");
    
    // set up the render system. Since this is running on Mac, our only option is OpenGL.
    obApplication->mRoot->setRenderSystem(obApplication->mRoot->getRenderSystemByName("OpenGL Rendering Subsystem"));
    
    // Initialise without an automatically created window
    obApplication->mRoot->initialise(false);
    
    // Ask for a new renderwindow passing in the ogreView in our nib file
    Ogre::NameValuePairList misc;
    // Pass the handle to the ogreView in our nib
    misc["externalWindowHandle"] = Ogre::StringConverter::toString((size_t)ogreView);
    // Tell OGRE that we're using cocoa, so it doesn't need to make a window for us
    misc["macAPI"] = "cocoa";
    
    // Actually create the render window
    NSRect frame = [ogreView frame];
    obApplication->mRoot->createRenderWindow("ogre window", frame.size.width, frame.size.height, false, &misc);
    
    obApplication->mWindow = [(__bridge OgreView*)rawOgreView ogreWindow];
    // This cast works so we do actually have a Ogre::OSXCocoaWindow here
    std::cout << "mWindow is " << (void*)obApplication->mWindow;
    
    OSXInputAdapter::SPtr adapter(new OSXInputAdapter());
    
    BOOST_ASSERT_MSG(adapter, "Expected adapter");
    
    adapter->allowRapidFireKeyPresses(false);
    
    adapter->addInputListener(obApplication);
    
    //BOOST_ASSERT_MSG(obApplication->getInputAdapter() == adapter, "Expected adapter");
    
    adapter->setCocoaView(ogreView);
    
    ogreView.inputAdapter = adapter;
    
    return true;
}


