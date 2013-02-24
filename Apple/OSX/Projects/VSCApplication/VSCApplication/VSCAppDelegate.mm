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
#include "VSCOBElement.h"
#include "VSCOBResourceManager.h"
#include "VSCCollisionMapper.h"
#include "VSCMIDI.h"
#include "VSCMIDIOutput.h"
#include "VSCMIDIOutputManager.h"
#include "VSCException.h"
#include "VSCTaskQueue.h"
#include "VSCTaskTest.h"
#include "VSCMIDITasks.h"

#include "VSCEnvironmentTest.h"

#include <Ogre/OSX/macUtils.h>

#include <boost/assert.hpp>
#include <boost/foreach.hpp>


//#define VSCOSX_TEST_ELEMENT_INSPECTOR_WINDOW // TEST
//#define VSCOSX_TEST_TASK_QUEUE // TEST

#define VSCOSX_FULL_APPLICATION // RUN FULL BLOWN APP
#define VSCOSX_SETUP_ENVIRONMENT_TEST

#ifdef VSCOSX_TEST_ELEMENT_INSPECTOR_WINDOW
#import "VSCOBOSXElementInspectorWindowController.h"
#endif

using namespace VSC;
using namespace VSC::IM;

@interface VSCAppDelegate ()

#ifdef VSCOSX_TEST_ELEMENT_INSPECTOR_WINDOW
@property (nonatomic, strong) VSCOBOSXElementInspectorWindowController* testElementInspectorWindowController;
#endif


@end

@implementation VSCAppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    
    [[NSUserDefaults standardUserDefaults] setBool:NO forKey:@"NSConstraintBasedLayoutVisualizeMutuallyExclusiveConstraints"];
    
    /*
     *  Refresh MIDI outputs and open them all
     */
    
    TaskQueue::SPtr midiTaskQueue = MIDI::SingletonMIDITaskQueue();
    BOOST_ASSERT(midiTaskQueue);
    midiTaskQueue->start();
    
    MIDI::OutputManager::SPtr outputManager = MIDI::OutputManager::singletonManager();
    BOOST_ASSERT(outputManager);
    outputManager->refreshOutputs();
    const MIDI::Outputs& outputs = outputManager->getOutputs();
    BOOST_FOREACH(MIDI::Output::SPtr output, outputs)
    {
        try
        {
            output->open();
        }
        catch (VSCMIDIException& e) {
            std::cerr << e.what() << " - Additional info: ";
            std::cerr << e.getValueForKey(VSCBaseExceptionAdditionalInfoKey) << std::endl;
        }
    }
    
    self.midiWindowController = [[VSCOSXMIDIWindowController alloc] initWithWindowNibName:@"VSCOSXMIDIWindowController"];
    BOOST_ASSERT(self.midiWindowController);
    [self showMIDIWindow:nil];
    
#ifdef VSCOSX_TEST_ELEMENT_INSPECTOR_WINDOW
    self.testElementInspectorWindowController = [[VSCOBOSXElementInspectorWindowController alloc]
                                                 initWithWindowNibName:@"VSCOBOSXElementInspectorWindowController"];
    BOOST_ASSERT(self.testElementInspectorWindowController);
    [self.testElementInspectorWindowController showWindow:self];
#endif
    
#ifdef VSCOSX_TEST_TASK_QUEUE
    TaskQueue::SPtr taskQueue = MIDI::SingletonMIDITaskQueue();
    TaskTest::SPtr taskTest = TaskTest::SPtr(new TaskTest);
    taskTest->performTestWithTaskQueue(taskQueue);
#endif
    
#ifdef VSCOSX_FULL_APPLICATION
    
    GlobalApplication::SPtr globalApplication = GlobalApplication::singletonGlobalApplication();
    
    std::string resourcePath = Ogre::macBundlePath() + "/Contents/Resources/resources.cfg";
    
    OB::ResourceManager::SPtr resourceManager = OB::ResourceManager::SPtr(new OB::ResourceManager(resourcePath));
    OB::Application::SPtr obApplication = OB::Application::singletonApplication();
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
    
    
    Environment::SPtr environment = globalApplication->createEnvironment<Environment>();
    BOOST_ASSERT(environment);
    
    OB::Scene::SPtr scene = obApplication->createScene<OB::Scene>();
    BOOST_ASSERT(scene);
    environment->setScene(scene);
    
    CollisionMapper::SPtr collisionMapper = CollisionMapper::SPtr(new CollisionMapper);
    environment->setCollisionMapper(collisionMapper);
    
    self.environmentWindowController.environment = environment;
    
    [self showEnvironmentWindow:self];
    
    [self.applicationManager startOgreRendering];
    
#ifdef VSCOSX_SETUP_ENVIRONMENT_TEST
    
    EnvironmentTest::SPtr test = EnvironmentTest::SPtr(new EnvironmentTest);
    test->setupTestForEnvironment(environment);
    
    const OB::Elements& elements = environment->getScene()->getElements();
    OB::Element::SPtr element = OB::Element::SPtr();
    BOOST_FOREACH(OB::Element::SPtr e, elements)
    {
        EventChains chains = environment->getCollisionMapper()->getEventChainsForCollisionStarted(e);
        if (chains.size() > 0)
        {
            element = e;
            break;
        }
    }
    
    BOOST_ASSERT(element);
    if (element) [self.environmentWindowController showElementInspectorForElement:element];
    
#endif
    

    
#endif
    
}

- (void)applicationWillTerminate:(NSNotification *)aNotification
{
    [self.applicationManager stopOgreRendering];
    
    OB::Application::singletonApplication()->shutdown();
    
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
