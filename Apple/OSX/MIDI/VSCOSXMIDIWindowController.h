//
//  VSCOSXMIDIWindowController.h
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 3/19/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#import "VSCOSXApplicationManagerProtocol.h"

@class VSCOSXMIDITestController;
@class VSCOSXMIDITest;

@interface VSCOSXMIDIWindowController : NSWindowController <NSTableViewDelegate, NSTableViewDataSource>

@property (nonatomic, strong) IBOutlet VSCOSXMIDITestController* midiTestController;
@property (nonatomic, strong) IBOutlet VSCOSXMIDITest* midiTest;

@property (nonatomic, strong) IBOutlet NSTableView* midiInputsTable;
@property (nonatomic, strong) IBOutlet NSTableView* midiOutputsTable;

@property (nonatomic, strong) IBOutlet NSButton* refreshInputsButton;
@property (nonatomic, strong) IBOutlet NSButton* refreshOutputsButton;

@property (weak) IBOutlet id<VSCOSXApplicationManagerProtocol> applicationManager;

@end
