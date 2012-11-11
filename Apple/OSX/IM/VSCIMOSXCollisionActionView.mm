//
//  VSCIMCollisionActionView.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/28/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCIMOSXCollisionActionView.h"
#import "VSCIMOSXCollisionEventChainController.h"
#import "NSString+VSCAdditions.h"

#include "VSCMIDI.h"
#include "VSCMIDIOutputManager.h"
#include "VSCIMCollisionMIDIActions.h"

#include <boost/assert.hpp>
#include <boost/foreach.hpp>

NSDictionary* actionTypeMenuItemStringDict = nil;

NSString* const VSCIMOSXNoMidiOutputString          = @"No MIDI Output";
NSString* const VSCIMOSXNoMidiControlNumberString   = @"No MIDI Control Number";

/*
 *  Heights should agree with nib
 */

const CGFloat VSCIMOSXCollisionActionViewBaseHeight = 54.0;
const CGFloat VSCIMOSXCollisionActionViewMIDISetupHeight = 48.0;
const CGFloat VSCIMOSXCollisionActionViewMIDIControlSetupHeight = 26.0;

/** // Using nib instead...

const CGFloat VSCIMOSXCollisionActionViewHorizontalMargin   = 5.0;
const CGFloat VSCIMOSXCollisionActionViewVerticalMargin     = 5.0;

const CGFloat VSCIMOSXCollisionActionViewPopUpButtonHeight  = 10.0;

const CGFloat VSCIMOSXCollisionActionViewBaseHeight         = 20.0;
const CGFloat VSCIMOSXCollisionActionViewMappingHeight      = 20.0;
const CGFloat VSCIMOSXCollisionActionViewParameterHeight    = 20.0;
 
 */

/*
 *  Private internals
 */

@interface VSCIMOSXCollisionActionView ()

/*
 *  Used by all action types
 */

@property (nonatomic, assign) VSCIMOSXCollisionActionType currentActionType;
//@property (nonatomic, strong) IBOutlet NSPopUpButton* actionTypePopUpButton;

@property (nonatomic, strong) IBOutlet NSView* mainView;
@property (nonatomic, strong) IBOutlet NSTextField* actionTypeTextField;
@property (nonatomic, strong) IBOutlet NSButton* mappingsButton;

-(void) setupInterfaceForNewCollisionAction;

/*
 *  Only for MIDI actions
 */

@property (nonatomic, strong) IBOutlet NSView* midiSetupView;
@property (nonatomic, strong) IBOutlet NSPopUpButton* midiOutputPopUpButton;

/*
 *  Only for MIDI control actions
 */

@property (nonatomic, strong) IBOutlet NSView* midiControlSetupView;
@property (nonatomic, strong) IBOutlet NSPopUpButton* midiControlNumberPopUpButton;

-(void) updateMIDIControlNumbers;

/*
 *  Action view callbacks
 */

-(IBAction) showCollisionMappings:(id)sender;
-(IBAction) refreshMIDIOutputs:(id)sender;
-(IBAction) midiOutputSelected:(id)sender;
-(IBAction) refreshMIDIControlNumbers:(id)sender;
-(IBAction) midiControlNumberSelected:(id)sender;

@end


@implementation VSCIMOSXCollisionActionView

@synthesize collisionAction = _collisionAction;

+(CGFloat) heightOfViewForCollisionAction:(VSC::IM::CollisionAction::SPtr)collisionAction 
{
    
    CGFloat totalHeight = VSCIMOSXCollisionActionViewBaseHeight;
    
    if (VSC::IM::collisionActionIsMIDI(collisionAction))
    {
        totalHeight += VSCIMOSXCollisionActionViewMIDISetupHeight;
    }
    
    if (VSC::IM::collisionActionIsMIDIControl(collisionAction))
    {
        totalHeight += VSCIMOSXCollisionActionViewMIDIControlSetupHeight;
    }
    
    return totalHeight;
}

+(void) initialize
{
    if (actionTypeMenuItemStringDict) {
        actionTypeMenuItemStringDict = @{
        @((int)VSCIMOSXCollisionActionTypeMIDINoteOn)           : @"MIDI Note On",
        @((int)VSCIMOSXCollisionActionTypeMIDINoteOnAndOff)     : @"MIDI Note On and Off",
        @((int)VSCIMOSXCollisionActionTypeMIDINoteOff)          : @"MIDI Note Off",
        @((int)VSCIMOSXCollisionActionTypeMIDIControlChange)    : @"MIDI Control Change"
        };
    }
}

+(NSString*) stringForActionType:(VSCIMOSXCollisionActionType)actionType
{
    BOOST_ASSERT(actionTypeMenuItemStringDict);
    
    return [actionTypeMenuItemStringDict objectForKey:@((int)actionType)];
}

