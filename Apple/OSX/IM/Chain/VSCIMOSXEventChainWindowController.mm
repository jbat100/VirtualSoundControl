//
//  VSCIMOSXEventChainWindowController.m
//  VSCApplication
//
//  Created by Jonathan Thorpe on 2/3/13.
//  Copyright (c) 2013 JBAT. All rights reserved.
//

#import "VSCIMOSXEventChainWindowController.h"

#import "VSCIMOSXEventChainViewController.h"

@interface VSCIMOSXEventChainWindowController ()

@end

@implementation VSCIMOSXEventChainWindowController

static const bool traceInterface = true;

- (id)initWithWindow:(NSWindow *)window
{
    self = [super initWithWindow:window];
    if (self) {
        // Initialization code here.
    }
    
    return self;
}

- (id)initWithWindowNibName:(NSString *)windowNibName
{
    self = [super initWithWindowNibName:windowNibName];
    if (self)
    {
        [self customInit];
    }
    return self;
}

-(void) customInit
{
    static BOOL calledOnce = NO;
    
    BOOST_ASSERT(calledOnce == NO);
    
    if (traceInterface) NSLog(@"%@ customInit", self);
    
    self.eventChainViewController = [[VSCIMOSXEventChainViewController alloc]
                                           initWithNibName:@"VSCIMOSXEventChainViewController"
                                           bundle:nil];
    
    BOOST_ASSERT(self.eventChainViewController);
    
    calledOnce = YES;
}

- (void)windowDidLoad
{
    [super windowDidLoad];
    
    if (traceInterface) NSLog(@"%@ windowDidLoad", self);
    
    NSView* contentView = [self.window contentView];
    
    BOOST_ASSERT(self.eventChainViewController);
    BOOST_ASSERT([contentView isKindOfClass:[NSView class]]);
    
    NSView* eventChainView = [self.eventChainViewController view];
    eventChainView.translatesAutoresizingMaskIntoConstraints = NO;
    
    [contentView addSubview:eventChainView];
    
    NSDictionary *viewsDictionary = NSDictionaryOfVariableBindings(eventChainView);
    [contentView addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"H:|[eventChainView]|"
                                                                        options:0
                                                                        metrics:nil
                                                                          views:viewsDictionary]];
    [contentView addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"V:|[eventChainView]|"
                                                                        options:0
                                                                        metrics:nil
                                                                          views:viewsDictionary]];
    self.window.delegate = self;
    
    BOOST_ASSERT(self.eventChainViewController.eventTableView);
    
    [self.eventChainViewController.eventTableView reloadData];
}

@end
