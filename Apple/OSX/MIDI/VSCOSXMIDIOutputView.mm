//
//  DemoView.m
//  JAListView
//
//  Created by Josh Abernathy on 9/29/10.
//  Copyright 2010 Maybe Apps. All rights reserved.
//

#import "VSCOSXMIDIOutputView.h"

#import "NSString+VSCAdditions.h"

@interface VSCOSXMIDIOutputView ()

- (void)drawBackground;

@property (nonatomic, readonly) NSGradient *gradient;

@end


@implementation VSCOSXMIDIOutputView

@synthesize selected = _selected;
@synthesize midiOutput = _midiOutput;
@synthesize gradient = _gradient;
//@synthesize textField;
//@synthesize shadowTextField;

+ (VSCOSXMIDIOutputView *)demoView {
    
    static NSNib *nib = nil;
    if(nib == nil) {
        nib = [[NSNib alloc] initWithNibNamed:NSStringFromClass(self) bundle:nil];
    }
    
    NSArray *objects = nil;
    [nib instantiateNibWithOwner:nil topLevelObjects:&objects];
    for(id object in objects) {
        if([object isKindOfClass:self]) {
            return object;
        }
    }
    
    NSAssert1(NO, @"No view of class %@ found.", NSStringFromClass(self));
    
    return nil;
}


#pragma mark NSView

- (void)drawRect:(NSRect)rect {
    [super drawRect:rect];
    [self drawBackground];
}

-(VSC::MIDI::Output::WPtr) midiOutput {
    return _midiOutput;
}

-(void) setMidiOutput:(VSC::MIDI::Output::WPtr)midiOutput {
    _midiOutput = midiOutput;
    [self refresh];
}


-(void) refresh {
    
    VSC::MIDI::Output::SPtr output = self.midiOutput.lock();
    
    if (output) {
        [self.nameTextField setStringValue:[NSString stringWithStdString:output->getOutputPort().name]];
        [self.numberTextField setStringValue:[@(output->getOutputPort().number) stringValue]];
        [self.virtualTextField setStringValue:output->getOutputPort().isVirtual ? @"Virtual" : @"Non-virtual"];
    }
    else {
        [self.nameTextField setStringValue:@"No Output"];
        [self.numberTextField setStringValue:[@(output->getOutputPort().number) stringValue]];
        [self.virtualTextField setStringValue:output->getOutputPort().isVirtual ? @"Virtual" : @"Non-virtual"];
    }
    
    if (output->getState() == VSC::MIDI::Output::StateOpened) {
        [self.mainActionButton setTitle:@"Close"];
        [self.statusTextField setStringValue:@"Opened"];
    }
    else if (output->getState() == VSC::MIDI::Output::StateClosed) {
        [self.mainActionButton setTitle:@"Open"];
        [self.statusTextField setStringValue:@"Closed"];
    }
    else {
        [self.mainActionButton setTitle:@"Unknown Status"];
        [self.statusTextField setStringValue:@"Attempt Open"];
    }
    
}


#pragma mark API

- (void)drawBackground {
    
    [self.gradient drawInRect:self.bounds angle:self.selected ? 270.0f : 90.0f];
    
    [[NSColor colorWithDeviceWhite:0.5f alpha:1.0f] set];
    NSRectFill(NSMakeRect(0.0f, 0.0f, self.bounds.size.width, 1.0f));
    
    [[NSColor colorWithDeviceWhite:0.93f alpha:1.0f] set];
    NSRectFill(NSMakeRect(0.0f, self.bounds.size.height - 1.0f, self.bounds.size.width, 1.0f));
    
}

- (NSGradient *)gradient {
    if(_gradient == nil) {
        _gradient = [[NSGradient alloc] initWithStartingColor:[NSColor colorWithDeviceWhite:0.8f alpha:1.0f]
                                                 endingColor:[NSColor colorWithDeviceWhite:0.85f alpha:1.0f]];
    }
    
    return _gradient;
}

- (void)setSelected:(BOOL)isSelected {
    _selected = isSelected;
    [self setNeedsDisplay:YES];
}

@end
