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

@property (nonatomic, assign) BOOL calledCustomInit;

-(void) customInit;

- (void)printUIDescription;

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
        //[self customInit];
    }
    return self;
}

-(void) customInit
{
    
    static BOOL calledOnce = NO;
    
    BOOST_ASSERT(calledOnce == NO);
    
    if (traceInterface) NSLog(@"%@ customInit", self);
    
    self.elementInspectorViewController = [[VSCOSXOBSceneElementInspectorViewController alloc]
                                           initWithNibName:@"VSCOSXOBSceneElementInspectorViewController"
                                           bundle:nil];
    
    BOOST_ASSERT(self.elementInspectorViewController);
    
    calledOnce = YES;
}

- (void)printUIDescription
{
    NSLog(@"------------------%@ printUIDescription----------------------", self);
    NSLog(@"self.mainBox : %@, %@", self.mainBox, NSStringFromRect(self.mainBox.frame));
    NSLog(@"self.mainBox.contentView : %@, %@", self.mainBox.contentView, NSStringFromRect([(NSView*)[self.mainBox contentView] frame]));
    NSLog(@"self.elementInspectorViewController.view %@, %@",
          self.elementInspectorViewController.view, NSStringFromRect(self.elementInspectorViewController.view.frame));
    NSLog(@"-------------------------------------------------------------");
}

- (void)windowDidLoad
{
    [super windowDidLoad];
    
    if (traceInterface) NSLog(@"%@ windowDidLoad", self);
    
    BOOST_ASSERT(self.elementInspectorViewController);
    BOOST_ASSERT(self.mainBox);
    
    if (self.elementInspectorViewController && self.mainBox)
    {
        BOOST_ASSERT([self.mainBox contentView]);
        //[self.elementInspectorViewController view].frame = self.mainBox.bounds;
        [[self.mainBox contentView] addSubview:[self.elementInspectorViewController view]];
    }
    
    //if (traceInterface) [self printUIDescription];
    
    NSView* boxContentView = [self.mainBox contentView];
    //boxContentView.translatesAutoresizingMaskIntoConstraints = NO;
    NSView* elementInspectorView = [self.elementInspectorViewController view];
    
    NSDictionary *viewsDictionary = NSDictionaryOfVariableBindings(boxContentView, elementInspectorView);
    [boxContentView addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"H:|[elementInspectorView]|"
                                                                           options:0
                                                                           metrics:nil
                                                                             views:viewsDictionary]];
    [boxContentView addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"V:|[elementInspectorView]|"
                                                                           options:0
                                                                           metrics:nil
                                                                             views:viewsDictionary]];

    self.window.delegate = self;
}

#pragma mark - NSWindowDelegate

- (void)windowDidResize:(NSNotification *)notification
{
    if (traceInterface)
    {
        //NSLog(@"%@ windowDidResize: %@", self, notification);
        //[self printUIDescription];
    }
}


@end
