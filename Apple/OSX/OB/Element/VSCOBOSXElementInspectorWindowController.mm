//
//  VSCElementInspectorWindowController.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 11/10/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCOBOSXElementInspectorWindowController.h"
#import "VSCOBOSXElementInspectorViewController.h"
#import "VSCOBOSXElementDetailView.h"
#import "VSCOBOSXElementEventChainsView.h"

#import "DMTabBar.h"

@interface VSCOBOSXElementInspectorWindowController ()

@property (nonatomic, assign) BOOL calledCustomInit;

-(void) customInit;

- (void)printUIDescription;

@end

@implementation VSCOBOSXElementInspectorWindowController

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
        //[self customInit];
    }
    return self;
}

-(void) customInit
{
    static BOOL calledOnce = NO;
    
    BOOST_ASSERT(calledOnce == NO);
    
    if (traceInterface) NSLog(@"%@ customInit", self);
    
    self.elementInspectorViewController = [[VSCOBOSXElementInspectorViewController alloc]
                                           initWithNibName:@"VSCOBOSXElementInspectorViewController"
                                           bundle:nil];
    
    BOOST_ASSERT(self.elementInspectorViewController);
    
    calledOnce = YES;
}

- (void)printUIDescription
{
    NSLog(@"------------------%@ printUIDescription----------------------", self);
    NSView* inspectorView = self.elementInspectorViewController.view;
    NSView* tabBar = self.elementInspectorViewController.tabBar;
    NSView* windowContentView = [[self window] contentView];
    NSLog(@"windowContentView %@, %@", windowContentView, NSStringFromRect(windowContentView.frame));
    NSLog(@"inspectorView %@, %@", inspectorView, NSStringFromRect(inspectorView.frame));
    NSLog(@"tabBar %@, %@", tabBar, NSStringFromRect(tabBar.frame));
    NSLog(@"-------------------------------------------------------------");
}

- (void)windowDidLoad
{
    [super windowDidLoad];
    
    if (traceInterface) NSLog(@"%@ windowDidLoad", self);
    
    NSView* contentView = [self.window contentView];
    
    BOOST_ASSERT(self.elementInspectorViewController);
    BOOST_ASSERT([contentView isKindOfClass:[NSView class]]);
    
    NSView* elementInspectorView = [self.elementInspectorViewController view];
    elementInspectorView.translatesAutoresizingMaskIntoConstraints = NO;
    
    [contentView addSubview:elementInspectorView];
    
    NSDictionary *viewsDictionary = NSDictionaryOfVariableBindings(elementInspectorView);
    [contentView addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"H:|[elementInspectorView]|"
                                                                           options:0
                                                                           metrics:nil
                                                                             views:viewsDictionary]];
    [contentView addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"V:|[elementInspectorView]|"
                                                                           options:0
                                                                           metrics:nil
                                                                             views:viewsDictionary]];
    
    [self.elementInspectorViewController setupIfNecessary];

    self.window.delegate = self;
}

#pragma mark - NSWindowDelegate

- (void)windowDidResize:(NSNotification *)notification
{
    if (traceInterface)
    {
        NSLog(@"%@ windowDidResize: %@", self, notification);
        //[self printUIDescription];
        //[self.elementInspectorViewController.elementDetailView printUIDescription];
    }
}


@end
