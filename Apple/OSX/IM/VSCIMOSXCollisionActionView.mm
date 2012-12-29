//
//  VSCIMCollisionActionView.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/28/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCIMOSXCollisionActionView.h"
#import "VSCIMOSXCollisionEventChainController.h"
#import "VSCOSXOBSceneElementEditor.h"
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

const CGFloat VSCIMOSXCollisionActionViewBaseHeight = 50.0;
const CGFloat VSCIMOSXCollisionActionViewMIDISetupHeight = 50.0;
const CGFloat VSCIMOSXCollisionActionViewMIDIControlSetupHeight = 26.0;

/*
 *  Private internals
 */

@interface VSCIMOSXCollisionActionView ()

/*
 *  Used by all action types
 */

@property (nonatomic, assign) VSCIMOSXCollisionActionType currentActionType;

/*
 *  Constraints!
 */
@property (strong) IBOutlet NSLayoutConstraint* mainViewBottomConstraint;

-(void) commonInit;
-(void) updateMIDIInterface;
-(void) updateMIDIControlNumbers;

@end


@implementation VSCIMOSXCollisionActionView

static const BOOL debugDraw = NO;

@synthesize collisionAction = _collisionAction;

#pragma mark - Static Methods

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
    if (!actionTypeMenuItemStringDict) {
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
    self.currentActionType = VSCIMOSXCollisionActionTypeNone;
    self.translatesAutoresizingMaskIntoConstraints = NO;
}

- (void)drawRect:(NSRect)dirtyRect
{
    CGContextRef myContext = (CGContextRef)[[NSGraphicsContext currentContext] graphicsPort];
    if (debugDraw)
    {
        switch (self.currentActionType)
        {
            case VSCIMOSXCollisionActionTypeMIDINoteOn:
                CGContextSetRGBFillColor (myContext, 1.0, 0.0, 0.0, 1.0);
                break;
                
            case VSCIMOSXCollisionActionTypeMIDINoteOff:
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
    BOOST_ASSERT(self.mainView);
    self.mainView.translatesAutoresizingMaskIntoConstraints = NO;
    BOOST_ASSERT(self.midiSetupView);
    self.midiSetupView.translatesAutoresizingMaskIntoConstraints = NO;
}

#pragma mark - Custom Setter

-(void) setCollisionAction:(VSC::IM::CollisionAction::WPtr)action {
    
    if (action.lock() != _collisionAction.lock())
    {
        _collisionAction = action;
        self.currentActionType = VSCIMOSXCollisionActionTypeForCollisionAction(_collisionAction.lock());
        [self setupInterfaceForNewCollisionAction];
    }
    
}

#pragma mark - UI Helper

-(void) setupInterfaceForNewCollisionAction
{
    
    VSC::IM::CollisionAction::SPtr action = self.collisionAction.lock();
    
    /*
     *  Setup the view and its subviews according to action type
     */
    
    NSDictionary *viewsDictionary = nil;
    // start from zero
    [[self subviews] makeObjectsPerformSelector:@selector(removeFromSuperview)];
    [self removeConstraints:[self constraints]];
    
    BOOST_ASSERT(self.mainView);
    [self addSubview:self.mainView];
    NSView* mainView = self.mainView;
    
    CGRect f = self.frame;
    f.size.height = [[self class] heightOfViewForCollisionAction:action];
    self.frame = f;
    
    
    viewsDictionary = NSDictionaryOfVariableBindings(mainView);
    [self addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"H:|[mainView]|" options:0
                                                                 metrics:nil views:viewsDictionary]];
    
    NSArray* vConstraints = [NSLayoutConstraint constraintsWithVisualFormat:@"V:|[mainView]|"
                                                                    options:0 metrics:nil views:viewsDictionary];
    
    
    if (VSC::IM::collisionActionIsMIDI(action))
    {
        NSView* midiView = self.midiSetupView;
        
        BOOST_ASSERT(midiView);
        [self addSubview:midiView];
        
        viewsDictionary = NSDictionaryOfVariableBindings(mainView, midiView);
        [self addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"H:|[midiView]|" options:0
                                                                     metrics:nil views:viewsDictionary]];
        
        vConstraints = [NSLayoutConstraint constraintsWithVisualFormat:@"V:|[mainView][midiView]|" options:0
                                                               metrics:nil views:viewsDictionary];

    }
    
    [self addConstraints:vConstraints];
    
    /*
     *  Now that we've set up the view properly, we can update it
     */
    
    [self.actionTypeTextField setStringValue:[[self class] stringForActionType:self.currentActionType]];
    
    if (VSC::IM::collisionActionIsMIDI(action))
    {
        [self updateMIDIInterface];
        
        VSC::IM::CollisionMIDIAction::SPtr midiAction;
        midiAction = boost::dynamic_pointer_cast<VSC::IM::CollisionMIDIAction>(self.collisionAction.lock());
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
}

-(void) updateMIDIInterface
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
        
        VSC::IM::CollisionMIDIAction::SPtr action;
        action = boost::dynamic_pointer_cast<VSC::IM::CollisionMIDIAction>(self.collisionAction.lock());
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
        
        if (VSC::IM::collisionActionIsMIDIControl(action)) [self updateMIDIControlNumbers];
    }
}

-(void) updateMIDIControlNumbers
{
    VSC::IM::CollisionAction::SPtr action = self.collisionAction.lock();
    BOOST_ASSERT(VSC::IM::collisionActionIsMIDIControl(action));
    
    [self.midiControlNumberPopUpButton removeAllItems];
    
    VSC::IM::CollisionMIDIControlAction::SPtr controlAction;
    controlAction = boost::dynamic_pointer_cast<VSC::IM::CollisionMIDIControlAction>(self.collisionAction.lock());
    
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

-(IBAction) showCollisionMappings:(id)sender
{
    BOOST_ASSERT(self.eventChainController);
    BOOST_ASSERT([self.eventChainController respondsToSelector:@selector(sender:requestsShowMappingsForCollisionAction:)]);
    
    [self.eventChainController sender:self requestsShowMappingsForCollisionAction:self.collisionAction.lock()];
}

-(IBAction) refreshMIDIInterface:(id)sender
{
    [self updateMIDIInterface];
}

-(IBAction) midiOutputSelected:(id)sender
{
    BOOST_ASSERT(self.midiOutputPopUpButton);
    VSC::MIDI::OutputManager::SPtr outputManager = VSC::MIDI::OutputManager::singletonManager();
    BOOST_ASSERT(outputManager);
    VSC::IM::CollisionMIDIAction::SPtr action = boost::dynamic_pointer_cast<VSC::IM::CollisionMIDIAction>(self.collisionAction.lock());
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
