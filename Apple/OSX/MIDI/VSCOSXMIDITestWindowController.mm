//
//  VSCOSXMIDITestWindowController.m
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 3/19/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCOSXMIDITestWindowController.h"

#import "VSCOSXMIDITestController.h"

@interface VSCOSXMIDITestWindowController ()

@end

@implementation VSCOSXMIDITestWindowController

@synthesize midiTestController = _midiTestController;

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
    
    [self.midiTestController setupView];
    
    
}
 

@end
