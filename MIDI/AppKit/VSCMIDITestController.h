//
//  VSCMIDITestController.h
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 3/21/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Foundation/Foundation.h>

@class VSCMIDITestView;
@class VSCMIDITest;

@interface VSCMIDITestController : NSObject <NSTableViewDelegate, NSTableViewDataSource, NSComboBoxDelegate, NSComboBoxDataSource>

@property (nonatomic, strong) IBOutlet VSCMIDITest* midiTest;
@property (nonatomic, strong) IBOutlet VSCMIDITestView* midiTestView;

@property (nonatomic, strong) NSArray* controlChannels; 

-(void) setupView;

-(IBAction) refreshInputs:(id)sender;
-(IBAction) refreshOutputs:(id)sender;
-(IBAction) setMidiOutputWithRowSelection:(id)sender;

-(IBAction) sendMidiControlMessage:(id)sender;
-(IBAction) sendMidiNoteOnMessage:(id)sender;
-(IBAction) sendMidiNoteOffMessage:(id)sender;

-(IBAction) controlSliderChangedValue:(id)sender;

@end
