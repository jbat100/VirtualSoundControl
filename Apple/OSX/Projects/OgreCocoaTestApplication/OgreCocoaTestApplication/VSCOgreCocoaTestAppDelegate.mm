//
//  VSCAppDelegate.m
//  OgreCocoaTestApplication
//
//  Created by Jonathan Thorpe on 4/30/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCOgreCocoaTestAppDelegate.h"

#import <OSX/macUtils.h>

@implementation VSCOgreCocoaTestAppDelegate

@synthesize window = _window;
@synthesize ogreView = _ogreView;

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    
    NSLog(@"Finished launching, %@ %@", self.window, self.ogreView);
    
    // get platform-specific working directory
    Ogre::String workDir = Ogre::StringUtil::BLANK;
    Ogre::String pluginsPath = Ogre::StringUtil::BLANK;
    workDir = Ogre::macBundlePath() + "/Contents/Resources/";
    pluginsPath = workDir;
    
    // get the ogre root
    Ogre::Root *mRoot = OGRE_NEW Ogre::Root(pluginsPath + "plugins.cfg", workDir + "ogre.cfg", workDir + "ogre.log");
    
    // set up the render system. Since this is running on Mac, our only option is OpenGL.
    mRoot->setRenderSystem(mRoot->getRenderSystemByName("OpenGL Rendering Subsystem"));
    
    // Initialise without an automatically created window
    mRoot->initialise(false);
    
    // Ask for a new renderwindow passing in the ogreView in our nib file
    Ogre::NameValuePairList misc;
    // Pass the handle to the ogreView in our nib
    misc["externalWindowHandle"] = Ogre::StringConverter::toString((size_t)self.ogreView);
    // Tell OGRE that we're using cocoa, so it doesn't need to make a window for us
    misc["macAPI"] = "cocoa";
    
    // Actually create the render window
    NSRect frame = [self.ogreView frame];
    mRoot->createRenderWindow("ogre window", frame.size.width, frame.size.height, false, &misc);
    // And then get a pointer to it.
    Ogre::RenderWindow *mWindow = [self.ogreView ogreWindow];
    
    // Create the scene manager
    Ogre::SceneManager *mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC, "my scene manager");
    
    // Setup the resource locations, and load resources
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation(workDir, std::string("FileSystem"),
                                                                   Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, false);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
    
    
    // Create the camera, node & attach camera
    Ogre::Camera *mCamera = mSceneMgr->createCamera("PlayerCam");
    Ogre::SceneNode* camNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    camNode->attachObject(mCamera);
    mWindow->addViewport(mCamera);
    
    // Create a light
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0, 0, 0));
    mSceneMgr->createLight("MainLight");
    
    // Add a object, give it it's own node
    objectNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    Ogre::Entity *head = mSceneMgr->createEntity("head", "ogrehead.mesh");
    
    objectNode->attachObject(head);
    objectNode->setPosition(Ogre::Vector3(0, 0, -500));
    
    // create a timer that causes OGRE to render at 50fps
    [NSTimer scheduledTimerWithTimeInterval:0.02 target:self selector:@selector(renderFrame) userInfo:NULL repeats:YES];
}

- (void)renderFrame
{
    // Every time the timer fires, render a frame, and rotate the node
    Ogre::Root::getSingleton().renderOneFrame();
    objectNode->rotate(Ogre::Vector3(0, 1, 0), Ogre::Radian(0.01));
}

@end
