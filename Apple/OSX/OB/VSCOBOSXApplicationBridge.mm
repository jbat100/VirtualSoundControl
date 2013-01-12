

#import "VSCOBOSXApplicationBridge.h"
#import "VSCOBOSXSceneDisplayView.h"
#import <Cocoa/Cocoa.h>

#include "VSCException.h"

#include <Ogre/Ogre.h>
#include <Ogre/OSX/macUtils.h>

#include <boost/assert.hpp>

/*

VSC::OB::OSXApplicationBridge::OSXApplicationBridge()
{
    this->setResourcePath( Ogre::macBundlePath() + "/Contents/Resources/");
}
 */


Ogre::Root* VSC::OB::OSXApplicationBridge::createOgreRoot()
{
    
    // get platform-specific working directory
    Ogre::String workDir = Ogre::StringUtil::BLANK;
    Ogre::String pluginsPath = Ogre::StringUtil::BLANK;
    workDir = Ogre::macBundlePath() + "/Contents/Resources/";
    pluginsPath = workDir;
    
    Ogre::Root* root = OGRE_NEW Ogre::Root(pluginsPath + "plugins.cfg", workDir + "ogre.cfg", workDir + "ogre.log");
    
    // set up the render system. Since this is running on Mac, our only option is OpenGL.
    root->setRenderSystem(root->getRenderSystemByName("OpenGL Rendering Subsystem"));
    
    // Initialise without an automatically created window
    root->initialise(false);
    
    return root;
}


void VSC::OB::OSXApplicationBridge::setupDisplayWithView(Display::SPtr display, void* view) {
    
    if (!view) throw VSCInvalidArgumentException("Expected non nil view");
    VSCOBOSXSceneDisplayView* sceneView = (__bridge VSCOBOSXSceneDisplayView*)view;
    if ([sceneView isKindOfClass:[VSCOBOSXSceneDisplayView class]] == false) throw VSCInvalidArgumentException("Expected non nil view");
    
    Application::SPtr application = this->getApplication();
    BOOST_ASSERT(application);
    if(!application) return;
    
    Ogre::Root* root = application->getOgreRoot();
    BOOST_ASSERT(root);
    if(!root) return;
    
    // Ask for a new renderwindow passing in the ogreView in our nib file
    Ogre::NameValuePairList misc;
    // Pass the handle to the ogreView in our nib
    misc["externalWindowHandle"] = Ogre::StringConverter::toString((size_t)sceneView);
    // Tell OGRE that we're using cocoa, so it doesn't need to make a window for us
    misc["macAPI"] = "cocoa";
    
    // Actually create the render window
    NSRect frame = [sceneView frame];
    
    root->createRenderWindow("ogre window", frame.size.width, frame.size.height, false, &misc);
    
    Ogre::RenderWindow* window = [sceneView ogreWindow];
    // This cast works so we do actually have a Ogre::OSXCocoaWindow here
    std::cout << "mWindow is " << (void*)window;
    
    this->setDisplayRenderWindow(display, window);
    
}

