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

@property (nonatomic, assign) BOOL traceInterface;

-(void) customInit;

@end

@implementation VSCOSXOBSceneElementInspectorWindowController

const static BOOL traceInterface = YES;

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
    
    if (traceInterface) NSLog(@"%@ customInit", self);
    
    self.elementInspectorViewController = [[VSCOSXOBSceneElementInspectorViewController alloc]
                                           initWithNibName:@"VSCOSXOBSceneElementInspectorViewController"
                                           bundle:nil];
    
    BOOST_ASSERT(self.elementInspectorViewController);
}

- (void)windowDidLoad
{
    [super windowDidLoad];
    
    if (traceInterface) NSLog(@"%@ windowDidLoad", self);
    
    BOOST_ASSERT(self.elementInspectorViewController);
    BOOST_ASSERT(self.mainBox);
    
    if (self.elementInspectorViewController && self.mainBox)
    {
        [self.mainBox setContentView:[self.elementInspectorViewController view]];
    }
}





@end
