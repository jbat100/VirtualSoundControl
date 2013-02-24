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
    
    [self.window setAutorecalculatesKeyViewLoop:YES];
    
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
    
    /*
     *  Set up first responder observer
     */
    
    [[self window] addObserver:self forKeyPath:@"firstResponder"
                       options:(NSKeyValueObservingOptionNew | NSKeyValueObservingOptionOld)
                       context:NULL];
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

- (void)keyDown:(NSEvent *)theEvent
{
    NSLog(@"%@ keyDown %@", self, theEvent);
    [[self window] recalculateKeyViewLoop];
}

/*
- (id)windowWillReturnFieldEditor:(NSWindow *)sender toObject:(id)client
{
    NSLog(@"%@ windowWillReturnFieldEditor: %@ toObject: ", self, sender, );
    return nil;
}
 */

#pragma mark - KVO

- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary *)change context:(void *)context {
    
    if ([keyPath isEqual:@"firstResponder"] && [object isKindOfClass:[NSWindow class]])
    {
        NSLog(@"%@ firstResponder changed to %@", self, [[self window] firstResponder]);
        
        id responder = [[self window] firstResponder];
        
        if ([responder isKindOfClass:[NSTextField class]])
        {
            NSTextField* textField = (NSTextField*)responder;
            NSText* text = [[self window] fieldEditor:YES forObject:(id)responder];
            NSLog(@"selectable %d editable %d enabled %d text %@", textField.isSelectable, textField.isEditable, textField.isEnabled, text);
        }
    }
    
    //[super observeValueForKeyPath:keyPath ofObject:object change:change context:context];
}


@end
