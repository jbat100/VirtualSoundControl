//
//  VSCOSXMIDIWindowController.m
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 3/19/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCOSXMIDIWindowController.h"

#import "VSCOSXMIDITestController.h"

@interface VSCOSXMIDIWindowController ()

@end

@implementation VSCOSXMIDIWindowController


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

-(IBAction) refreshInputs:(id)sender {
    //NSAssert(midiTest, @"_midi is NULL");
    if ([self.midiTest getMidi]) {
        [self.midiTest getMidi]->refreshInputPorts();
        //const VSCMIDI::OutputPortList l = [self.midiTest getMidi]->getOutputPorts();
        std::cout << [self.midiTest getMidi]->outputPortDescription() << std::endl;
    }
    [self.midiTestView.midiInputsTable reloadData];
}

-(IBAction) refreshOutputs:(id)sender {
    NSAssert([self.midiTest getMidi], @"_midi is NULL");
    if ([self.midiTest getMidi]) {
        [self.midiTest getMidi]->refreshOutputPorts();
        std::cout << [self.midiTest getMidi]->outputPortDescription();
    }
    [self.midiTestView.midiOutputsTable reloadData];
}

-(IBAction) setMidiOutputWithRowSelection:(id)sender {
    NSAssert([self.midiTest getMidi], @"_midi is NULL");
    if ([self.midiTest getMidi]) {
        
        NSInteger rowIndex = [self.midiTestView.midiOutputsTable selectedRow];
        
        if (rowIndex < 0) {
            return;
        }
        
        const std::list<VSCMIDIOutputPort> portList = [self.midiTest getMidi]->getOutputPorts();
        std::list<VSCMIDIOutputPort>::const_iterator portIt = [self.midiTest getMidi]->getOutputPorts().begin();
        std::advance(portIt, rowIndex);
        VSCMIDIOutputPort outputPort = *portIt;
        [self.midiTest setMidiOutput:VSCMIDIOutputPtr(new VSCMIDIOutput(outputPort))];
        
        [self updateMidiOutputTextField];
        [self.midiTestView.midiOutputsTable reloadData];
        
    }
    
}



#pragma mark - NSTableView Delegate/Datasource

- (NSInteger)numberOfRowsInTableView:(NSTableView *)aTableView {
    
    if (aTableView == self.midiTestView.midiInputsTable) {
        return [self.midiTest getMidi]->getInputPorts().size();
    }
    
    else if (aTableView == self.midiTestView.midiOutputsTable) {
        return [self.midiTest getMidi]->getOutputPorts().size();
    }
    
    return 0;
}

- (id)tableView:(NSTableView *)aTableView objectValueForTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex {
    
    NSString* columnIdentifier = [aTableColumn identifier];
    
    if (aTableView == self.midiTestView.midiInputsTable) {
        
        const std::list<VSCMIDIInputPort> portList = [self.midiTest getMidi]->getInputPorts();
        std::list<VSCMIDIInputPort>::const_iterator portIt = [self.midiTest getMidi]->getInputPorts().begin();
        std::advance(portIt, rowIndex);
        
        if ([columnIdentifier isEqualToString:VSCMIDIPortNameColumnIdentifier]) {
            return [NSString stringWithStdString:portIt->name];
        }
        else if ([columnIdentifier isEqualToString:VSCMIDIPortNumberColumnIdentifier]) {
            return [NSNumber numberWithUnsignedInt:portIt->number];
        }
        else if ([columnIdentifier isEqualToString:VSCMIDIPortVirtualColumnIdentifier]) {
            return portIt->isVirtual ? @"Yes" : @"No";
        }
        else if ([columnIdentifier isEqualToString:VSCMIDIPortSelectedColumnIdentifier]) {
            return @"No";
        }
        
    }
    
    if (aTableView == self.midiTestView.midiOutputsTable) {
        
        const std::list<VSCMIDIOutputPort> portList = [self.midiTest getMidi]->getOutputPorts();
        std::list<VSCMIDIOutputPort>::const_iterator portIt = [self.midiTest getMidi]->getOutputPorts().begin();
        std::advance(portIt, rowIndex);
        
        if ([columnIdentifier isEqualToString:VSCMIDIPortNameColumnIdentifier]) {
            return [NSString stringWithStdString:portIt->name];
        }
        else if ([columnIdentifier isEqualToString:VSCMIDIPortNumberColumnIdentifier]) {
            return [NSNumber numberWithUnsignedInt:portIt->number];
        }
        else if ([columnIdentifier isEqualToString:VSCMIDIPortVirtualColumnIdentifier]) {
            //return portIt->isVirtual ? @"Yes" : @"No";
        }
        else if ([columnIdentifier isEqualToString:VSCMIDIPortSelectedColumnIdentifier]) {
            //return @"No";
            if ([self.midiTest getMidiOutput]) {
                if ([self.midiTest getMidiOutput]->getOutputPort() == (*portIt)) return [NSNumber numberWithBool:YES];
                else return [NSNumber numberWithBool:NO];
            }
        }
        
    }
    
    return @"";
}



 

@end
