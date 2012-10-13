//
//  VSCOBOSXSceneWindow.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 7/12/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#include "VSCOBApplication.h"

@class VSCOBOSXSceneView;

namespace VSC {
    namespace OB {
        class Application;
    }
}


@interface VSCOBOSXSceneWindowController : NSWindowController

@property (nonatomic, strong) IBOutletCollection(VSCOBOSXSceneView) NSArray* ogreViews;

@property (nonatomic, strong) IBOutlet NSPopUpButton* scenePopUpButton;

@property (nonatomic, assign) VSC::OB::Application::WPtr ogreBulletApplication;

- (IBAction)test1:(id)sender;
- (IBAction)test2:(id)sender;
- (IBAction)test3:(id)sender;

- (IBAction)scenePopUpButtonChanged:(id)sender;

@end
