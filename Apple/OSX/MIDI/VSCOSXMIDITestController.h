//
//  VSCOSXMIDITestController.h
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 3/21/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@class VSCOSXMIDITestView;
@class VSCOSXMIDITest;


@interface VSCOSXMIDITestController : NSObject <NSComboBoxDelegate, NSComboBoxDataSource>

@property (nonatomic, strong) IBOutlet VSCOSXMIDITest* midiTest;
@property (nonatomic, strong) IBOutlet VSCOSXMIDITestView* midiTestView;

@property (nonatomic, strong) NSArray* controlChannels; 

-(void) setupView;

-(IBAction) refreshInputs:(id)sender;
-(IBAction) refreshOutputs:(id)sender;
-(IBAction) setMidiOutputWithRowSelection:(id)sender;

-(IBAction) sendMidiControlMessage:(id)sender;
-(IBAction) sendMidiNoteOnMessage:(id)sender;
-(IBAction) sendMidiNoteOffMessage:(id)sender;

-(IBAction) controlSliderChangedValue:(id)sender;

-(IBAction) showEnveloppeEditor:(id)sender;
-(IBAction) fireEnveloppe:(id)sender;

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

@end
