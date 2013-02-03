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
#include "VSCIMEvent.h"
#include "VSCIMAction.h"
#include "VSCIMMIDIActions.h"

#include <boost/assert.hpp>
#include <boost/foreach.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

NSDictionary* actionTypeMenuItemStringDict = nil;

NSString* const VSCIMOSXNoMidiOutputString          = @"No MIDI Output";
NSString* const VSCIMOSXNoMidiControlNumberString   = @"No MIDI Control Number";

/*
 *  Heights should agree with nib (+ <margin>)
 */

const CGFloat VSCIMOSXActionViewBaseHeight = 36.0 + 6;
const CGFloat VSCIMOSXActionViewMIDISetupHeight = 39.0 + 4;
const CGFloat VSCIMOSXActionViewMIDIControlSetupHeight = 15.0;

/*
 *  Private internals
 */

@interface VSCIMOSXActionView ()

/*
 *  Used by all action types
 */

@property (nonatomic, assign) VSC::IM::ActionType currentActionType;

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

@synthesize collisionAction = _collisionAction;

#pragma mark - Static Methods

+(CGFloat) heightOfViewForAction:(VSC::IM::Action::SPtr)collisionAction 
{
    
    CGFloat totalHeight = VSCIMOSXActionViewBaseHeight;
    
    if (VSC::IM::actionIsMIDIAction(collisionAction))
    {
        totalHeight += VSCIMOSXActionViewMIDISetupHeight;
    }
    
    if (VSC::IM::actionIsMIDIControlAction(collisionAction))
    {
        totalHeight += VSCIMOSXActionViewMIDIControlSetupHeight;
    }
    
    return totalHeight;
}

+(void) initialize
{
    if (!actionTypeMenuItemStringDict)
    {
        actionTypeMenuItemStringDict = @{
        @((int)VSC::IM::ActionTypeMIDINoteOn)           : @"MIDI Note On",
        @((int)VSC::IM::ActionTypeMIDINoteOnAndOff)     : @"MIDI Note On and Off",
        @((int)VSC::IM::ActionTypeMIDINoteOff)          : @"MIDI Note Off",
        @((int)VSC::IM::ActionTypeMIDIControlChange)    : @"MIDI Control Change"
        };
    }
}

+(NSString*) stringForActionType:(VSC::IM::ActionType)actionType
{
    BOOST_ASSERT(actionTypeMenuItemStringDict);
    
    return [actionTypeMenuItemStringDict objectForKey:@((int)actionType)];
}

+(VSC::IM::ActionType) actionTypeForString:(NSString*)menuItemString
{
    BOOST_ASSERT(actionTypeMenuItemStringDict);
    
    NSSet* types = [actionTypeMenuItemStringDict keysOfEntriesPassingTest:^BOOL(id key, id obj, BOOL *stop) {
        if ([obj isKindOfClass:[NSString class]] == NO) return false;
        if ([(NSString*)obj isEqualToString:menuItemString]) return true;
        return false;
    }];
    
    BOOST_ASSERT([types count] < 2);
    
    if ([types count] == 0) return VSC::IM::ActionTypeNone;
    
    return (VSC::IM::ActionType)[[types anyObject] intValue];
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
    self.currentActionType = VSC::IM::ActionTypeNone;
    self.translatesAutoresizingMaskIntoConstraints = NO;
}

- (void)drawRect:(NSRect)dirtyRect
{
    CGContextRef myContext = (CGContextRef)[[NSGraphicsContext currentContext] graphicsPort];
    if (debugDraw)
    {
        switch (self.currentActionType)
        {
            case VSC::IM::ActionTypeMIDINoteOn:
                CGContextSetRGBFillColor (myContext, 1.0, 0.0, 0.0, 1.0);
                break;
                
            case VSC::IM::ActionTypeMIDINoteOff:
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

-(VSC::IM::Action_SPtr) action
{
    VSC::IM::Event::SPtr actionEvent = self.event.lock();
    BOOST_ASSERT(actionEvent);
    VSC::IM::Action::SPtr action = boost::dynamic_pointer_cast<VSC::IM::Action>(actionEvent);
    BOOST_ASSERT(action);
    return action;
}

#pragma mark - Custom Setter

-(void) setEvent:(VSC::IM::Event::WPtr)weakEvent
{
    [super setEvent:weakEvent];
    self.currentActionType = VSC::IM::VSC::IM::actionTypeForAction(weakEvent.lock());
    [self setupInterfaceForNewAction];
}

-(BOOL) checkEvent:(VSC::IM::Event::SPtr)testEvent
{
    BOOST_ASSERT(testEvent);
    if (!testEvent) return YES;
    VSC::IM::Action::SPtr action = boost::dynamic_pointer_cast<VSC::IM::Action>(testEvent);
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
    
    VSC::IM::Action::SPtr action = [self action];
    
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
    
    
    if (VSC::IM::actionIsMIDIAction(action))
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
    
    [self.actionTypeTextField setStringValue:[[self class] stringForActionType:self.currentActionType]];
    
    if (VSC::IM::actionIsMIDIAction(action))
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
        
        VSC::IM::Action::SPtr action = [self action];
        VSC::IM::MIDIAction::SPtr collisionAction = boost::dynamic_pointer_cast<VSC::IM::MIDIAction>(action);
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
        
        if (VSC::IM::actionIsMIDIControlAction(action)) [self updateMIDIControlNumbers];
    }
}

-(void) updateMIDIChannel
{
    VSC::IM::MIDIAction::SPtr midiAction = boost::dynamic_pointer_cast<VSC::IM::MIDIAction>([self action]);
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
    VSC::IM::Action::SPtr action = [self action];
    BOOST_ASSERT(VSC::IM::actionIsMIDIControlAction(action));
    
    [self.midiControlNumberPopUpButton removeAllItems];
    
    VSC::IM::MIDIControlAction::SPtr controlAction;
    controlAction = boost::dynamic_pointer_cast<VSC::IM::MIDIControlAction>(self.collisionAction.lock());
    
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
    VSC::IM::MIDIAction::SPtr action = boost::dynamic_pointer_cast<VSC::IM::MIDIAction>([self action]);
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
    VSC::IM::MIDIControlAction::SPtr action = boost::dynamic_pointer_cast<VSC::IM::MIDIControlAction>([self action]);
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
