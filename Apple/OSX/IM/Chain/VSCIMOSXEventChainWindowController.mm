//
//  VSCIMOSXEventChainWindowController.m
//  VSCApplication
//
//  Created by Jonathan Thorpe on 2/3/13.
//  Copyright (c) 2013 JBAT. All rights reserved.
//

#import "VSCIMOSXEventChainWindowController.h"
#import "VSCIMOSXEventChainViewController.h"
#import "VSCIMOSXEventListView.h"

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
    
    [self.eventChainViewController senderRequestsEventChainView:self];
    
    BOOST_ASSERT(self.eventChainViewController.eventListView.eventTableView);
    
    [self.eventChainViewController.eventListView.eventTableView reloadData];
    
    /*
     *  Set up first responder observer
     */
    
    [[self window] addObserver:self forKeyPath:@"firstResponder"
                       options:(NSKeyValueObservingOptionNew | NSKeyValueObservingOptionOld)
                       context:NULL];
}

- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary *)change context:(void *)context {
    
    if ([keyPath isEqual:@"firstResponder"] && [object isKindOfClass:[NSWindow class]])
    {
        NSLog(@"%@ firstResponder changed to %@", self, [[self window] firstResponder]);
    }

    //[super observeValueForKeyPath:keyPath ofObject:object change:change context:context];
}

@end