+(VSCIMOSXCollisionActionType) actionTypeForString:(NSString*)menuItemString
{
    BOOST_ASSERT(actionTypeMenuItemStringDict);
    
    NSSet* types = [actionTypeMenuItemStringDict keysOfEntriesPassingTest:^BOOL(id key, id obj, BOOL *stop) {
        if ([obj isKindOfClass:[NSString class]] == NO) return false;
        if ([(NSString*)obj isEqualToString:menuItemString]) return true;
        return false;
    }];
    
    BOOST_ASSERT([types count] < 2);
    
    if ([types count] == 0) return VSCIMOSXCollisionActionTypeNone;
    
    return (VSCIMOSXCollisionActionType)[[types anyObject] intValue];
}

- (id)initWithFrame:(NSRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code here.
        
        self.currentActionType = VSCIMOSXCollisionActionTypeNone;
        
    }
    
    return self;
}

- (void)drawRect:(NSRect)dirtyRect
{
    // Drawing code here.
}

-(void) awakeFromNib
{
    [self setAutoresizesSubviews:YES];
    
    [self.midiSetupView setAutoresizingMask:(NSUInteger)(NSViewWidthSizable | NSViewMinYMargin)];
}


#pragma mark - Custom Setter

-(void) setCollisionAction:(VSC::IM::CollisionAction::WPtr)action {
    
    _collisionAction = action;
    
    self.currentActionType = VSCIMOSXCollisionActionTypeForCollisionAction(_collisionAction.lock());
    
    [self setupInterfaceForNewCollisionAction];
    
}

#pragma mark - UI Helper



-(void) setupInterfaceForNewCollisionAction
{
    
    VSC::IM::CollisionAction::SPtr action = self.collisionAction.lock();
    
    /*
     *  If action then tighten your seat belts, here we go...
     */
    
    CGFloat h = [[self class] heightOfViewForCollisionAction:action];
    
    NSRect f = self.frame;
    f.size.height = h;
    self.frame = f;
    
    [self.actionTypeTextField setStringValue:[[self class] stringForActionType:self.currentActionType]];
    
    h -= VSCIMOSXCollisionActionViewBaseHeight;
    
    if (!action)
    {
        // empty interface and return
        
        [self.mappingsButton setEnabled:NO];
    }
    
    else
    {
    
        [self.mappingsButton setEnabled:YES];
        
        if (VSC::IM::collisionActionIsMIDI(action))
        {
            f = NSMakeRect(0.0, h, self.frame.size.width, VSCIMOSXCollisionActionViewMIDISetupHeight);
            self.midiSetupView.frame = f;
            
            if ([self.midiSetupView superview] != self)
            {
                
            }
        }
        
    }
}

-(void) updateMIDIControlNumbers
{
    //BOOST_ASSERT(self.currentActionType == VSCIMOSXCollisionActionTypeMIDIControlChange);
    
    [self.midiControlNumberPopUpButton removeAllItems];
    
    VSC::IM::CollisionMIDIAction::SPtr midiAction = boost::dynamic_pointer_cast<VSC::IM::CollisionMIDIAction>(self.collisionAction.lock());
    
    BOOST_ASSERT(midiAction);
    
    VSC::MIDI::Output::SPtr midiOutput = midiAction->getMIDIOutput();
    
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

-(IBAction) showCollisionMappings:(id)sender
{
    [self.eventChainController showCollisionMappingsForCollisionAction:self.collisionAction.lock()];
}

-(IBAction) refreshMIDIOutputs:(id)sender
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
        
        VSC::IM::CollisionMIDIAction::SPtr action = boost::dynamic_pointer_cast<VSC::IM::CollisionMIDIAction>(self.collisionAction.lock());
        
        BOOST_ASSERT(action);
        
        if (action)
        {
            VSC::MIDI::Output::SPtr midiOutput = action->getMIDIOutput();
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
    }
    
    [self updateMIDIControlNumbers];
    
}

-(IBAction) midiOutputSelected:(id)sender
{
    BOOST_ASSERT(self.midiOutputPopUpButton);
    VSC::MIDI::OutputManager::SPtr outputManager = VSC::MIDI::OutputManager::singletonManager();
    BOOST_ASSERT(outputManager);
    VSC::IM::CollisionMIDIAction::SPtr action = boost::dynamic_pointer_cast<VSC::IM::CollisionMIDIAction>(self.collisionAction.lock());
    BOOST_ASSERT(action);
    
    if (action && outputManager)
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
    VSC::IM::CollisionMIDIControlAction::SPtr action = boost::dynamic_pointer_cast<VSC::IM::CollisionMIDIControlAction>(self.collisionAction.lock());
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



@end
