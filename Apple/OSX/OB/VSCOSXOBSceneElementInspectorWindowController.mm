//
//  VSCSceneElementInspectorWindowController.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 11/10/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCOSXOBSceneElementInspectorWindowController.h"
#import "VSCOSXOBSceneElementInspectorViewController.h"

@interface VSCOSXOBSceneElementInspectorWindowController ()

-(void) customInit;

@end

@implementation VSCOSXOBSceneElementInspectorWindowController

- (id)initWithWindowNibName:(NSString *)windowNibName
{
    self = [super initWithWindowNibName:windowNibName];
    if (self)
    {
        [self customInit];
    }
    return self;
}

// according to the documentation, is the designated initializer so should always be called?
- (id)initWithWindow:(NSWindow *)window
{
    self = [super initWithWindow:window];
    if (self)
    {
        [self customInit];
    }
    return self;
}

-(void) customInit
{
    self.elementInspectorViewController = [[VSCOSXOBSceneElementInspectorViewController alloc]
                                           initWithNibName:@"VSCOSXOBSceneElementInspectorViewController"
                                           bundle:nil];
    
    BOOST_ASSERT(self.elementInspectorViewController);
}

- (void)windowDidLoad
{
    [super windowDidLoad];
    
    // Implement this method to handle any initialization after your window controller's window has been loaded from its nib file.
    
    BOOST_ASSERT(self.elementInspectorViewController);
}





@end
