//
//  VSCIMActionView.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/28/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCIMOSXActionView.h"
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

NSDictionary* actionTypeMenuItemStringDict = nil;

NSString* const VSCIMOSXNoMidiOutputString          = @"No MIDI Output";
NSString* const VSCIMOSXNoMidiControlNumberString   = @"No MIDI Control Number";

/*
 *  Heights should agree with nib (+ <margin>)
 */

const CGFloat VSCIMOSXActionViewBaseHeight = 36.0 + 6;
const CGFloat VSCIMOSXActionViewMIDIOutputSetupHeight = 39.0 + 4;
const CGFloat VSCIMOSXActionViewMIDIChannelSetupHeight = 39.0 + 4;
const CGFloat VSCIMOSXActionViewMIDIControlSetupHeight = 15.0;

/*
 *  Private internals
 */

@interface VSCIMOSXActionView ()

/*
 *  Constraints!
 */
@property (strong) IBOutlet NSLayoutConstraint* mainViewBottomConstraint;

-(void) commonInit;
-(void) reset;
-(void) updateMIDIOutputs;
-(void) updateMIDIControlNumbers;

@end


@implementation VSCIMOSXActionView

static const BOOL debugDraw = NO;

#pragma mark - Static Methods

+(CGFloat) heightOfViewForAction:(Action::SPtr)action
{
    
    CGFloat totalHeight = VSCIMOSXActionViewBaseHeight;
    
    Action::Implementation::SPtr implementation = action->getImplementation();
    
    MIDI::OutputOwner::SPtr midiOutputOwner = boost::dynamic_pointer_cast<MIDI::OutputOwner>(implementation);
    if (midiOutputOwner)
    {
        totalHeight += VSCIMOSXActionViewMIDIOutputSetupHeight;
    }
    
    MIDI::ChannelOwner::SPtr midiChanneltOwner = boost::dynamic_pointer_cast<MIDI::ChannelOwner>(implementation);
    if (midiChanneltOwner)
    {
        totalHeight += VSCIMOSXActionViewMIDIChannelSetupHeight;
    }
    
    MIDI::ControlNumberOwner::SPtr midiControlNumberOwner = boost::dynamic_pointer_cast<MIDI::ControlNumberOwner>(implementation);
    if (midiControlNumberOwner)
    {
        totalHeight += VSCIMOSXActionViewMIDIControlSetupHeight;
    }
    
    return totalHeight;
}

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
    self.currentActionType = ActionTypeNone;
    self.translatesAutoresizingMaskIntoConstraints = NO;
}

