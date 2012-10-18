

#import "VSCOBOSXSceneBridge.h"
#import "OgreOSXCocoaWindow.h"
#import "VSCOBOSXSceneView.h"

#include "VSCException.h"
#include "VSCApplication.h"

#import <Cocoa/Cocoa.h>

#include <Ogre/Ogre.h>

#include <boost/assert.hpp>


VSC::OB::Display::SPtr VSC::OB::OSXSceneBridge::createDisplayWithView(void* view) {
    
    if (!view) throw VSCInvalidArgumentException("Expected non nil view");
    VSCOBOSXSceneView* sceneView = (__bridge VSCOBOSXSceneView*)view;
    if ([sceneView isKindOfClass:[VSCOBOSXSceneView class]] == false) throw VSCInvalidArgumentException("Expected non nil view");
    
    Scene::SPtr scene = this->getScene();
    BOOST_ASSERT(scene);
    if(!scene) return Display::SPtr();
    
    Application::SPtr application = scene->getApplication();
    BOOST_ASSERT(application);
    if(!application) return Display::SPtr();
    
    Ogre::Root* root = application->getOgreRoot();
    BOOST_ASSERT(root);
    if(!root) return Display::SPtr();
    
    Display::SPtr display = Display::SPtr(new Display);
 
    // Ask for a new renderwindow passing in the ogreView in our nib file
    Ogre::NameValuePairList misc;
    // Pass the handle to the ogreView in our nib
    misc["externalWindowHandle"] = Ogre::StringConverter::toString((size_t)ogreView);
    // Tell OGRE that we're using cocoa, so it doesn't need to make a window for us
    misc["macAPI"] = "cocoa";

    // Actually create the render window
    NSRect frame = [ogreView frame];
    
    root->createRenderWindow("ogre window", frame.size.width, frame.size.height, false, &misc);

    Ogre::RenderWindow* window = [sceneView ogreWindow];
    // This cast works so we do actually have a Ogre::OSXCocoaWindow here
    std::cout << "mWindow is " << (void*)obApplication->mWindow;
    
    display->setRenderWindow(window);
    
}



