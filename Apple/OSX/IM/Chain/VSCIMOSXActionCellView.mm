//
//  VSCIMActionView.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/28/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCIMOSXActionCellView.h"
#import "VSCIMOSXEventChainController.h"
#import "VSCOBOSXElementEditor.h"
#import "VSCOSXInterfaceFactory.h"
#import "NSString+VSCAdditions.h"

#include "VSCMIDI.h"
#include "VSCMIDIOutputManager.h"
#include "VSCMIDIOwners.h"

#include "VSCIMEvent.h"
#include "VSCIMAction.h"
#include "VSCIMActionImplementations.h"

#include <boost/assert.hpp>
#include <boost/foreach.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

using namespace VSC;
using namespace VSC::IM;

NSString* const VSCIMOSXNoMidiOutputString          = @"No MIDI Output";
NSString* const VSCIMOSXNoMidiControlNumberString   = @"No MIDI Control Number";


/*
 *  Private internals
 */

@interface VSCIMOSXActionCellView ()

/*
 *  Constraints!
 */
@property (strong) IBOutlet NSLayoutConstraint* mainViewBottomConstraint;

-(void) commonInit;
-(void) reset;
-(void) updateMIDIOutputs;
-(void) updateMIDIControlNumbers;


@end


@implementation VSCIMOSXActionCellView

static const BOOL debugDraw = NO;

#pragma mark - Static Methods

+(void) initialize
{

}

#pragma mark - NSView Methods

- (id)initWithFrame:(NSRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        [self commonInit];
    }
    
    return self;
}

-(id) initWithCoder:(NSCoder *)aDecoder
{
    self = [super initWithCoder:aDecoder];
    if (self) {
        [self commonInit];
    }
    return self;
}

-(void) commonInit
{
    self.translatesAutoresizingMaskIntoConstraints = NO;
}

- (void)drawRect:(NSRect)dirtyRect
{
    CGContextRef myContext = (CGContextRef)[[NSGraphicsContext currentContext] graphicsPort];
    if (debugDraw)
    {
        CGContextSetRGBFillColor (myContext, 1.0, 0.0, 0.0, 1.0);
        CGContextFillRect(myContext, self.bounds);
        CGContextSetGrayStrokeColor (myContext, 1.0, 1.0);
        CGContextStrokeRectWithWidth(myContext, NSRectToCGRect(self.bounds), 2.0);
    }
    else
    {
        CGContextSetRGBFillColor (myContext, 0.2, 0.2, 0.2, 1.0);
        CGContextFillRect(myContext, self.bounds);
        CGContextSetGrayStrokeColor (myContext, 0.8, 1.0);
        CGContextStrokeRectWithWidth(myContext, NSRectToCGRect(self.bounds), 2.0);
    }
}

-(void) awakeFromNib
{
    self.translatesAutoresizingMaskIntoConstraints = NO;
    
    //BOOST_ASSERT(self.mainView);
    //self.mainView.translatesAutoresizingMaskIntoConstraints = NO;
    //BOOST_ASSERT(self.midiOutputView);
    //self.midiOutputView.translatesAutoresizingMaskIntoConstraints = NO;
}

#pragma mark - Action Getter

-(Action_SPtr) action
{
    Event::SPtr actionEvent = self.event.lock();
    BOOST_ASSERT(actionEvent);
    Action::SPtr action = boost::dynamic_pointer_cast<Action>(actionEvent);
    BOOST_ASSERT(action);
    return action;
}

#pragma mark - Custom Setter

-(void) setEvent:(Event::WPtr)weakEvent
{
    [super setEvent:weakEvent];
    Action::SPtr action = boost::dynamic_pointer_cast<Action>(weakEvent.lock());
    BOOST_ASSERT(action);
    [self setupInterfaceForNewAction];
}

-(BOOL) checkEvent:(Event::SPtr)testEvent
{
    BOOST_ASSERT(testEvent);
    if (!testEvent) return YES;
    Action::SPtr action = boost::dynamic_pointer_cast<Action>(testEvent);
    BOOST_ASSERT(action);
    if (action) return YES;
    return NO;
}

#pragma mark - UI Helper

-(void) reset
{
    // start from zero
    [[self subviews] makeObjectsPerformSelector:@selector(removeFromSuperview)];
    [self removeConstraints:[self constraints]];
}


#pragma mark Debugging

-(void) printUIDescription
{
    NSLog(@"%@ %@, mainView.frame %@, midiOutputView.frame %@", self, NSStringFromRect(self.frame),
          NSStringFromRect(self.mainView.frame), NSStringFromRect(self.midiOutputView.frame));
}

@end
