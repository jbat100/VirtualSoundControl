//
//  VSCSceneElementInspectorWindowController.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 11/10/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCOSXOBSceneElementInspectorWindowController.h"

@interface VSCOSXOBSceneElementInspectorWindowController ()

@end

@implementation VSCOSXOBSceneElementInspectorWindowController

@synthesize elementInspectorView = _elementInspectorView;
@synthesize collisionEventChainController = _collisionEventChainController;

- (id)initWithWindow:(NSWindow *)window
{
    self = [super initWithWindow:window];
    if (self) {
        // Initialization code here.
    }
    
    return self;
}

- (void)windowDidLoad
{
    [super windowDidLoad];
    
    // Implement this method to handle any initialization after your window controller's window has been loaded from its nib file.
}

@end
