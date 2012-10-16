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

#include "VSCMIDI.h"
#include "VSCMIDIOutput.h"

//@class VSCOSXMIDITestController;
//@class VSCOSXMIDITest;

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

-(void) updateMIDIOutputInterface;

/*
 *  MIDI Testing 
 */

@property (nonatomic, assign) VSC::MIDI::Output::SPtr testMIDIOutput;

@property (nonatomic, strong) IBOutlet NSPopUpButton* midiOutputPopUpButton;
@property (nonatomic, strong) IBOutlet NSPopUpButton* controlNumberPopUpButton;

@property (nonatomic, strong) IBOutlet NSTextField* channelTextField;
@property (nonatomic, strong) IBOutlet NSTextField* controlValueTextField;
@property (nonatomic, strong) IBOutlet NSTextField* pitchTextField;
@property (nonatomic, strong) IBOutlet NSTextField* velocityTextField;
@property (nonatomic, strong) IBOutlet NSSlider* controlValueSlider;


@property (nonatomic, assign) unsigned int channel;
@property (nonatomic, assign) unsigned int controlValue;
@property (nonatomic, assign) unsigned int pitch;
@property (nonatomic, assign) unsigned int velocity;

@property (nonatomic, assign) VSC::MIDI::ControlNumber controlNumber;

-(IBAction) midiOutputSelected:(id)sender;
-(IBAction) controlNumberSelected:(id)sender;

-(IBAction) sendMidiControlMessage:(id)sender;
-(IBAction) sendMidiNoteOnMessage:(id)sender;
-(IBAction) sendMidiNoteOffMessage:(id)sender;

-(IBAction) controlSliderChangedValue:(id)sender;

-(IBAction) showEnveloppeEditor:(id)sender;
-(IBAction) fireEnveloppe:(id)sender;


@end
