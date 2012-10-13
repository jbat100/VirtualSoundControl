//
//  VSCOSXMIDIWindowController.h
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 3/19/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "VSCOSXApplicationManagerProtocol.h"
#import "JAListView.h"

#include "VSCMIDIOutput.h"

@class VSCOSXMIDITestController;
@class VSCOSXMIDITest;

@interface VSCOSXMIDIWindowController : NSWindowController <JAListViewDataSource, JAListViewDelegate, NSComboBoxDelegate, NSComboBoxDataSource>

@property (weak) IBOutlet id<VSCOSXApplicationManagerProtocol> applicationManager;

/*
 *  MIDI Inputs/Outputs
 */

@property (nonatomic, strong) IBOutlet JAListView* midiOutputsListView;
@property (nonatomic, strong) IBOutlet NSButton* refreshInputsButton;
@property (nonatomic, strong) IBOutlet NSButton* refreshOutputsButton;

-(IBAction) refreshInputs:(id)sender;
-(IBAction) refreshOutputs:(id)sender;

/*
 *  MIDI Testing 
 */

@property (nonatomic, assign) VSC::MIDI::Output::SPtr testMIDIOutput;

@property (nonatomic, strong) IBOutlet VSCOSXMIDITest* midiTest;

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

@property (nonatomic, strong) IBOutlet NSSlider* rtControlSlider;
@property (nonatomic, strong) IBOutlet NSComboBox* rtControlChannelComboBox;
@property (nonatomic, strong) IBOutlet NSButton* rtSendControlMessageButton;
@property (nonatomic, strong) IBOutlet NSTextField* rtControlValueTextField;
@property (nonatomic, strong) IBOutlet NSTextField* rtControlChannelTextField;

@property (nonatomic, strong) IBOutlet NSButton* envFireButton;
@property (nonatomic, strong) IBOutlet NSTextField* envUpdateFrequencyTextField;

-(IBAction) sendMidiControlMessage:(id)sender;
-(IBAction) sendMidiNoteOnMessage:(id)sender;
-(IBAction) sendMidiNoteOffMessage:(id)sender;

-(IBAction) controlSliderChangedValue:(id)sender;

-(IBAction) showEnveloppeEditor:(id)sender;
-(IBAction) fireEnveloppe:(id)sender;


@end
