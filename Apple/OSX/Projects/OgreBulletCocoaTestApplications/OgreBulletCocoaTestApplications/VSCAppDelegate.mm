//
//  VSCAppDelegate.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 5/1/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCAppDelegate.h"
#import "VSCOSXApplicationManager.h"
#import "VSCOSXEnvironmentWindowController.h"
#import "VSCOSXMIDIWindowController.h"
#import "VSCOBOSXSceneDisplayView.h"

#include "VSCGlobalApplication.h"
#include "VSCEnvironment.h"
#include "VSCOBScene.h"
#include "VSCOBApplication.h"
#include "VSCOBResourceManager.h"
#include "VSCIMCollisionMapper.h"
#include "VSCMIDI.h"
#include "VSCMIDIOutput.h"
#include "VSCMIDIOutputManager.h"
#include "VSCException.h"

#include "VSCEnvironmentTest.h"

#include <Ogre/OSX/macUtils.h>

#include <boost/assert.hpp>
#include <boost/foreach.hpp>

//#define VSCOSX_TEST_ELEMENT_INSPECTOR_WINDOW // TEST



#ifdef VSCOSX_TEST_ELEMENT_INSPECTOR_WINDOW
#import "VSCOSXOBSceneElementInspectorWindowController.h"
#endif

@interface VSCAppDelegate ()

#ifdef VSCOSX_TEST_ELEMENT_INSPECTOR_WINDOW
@property (nonatomic, strong) VSCOSXOBSceneElementInspectorWindowController* testElementInspectorWindowController;
#endif


@end

@implementation VSCAppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    
#ifdef VSCOSX_TEST_ELEMENT_INSPECTOR_WINDOW
    
    self.testElementInspectorWindowController = [[VSCOSXOBSceneElementInspectorWindowController alloc]
                                                 initWithWindowNibName:@"VSCOSXOBSceneElementInspectorWindowController"];
    
    BOOST_ASSERT(self.testElementInspectorWindowController);
    
    [self.testElementInspectorWindowController showWindow:self];
    
#else
    
    VSC::GlobalApplication::SPtr globalApplication = VSC::GlobalApplication::singletonGlobalApplication();
    
    std::string resourcePath = Ogre::macBundlePath() + "/Contents/Resources/resources.cfg";
    
    VSC::OB::ResourceManager::SPtr resourceManager = VSC::OB::ResourceManager::SPtr(new VSC::OB::ResourceManager(resourcePath));
    VSC::OB::Application::SPtr obApplication = VSC::OB::Application::singletonApplication();
    obApplication->init(resourceManager);
    
    /*
     *  Create application manager, and setup midi output manager
     */
    
    self.applicationManager = [[VSCOSXApplicationManager alloc] init];
    BOOST_ASSERT(self.applicationManager);

    self.environmentWindowController = [[VSCOSXEnvironmentWindowController alloc]
                                        initWithWindowNibName:@"VSCOSXEnvironmentWindowController"];
    BOOST_ASSERT(self.environmentWindowController);
    
    // create window to force a display + render windoe creation
    //[self.environmentWindowController loadWindow]; // docs says not to invoke this method
    NSWindow* w = self.environmentWindowController.window; // does not force creation
    BOOST_ASSERT(w);
    
    self.midiWindowController = [[VSCOSXMIDIWindowController alloc] initWithWindowNibName:@"VSCOSXMIDIWindowController"];
    BOOST_ASSERT(self.midiWindowController);
    
    /*
     *  Refresh outputs and open them all
     */
    
    self.midiWindowController.midiOutputManager = VSC::MIDI::OutputManager::singletonManager();
    
    const VSC::MIDI::Outputs& outputs = VSC::MIDI::OutputManager::singletonManager()->getOutputs();
    BOOST_FOREACH(VSC::MIDI::Output::SPtr output, outputs)
    {
        try {
            output->open();
        } catch (VSCMIDIException& e) {
            std::cerr << e.what() << " - Additional info: " << e.getValueForKey(VSCBaseExceptionAdditionalInfoKey) << std::endl;
        }
    }
    
    [self showMIDIWindow:nil];
    
    VSC::Environment::SPtr environment = globalApplication->createEnvironment<VSC::Environment>();
    BOOST_ASSERT(environment);
    
    VSC::OB::Scene::SPtr scene = obApplication->createScene<VSC::OB::Scene>();
    BOOST_ASSERT(scene);
    environment->setOBScene(scene);
    
    VSC::IM::CollisionMapper::SPtr collisionMapper = VSC::IM::CollisionMapper::SPtr(new VSC::IM::CollisionMapper);
    environment->setIMCollisionMapper(collisionMapper);
    
    self.environmentWindowController.environment = environment;
    
    [self showEnvironmentWindow:self];
    
    [self.applicationManager startOgreRendering];
    
    /*
     *  Setup test
     */
    
    VSC::EnvironmentTest::SPtr test = VSC::EnvironmentTest::SPtr(new VSC::EnvironmentTest);
    test->setupTestForEnvironment(environment);
    
    const VSC::OB::Scene::Elements& elements = environment->getOBScene()->getElements();
    VSC::OB::Scene::Element::SPtr element = VSC::OB::Scene::Element::SPtr();
    BOOST_FOREACH(VSC::OB::Scene::Element::SPtr e, elements)
    {
        VSC::IM::CollisionEventChain::SPtr chain = environment->getIMCollisionMapper()->getEventChainForCollisionStarted(e);
        if (chain->getNumberOfEvents() > 0)
        {
            element = e;
            break;
        }
    }
    
    BOOST_ASSERT(element);
    if (element) [self.environmentWindowController showElementInspectorForElement:element];
    
    
#endif
}

- (void)applicationWillTerminate:(NSNotification *)aNotification
{
    [self.applicationManager stopOgreRendering];
    
    VSC::OB::Application::singletonApplication()->shutdown();
    
}

-(IBAction)showMIDIWindow:(id)sender
{
    [self.midiWindowController showWindow:sender];
}

-(IBAction)showEnvironmentWindow:(id)sender
{
    [self.environmentWindowController showWindow:sender];
}

@end
