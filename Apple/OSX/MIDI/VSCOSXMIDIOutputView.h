//
//  DemoView.h
//  JAListView
//
//  Created by Josh Abernathy on 9/29/10.
//  Copyright 2010 Maybe Apps. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "JAListViewItem.h"

#include "VSCMIDIOutput.h"

@interface VSCOSXMIDIOutputView : JAListViewItem {
    //NSGradient *gradient;
    //BOOL selected;
    //NSTextField *textField;
    //NSTextField *shadowTextField;
}

+(VSCOSXMIDIOutputView*) midiOutputView;

@property (retain) IBOutlet NSTextField *numberTextField;
@property (retain) IBOutlet NSTextField *nameTextField;
@property (retain) IBOutlet NSTextField *virtualTextField;
@property (retain) IBOutlet NSTextField *statusTextField;

@property (retain) IBOutlet NSButton *mainActionButton;

@property (assign) VSC::MIDI::Output::WPtr midiOutput;

-(IBAction) mainAction:(id)sender;

-(void) refresh;

@end
