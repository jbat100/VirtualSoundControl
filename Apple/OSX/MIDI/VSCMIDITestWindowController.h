//
//  VSCMIDITestWindowController.h
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 3/19/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@class VSCMIDITestController;

@interface VSCMIDITestWindowController : NSWindowController 

@property (nonatomic, strong) IBOutlet VSCMIDITestController* midiTestController;


@end
