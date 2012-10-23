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

#include "VSCGlobalApplication.h"

#include "VSCOBScene.h"
#include "VSCOBApplication.h"
#include "VSCOBResourceManager.h"

#include "VSCMIDI.h"
#include "VSCMIDIOutput.h"
#include "VSCMIDIOutputManager.h"
#include "VSCException.h"

#include <Ogre/OSX/macUtils.h>

#include <boost/assert.hpp>
#include <boost/foreach.hpp>

//#import "OgreOSXCocoaView.h"

@interface VSCAppDelegate ()


@end

@implementation VSCAppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    VSC::GlobalApplication::SPtr globalApplication = VSC::GlobalApplication::singletonGlobalApplication();
    
    std::string resourcePath = Ogre::macBundlePath() + "/Contents/Resources/";
    
    VSC::OB::ResourceManager::SPtr resourceManager = VSC::OB::ResourceManager::SPtr(new VSC::OB::ResourceManager(resourcePath));
    VSC::OB::Application::SPtr obApplication = VSC::OB::Application::singletonApplication();
    obApplication->init(resourceManager);
    
    /*
     *  Create application manager, and setup midi output manager
     */
    
    self.applicationManager = [[VSCOSXApplicationManager alloc] init];
    BOOST_ASSERT(self.applicationManager);

    self.environmentWindowController = [[VSCOSXEnvironmentWindowController alloc] initWithWindowNibName:@"VSCOBOSXSceneWindow"];
    BOOST_ASSERT(self.environmentWindowController);
    
    self.midiWindowController = [[VSCOSXMIDIWindowController alloc] initWithWindowNibName:@"VSCOSXMIDIWindow"];
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
    VSC::OB::Scene::SPtr scene = obApplication->createScene<VSC::OB::Scene>();
    environment->setOBScene(scene);
    
    self.environmentWindowController.environment = environment;
    
    [self showEnvironmentWindow:self];
    
    [self.applicationManager startOgreRendering];
}

- (void)applicationWillTerminate:(NSNotification *)aNotification 
{
    [self.applicationManager stopOgreRendering];
    
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
