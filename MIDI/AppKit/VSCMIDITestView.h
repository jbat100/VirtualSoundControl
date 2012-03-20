//
//  VSCMIDITestView.h
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 3/9/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@class VSCMIDITest;

@interface VSCMIDITestView : NSView <NSTableViewDelegate, NSTableViewDataSource, NSTextFieldDelegate> {
    
    
}

@property (nonatomic, weak) VSCMIDITest* midiTest;

@property (nonatomic, strong) IBOutlet NSTableView* midiInputsTable;
@property (nonatomic, strong) IBOutlet NSTableView* midiOutputsTable;
@property (nonatomic, strong) IBOutlet NSButton* refreshInputsButton;
@property (nonatomic, strong) IBOutlet NSButton* refreshOutputsButton;
@property (nonatomic, strong) IBOutlet NSButton* createMidiOutputButton;

@property (nonatomic, strong) IBOutlet NSTextField* midiOutputTextField;

@property (nonatomic, strong) IBOutlet NSTextField* midiChannelTextField;
@property (nonatomic, strong) IBOutlet NSTextField* controlChannelTextField;
@property (nonatomic, strong) IBOutlet NSTextField* controlValueTextField;
@property (nonatomic, strong) IBOutlet NSTextField* notePitchTextField;
@property (nonatomic, strong) IBOutlet NSTextField* noteVelocityTextField;

@property (nonatomic, strong) IBOutlet NSButton* sendMidiControlButton;
@property (nonatomic, strong) IBOutlet NSButton* sendMidiNoteOnButton;
@property (nonatomic, strong) IBOutlet NSButton* sendMidiNoteOffButton;


-(void) refreshInterface;

-(IBAction) refreshInputs:(id)sender;
-(IBAction) refreshOutputs:(id)sender;
-(IBAction) setMidiOutputWithRowSelection:(id)sender;

-(IBAction) sendMidiControlMessage:(id)sender;
-(IBAction) sendMidiNoteOnMessage:(id)sender;
-(IBAction) sendMidiNoteOffMessage:(id)sender;


@end