- (void)drawRect:(NSRect)dirtyRect
{
    CGContextRef myContext = (CGContextRef)[[NSGraphicsContext currentContext] graphicsPort];
    if (debugDraw)
    {
        switch (self.currentActionType)
        {
            case ActionTypeMIDINoteOn:
                CGContextSetRGBFillColor (myContext, 1.0, 0.0, 0.0, 1.0);
                break;
                
            case ActionTypeMIDINoteOff:
                CGContextSetRGBFillColor (myContext, 0.0, 1.0, 0.0, 1.0);
                break;
                
            default:
                CGContextSetRGBFillColor (myContext, 0.4, 0.4, 0.4, 1.0);
        }
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
    //BOOST_ASSERT(self.midiSetupView);
    //self.midiSetupView.translatesAutoresizingMaskIntoConstraints = NO;
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

-(void) setupInterfaceForNewAction
{
    
    Action::SPtr action = [self action];
    
    /*
     *  Setup the view and its subviews according to action type
     */
    
    NSMutableArray* viewsForHorizontalConstraints = [NSMutableArray array];
    NSMutableString* verticalLayoutVisualFormat = [NSMutableString stringWithString:@"V:|"];
    NSMutableDictionary* viewBindingsDictionary = [NSMutableDictionary dictionary];
    
    if (self.mainView == nil)
    {
        self.mainView = [[VSCOSXInterfaceFactory defaultFactory] newActionCommonViewWithOwner:self];
    }
    
    BOOST_ASSERT(self.mainView);
    [self addSubview:self.mainView];
    [viewsForHorizontalConstraints addObject:self.mainView];
    [viewBindingsDictionary setValue:self.mainView forKey:@"mainView"];
    [verticalLayoutVisualFormat appendString:@"-2-[mainView]"];
    
    
    if (actionIsMIDIAction(action))
    {
        if (self.midiSetupView == nil)
        {
            self.midiSetupView = [[VSCOSXInterfaceFactory defaultFactory] newActionMIDIViewWithOwner:self];
        }
        
        BOOST_ASSERT(self.midiSetupView);
        [self addSubview:self.midiSetupView];
        [viewsForHorizontalConstraints addObject:self.midiSetupView];
        [viewBindingsDictionary setValue:self.midiSetupView forKey:@"midiSetupView"];
        [verticalLayoutVisualFormat appendString:@"-4-[midiSetupView]"];
    }
    
    [verticalLayoutVisualFormat appendString:@"-4-|"];
    
    /*
     *  Apply constraints
     */

    [self addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:verticalLayoutVisualFormat
                                                                 options:0
                                                                 metrics:nil
                                                                   views:viewBindingsDictionary]];
    
    for (NSView* viewForConstraint in viewsForHorizontalConstraints)
    {
        NSDictionary *localViewsDictionary = NSDictionaryOfVariableBindings(viewForConstraint);
        
        NSArray* contraints = [NSLayoutConstraint constraintsWithVisualFormat:@"H:|-4-[viewForConstraint]-4-|"
                                                                      options:0
                                                                      metrics:nil
                                                                        views:localViewsDictionary];
        
        [self addConstraints:contraints];
    }
    
    /*
     *  Update interface
     */
    
    [self.actionTypeTextField setStringValue:[NSString stringWithStdString:StringForActionType(self.currentActionType)]];
    
    if (actionIsMIDIAction(action))
    {
        [self updateMIDIOutputs];
        [self updateMIDIChannel];
    }
}

-(void) updateMIDIOutputs
{
    VSC::MIDI::OutputManager::SPtr outputManager = VSC::MIDI::OutputManager::singletonManager();
    BOOST_ASSERT(outputManager);
    
    [self.midiOutputPopUpButton removeAllItems];
    [self.midiOutputPopUpButton addItemWithTitle:VSCIMOSXNoMidiOutputString];
    
    if (outputManager)
    {
        const VSC::MIDI::Outputs& outputs = outputManager->getOutputs();
        
        BOOST_FOREACH(const VSC::MIDI::Output::SPtr& output, outputs)
        {
            NSString* title = [NSString stringWithStdString:output->getDescription()];
            [self.midiOutputPopUpButton addItemWithTitle:title];
        }
        
        Action::SPtr action = [self action];
        MIDIAction::SPtr collisionAction = boost::dynamic_pointer_cast<MIDIAction>(action);
        BOOST_ASSERT(collisionAction);
        
        if (collisionAction)
        {
            VSC::MIDI::Output::SPtr midiOutput = collisionAction->getMIDIOutput();
            if (midiOutput)
            {
                NSString* title = [NSString stringWithStdString:midiOutput->getDescription()];
                [self.midiOutputPopUpButton selectItemWithTitle:title];
            }
            else
            {
                [self.midiOutputPopUpButton selectItemWithTitle:VSCIMOSXNoMidiOutputString];
            }
        }
        else
        {
            [self.midiOutputPopUpButton selectItemWithTitle:VSCIMOSXNoMidiOutputString];
        }
        
        if (actionIsMIDIControlAction(action)) [self updateMIDIControlNumbers];
    }
}

-(void) updateMIDIChannel
{
    MIDIAction::SPtr midiAction = boost::dynamic_pointer_cast<MIDIAction>([self action]);
    BOOST_ASSERT(midiAction);
    
    if (midiAction)
    {
        [self.midiChannelTextField setIntegerValue:midiAction->getChannel()];
    }
    else
    {
        [self.midiChannelTextField setStringValue:@"No channel"];
    }
}

-(void) updateMIDIControlNumbers
{
    Action::SPtr action = [self action];
    BOOST_ASSERT(actionIsMIDIControlAction(action));
    
    [self.midiControlNumberPopUpButton removeAllItems];
    
    MIDIControlAction::SPtr controlAction;
    controlAction = boost::dynamic_pointer_cast<MIDIControlAction>([self action]);
    
    BOOST_ASSERT(controlAction);
    
    VSC::MIDI::Output::SPtr midiOutput = controlAction->getMIDIOutput();
    
    if (midiOutput)
    {
        const VSC::MIDI::ControlNumbers& controlNumbers = midiOutput->getValidControlNumbers();
        
        BOOST_FOREACH(const VSC::MIDI::ControlNumber& controlNumber, controlNumbers)
        {
            std::string controlNumberString = VSC::MIDI::controlNumberToString(controlNumber);
            BOOST_ASSERT(!controlNumberString.empty());
            if (!controlNumberString.empty())
            {
                [self.midiControlNumberPopUpButton addItemWithTitle:[NSString stringWithStdString:controlNumberString]];
            }
        }
    }
}

#pragma mark - UI Callbacks


-(IBAction) refreshMIDIOutputs:(id)sender
{
    [self updateMIDIOutputs];
}

-(IBAction) midiOutputSelected:(id)sender
{
    BOOST_ASSERT(self.midiOutputPopUpButton);
    VSC::MIDI::OutputManager::SPtr outputManager = VSC::MIDI::OutputManager::singletonManager();
    BOOST_ASSERT(outputManager);
    MIDIAction::SPtr action = boost::dynamic_pointer_cast<MIDIAction>([self action]);
    BOOST_ASSERT(action);
    
    if (action && outputManager && self.midiOutputPopUpButton)
    {
        NSString* title = [[self.midiOutputPopUpButton selectedItem] title];
        if (title && [title isEqualToString:VSCIMOSXNoMidiOutputString] == NO)
        {
            VSC::MIDI::Output::SPtr midiOutput = outputManager->getOutputWithDescription([title stdString]);
        }
        else if (title && [title isEqualToString:VSCIMOSXNoMidiOutputString] == NO)
        {
            action->setMIDIOutput(VSC::MIDI::Output::SPtr());
            [self.midiOutputPopUpButton selectItemWithTitle:VSCIMOSXNoMidiOutputString];
        }
        else
        {
            action->setMIDIOutput(VSC::MIDI::Output::SPtr());
            [self.midiOutputPopUpButton selectItemWithTitle:VSCIMOSXNoMidiOutputString];
        }
    }
    else
    {
        [self.midiOutputPopUpButton selectItemWithTitle:VSCIMOSXNoMidiOutputString];
    }
}

-(IBAction) refreshMIDIControlNumbers:(id)sender
{
    [self updateMIDIControlNumbers];
}

-(IBAction) midiControlNumberSelected:(id)sender
{
    BOOST_ASSERT(self.midiControlNumberPopUpButton);
    MIDIControlAction::SPtr action = boost::dynamic_pointer_cast<MIDIControlAction>([self action]);
    BOOST_ASSERT(action);
    
    if (action && self.midiControlNumberPopUpButton)
    {
        NSString* title = [[self.midiControlNumberPopUpButton selectedItem] title];
        if (title && [title isEqualToString:VSCIMOSXNoMidiControlNumberString] == NO)
        {
            VSC::MIDI::ControlNumber controlNumber = VSC::MIDI::stringToControlNumber([title stdString]);
            action->setControlNumber(controlNumber);
        }
        else
        {
            action->setControlNumber(VSC::MIDI::ControlNone);
            [self.midiControlNumberPopUpButton selectItemWithTitle:VSCIMOSXNoMidiControlNumberString];
        }
    }
    else
    {
        [self.midiControlNumberPopUpButton selectItemWithTitle:VSCIMOSXNoMidiControlNumberString];
    }
}

#pragma mark Debugging

-(void) printUIDescription
{
    NSLog(@"%@ %@, mainView.frame %@, midiSetupView.frame %@", self, NSStringFromRect(self.frame),
          NSStringFromRect(self.mainView.frame), NSStringFromRect(self.midiSetupView.frame));
}

@end
