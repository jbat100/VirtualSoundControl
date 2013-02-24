//
//  VSCIMOSXEventEditorViewController.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 11/27/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCIMOSXEventEditorViewController.h"
#import "VSCIMOSXMappingEditViewController.h"
#import "VSCIMOSXEventChainController.h"
#import "VSCIMOSXMappingCellView.h"

#import "VSCOSXTableSectionView.h"
#import "VSCOSXInterfaceFactory.h"
#import "NSString+VSCAdditions.h"

#include "VSCIMAction.h"
#include "VSCIMActionImplementations.h"

#include "VSCMIDI.h"
#include "VSCMIDIOutput.h"
#include "VSCMIDIOutputManager.h"
#include "VSCMIDIOwners.h"

#include <boost/assert.hpp>
#include <boost/foreach.hpp>

using namespace VSC;
using namespace VSC::IM;

NSString* const VSCIMOSXMappingCellViewReuseIdentifier = @"VSCIMOSXMappingCellViewReuseIdentifier";

NSString* const VSCIMOSXNoMidiOutputString          = @"No MIDI Output";
NSString* const VSCIMOSXNoMidiControlNumberString   = @"No MIDI Control Number";

@interface VSCIMOSXEventEditorViewController ()

@property (nonatomic, weak) IBOutlet NSButton* backToEventChainViewButton;

-(IBAction) backToEventChainView:(id)sender;

/*
 *  Action specific stuff
 */

-(Action::SPtr) action;

@property (nonatomic, strong) IBOutlet NSTextField* eventTypeTextField;

@property (nonatomic, strong) IBOutlet NSView* midiOutputView;
@property (nonatomic, strong) IBOutlet NSPopUpButton* midiOutputPopUpButton;

@property (nonatomic, strong) IBOutlet NSView* midiChannelView;
@property (nonatomic, strong) IBOutlet NSTextField* midiChannelTextField;

@property (nonatomic, strong) IBOutlet NSView* midiControlNumberView;
@property (nonatomic, strong) IBOutlet NSPopUpButton* midiControlNumberPopUpButton;

-(void) updateMIDIOutputs;
-(void) updateMIDIChannel;
-(void) updateMIDIControlNumbers;

-(IBAction) midiOutputSelected:(id)sender;
-(IBAction) midiChannelSelected:(id)sender;
-(IBAction) midiControlNumberSelected:(id)sender;

-(IBAction) refreshMIDIOutputs:(id)sender;
-(IBAction) refreshMIDIControlNumbers:(id)sender;

@end


@implementation VSCIMOSXEventEditorViewController

@synthesize event = _event;

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Initialization code here.
    }
    
    return self;
}

-(void)awakeFromNib
{
    BOOST_ASSERT(self.mappingTableView);
    BOOST_ASSERT(self.mappingTableView.delegate == self);
    BOOST_ASSERT(self.mappingTableView.dataSource == self);
    
    BOOST_ASSERT(self.editingView);
    BOOST_ASSERT(self.mappingTableScrollView);
    BOOST_ASSERT(self.eventTypeTextField);
    
    self.mappingTableScrollView.translatesAutoresizingMaskIntoConstraints = NO;
    self.mappingTableView.translatesAutoresizingMaskIntoConstraints = NO;
    self.editingView.translatesAutoresizingMaskIntoConstraints = NO;
    self.eventTypeTextField.translatesAutoresizingMaskIntoConstraints = NO;
}

#pragma mark - Custom Setters

-(void) setEvent:(VSC::IM::Event_WPtr)event
{
    _event = event;
    
    [self setupInterfaceForNewEvent];
}

#pragma mark - UI Helpers

-(void) reloadInterface
{
    [self.mappingTableView reloadData];
}

#pragma mark - UI Callbacks

-(IBAction) backToEventChainView:(id)sender
{
    BOOST_ASSERT(self.eventChainController);
    BOOST_ASSERT([self.eventChainController respondsToSelector:@selector(senderRequestsEventChainView:)]);
    
    [self.eventChainController senderRequestsEventChainView:self];
}

#pragma mark - Helpers


-(Action::SPtr) action
{
    Event::SPtr actionEvent = self.event.lock();
    //BOOST_ASSERT(actionEvent);
    Action::SPtr action = boost::dynamic_pointer_cast<Action>(actionEvent);
    //BOOST_ASSERT(action);
    return action;
}

#pragma mark - VSCIMOSXMappingController Methods



-(void) sender:(id)sender requestsEditorForMapping:(Mapping::SPtr)mapping
{
    BOOST_ASSERT(mapping);
    BOOST_ASSERT(sender);
    BOOST_ASSERT([sender isKindOfClass:[VSCIMOSXMappingCellView class]]);
    
    if (mapping && [sender isKindOfClass:[VSCIMOSXMappingCellView class]])
    {
        VSCIMOSXMappingCellView* mappingView = (VSCIMOSXMappingCellView*)sender;
        
        if (self.mappingEditPopover == nil)
        {
            self.mappingEditPopover = [[NSPopover alloc] init];
            self.mappingEditPopover.appearance = NSPopoverAppearanceHUD;
            self.mappingEditPopover.behavior = NSPopoverBehaviorTransient;
        }
        
        if (self.mappingEditViewController == nil)
        {
            self.mappingEditViewController = [[VSCIMOSXMappingEditViewController alloc]
                                                       initWithNibName:@"VSCIMOSXMappingEditViewController"
                                                       bundle:nil];
            BOOST_ASSERT(self.mappingEditViewController);
            BOOST_ASSERT(self.mappingEditViewController.view);
        }
        
        self.mappingEditViewController.mapping = Mapping::WPtr(mapping);
        BOOST_ASSERT(self.mappingEditViewController.offsetKeyedFieldView);
        BOOST_ASSERT(self.mappingEditViewController.scaleFactorKeyedFieldView);
        self.mappingEditPopover.contentViewController = self.mappingEditViewController;
        //self.mappingEditPopover.contentSize = NSMakeSize(213.0, 112.0);
        
        //self.mappingEditPopover.
        
        [self.mappingEditPopover showRelativeToRect:mappingView.editButton.frame
                                                      ofView:mappingView
                                               preferredEdge:NSMinXEdge];
    }
}

-(void) resetEditingView
{
    BOOST_ASSERT(self.editingView);
    [[self.editingView subviews] makeObjectsPerformSelector:@selector(removeFromSuperview)];
    [self.editingView removeConstraints:[self.editingView constraints]];
}


-(void) setupInterfaceForNewEvent
{
    [self resetEditingView];
    
    /*
     *  Setup the view and its subviews according to action type
     */
    
    NSMutableArray* viewsForHorizontalConstraints = [NSMutableArray array];
    NSMutableString* verticalLayoutVisualFormat = [NSMutableString stringWithString:@"V:|"];
    NSMutableDictionary* viewBindingsDictionary = [NSMutableDictionary dictionary];
    
    BOOST_ASSERT(self.editingView);
    NSView* editingView = self.editingView;

    BOOST_ASSERT(self.eventTypeTextField);
    [editingView addSubview:self.eventTypeTextField];
    [viewsForHorizontalConstraints addObject:self.eventTypeTextField];
    [viewBindingsDictionary setValue:self.eventTypeTextField forKey:@"eventTypeTextField"];
    [verticalLayoutVisualFormat appendString:@"-2-[eventTypeTextField]"];

    /*
     *  Handle different types of implementations
     */
    
    Action::SPtr action = [self action];
    Action::Implementation::SPtr implementation = Action::Implementation::SPtr();
    if (action)
    {
        implementation = action->getImplementation();
        [self.eventTypeTextField setStringValue:[NSString stringWithStdString:StringForActionType([self action]->getActionType())]];
    }
    else
    {
        [self.eventTypeTextField setStringValue:@"Delay"];
    }
    
    /*
     *  If the action implementation is a MIDI output owner, then show...
     */
    
    MIDI::OutputOwner::SPtr outputOwner = boost::dynamic_pointer_cast<MIDI::OutputOwner>(implementation);
    if (outputOwner)
    {
        if (self.midiOutputView == nil)
        {
            self.midiOutputView = [[VSCOSXInterfaceFactory defaultFactory] newActionMIDIOutputViewWithOwner:self];
        }
        BOOST_ASSERT(self.midiOutputView);
        [editingView addSubview:self.midiOutputView];
        [viewsForHorizontalConstraints addObject:self.midiOutputView];
        [viewBindingsDictionary setValue:self.midiOutputView forKey:@"midiOutputView"];
        [verticalLayoutVisualFormat appendString:@"-4-[midiOutputView]"];
        [self updateMIDIOutputs];
    }
    
    /*
     *  If the action implementation is a MIDI channel owner, then show...
     */
    
    MIDI::ChannelOwner::SPtr channelOwner = boost::dynamic_pointer_cast<MIDI::ChannelOwner>(implementation);
    if (channelOwner)
    {
        if (self.midiChannelView == nil)
        {
            self.midiChannelView = [[VSCOSXInterfaceFactory defaultFactory] newActionMIDIChannelViewWithOwner:self];
            NSNumberFormatter* formatter = self.midiChannelTextField.formatter;
            [formatter setAllowsFloats:NO];
        }
        BOOST_ASSERT(self.midiChannelView);
        [editingView addSubview:self.midiChannelView];
        [viewsForHorizontalConstraints addObject:self.midiChannelView];
        [viewBindingsDictionary setValue:self.midiChannelView forKey:@"midiChannelView"];
        [verticalLayoutVisualFormat appendString:@"-4-[midiChannelView]"];
        [self updateMIDIChannel];
    }
    
    /*
     *  If the action implementation is a MIDI control number owner, then show...
     */
    
    MIDI::ControlNumberOwner::SPtr controlNumberOwner = boost::dynamic_pointer_cast<MIDI::ControlNumberOwner>(implementation);
    if (controlNumberOwner)
    {
        if (self.midiControlNumberView == nil)
        {
            self.midiControlNumberView = [[VSCOSXInterfaceFactory defaultFactory] newActionMIDIControlNumberViewWithOwner:self];
        }
        BOOST_ASSERT(self.midiControlNumberView);
        [editingView addSubview:self.midiControlNumberView];
        [viewsForHorizontalConstraints addObject:self.midiControlNumberView];
        [viewBindingsDictionary setValue:self.midiControlNumberView forKey:@"midiControlNumberView"];
        [verticalLayoutVisualFormat appendString:@"-4-[midiControlNumberView]"];
        [self updateMIDIControlNumbers];
    }
    
    /*
     *  Add mapping table scroll view (which contains the table view) 
     */
    
    BOOST_ASSERT(self.mappingTableScrollView);
    [editingView addSubview:self.mappingTableScrollView];
    [viewsForHorizontalConstraints addObject:self.mappingTableScrollView];
    [viewBindingsDictionary setValue:self.mappingTableScrollView forKey:@"mappingTableScrollView"];
    [verticalLayoutVisualFormat appendString:@"-4-[mappingTableScrollView(>=150)]"];
    
    /*
     *  Check and update mapping table view
     */
    
    BOOST_ASSERT(self.mappingTableView);
    [self.mappingTableView reloadData];
    
    /*
     *  Bottom constraint
     */
    
    [verticalLayoutVisualFormat appendString:@"-|"];
    
    /*
     *  Apply constraints
     */
    
    [editingView addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:verticalLayoutVisualFormat
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
        
        [editingView addConstraints:contraints];
    }
    
    [[self view] setNeedsLayout:YES];
    [[self view] setNeedsDisplay:YES];
    
    [[[self view] window] recalculateKeyViewLoop];
    
}

-(void) updateMIDIOutputs
{
    BOOST_ASSERT(self.midiOutputPopUpButton);
    [self.midiOutputPopUpButton removeAllItems];
    [self.midiOutputPopUpButton addItemWithTitle:VSCIMOSXNoMidiOutputString];
    
    MIDI::OutputManager::SPtr outputManager = VSC::MIDI::OutputManager::singletonManager();
    BOOST_ASSERT(outputManager);
    if (outputManager)
    {
        const MIDI::Outputs& outputs = outputManager->getOutputs();
        
        BOOST_FOREACH(const MIDI::Output::SPtr& output, outputs)
        {
            NSString* title = [NSString stringWithStdString:output->getDescription()];
            [self.midiOutputPopUpButton addItemWithTitle:title];
        }
        
        MIDI::OutputOwner::SPtr midiOutputOwner = ExtractMIDIOutputOwnerForAction([self action]);
        BOOST_ASSERT(midiOutputOwner);
        if (midiOutputOwner)
        {
            MIDI::Output::SPtr midiOutput = midiOutputOwner->getMIDIOutput();
            if (midiOutput)
            {
                NSString* title = [NSString stringWithStdString:midiOutput->getDescription()];
                [self.midiOutputPopUpButton selectItemWithTitle:title];
                return;
            }
        }
    }
    
    [self.midiOutputPopUpButton selectItemWithTitle:VSCIMOSXNoMidiOutputString];
}

-(void) updateMIDIChannel
{
    MIDI::ChannelOwner::SPtr channelOwner = ExtractMIDIChannelOwnerForAction([self action]);
    BOOST_ASSERT(channelOwner);
    if (channelOwner)
    {
        unsigned int channel = channelOwner->getMIDIChannel();
        [self.midiChannelTextField setIntegerValue:(NSInteger)channel];
    }
    else
    {
        [self.midiChannelTextField setStringValue:@"No channel"];
    }
}

-(void) updateMIDIControlNumbers
{
    Action::SPtr action = [self action];
    BOOST_ASSERT(action);
    
    [self.midiControlNumberPopUpButton removeAllItems];
    
    MIDI::OutputOwner::SPtr midiOutputOwner = ExtractMIDIOutputOwnerForAction([self action]);
    BOOST_ASSERT(midiOutputOwner);
    if (midiOutputOwner)
    {
        MIDI::Output::SPtr midiOutput = midiOutputOwner->getMIDIOutput();
        if (midiOutput)
        {
            const VSC::MIDI::ControlNumbers& controlNumbers = midiOutput->getValidControlNumbers();
            BOOST_FOREACH(const VSC::MIDI::ControlNumber& controlNumber, controlNumbers)
            {
                std::string controlNumberString = MIDI::ControlNumberToString(controlNumber);
                BOOST_ASSERT(!controlNumberString.empty());
                if (!controlNumberString.empty())
                {
                    [self.midiControlNumberPopUpButton addItemWithTitle:[NSString stringWithStdString:controlNumberString]];
                }
            }
        }
    }
    
    MIDI::ControlNumberOwner::SPtr controlNumberOwner =  ExtractMIDIControlNumberOwnerForAction([self action]);
    BOOST_ASSERT(controlNumberOwner);
    if (controlNumberOwner)
    {
        MIDI::ControlNumber controlNumber = controlNumberOwner->getMIDIControlNumber();
        std::string controlNumberString = MIDI::ControlNumberToString(controlNumber);
        [self.midiControlNumberPopUpButton selectItemWithTitle:[NSString stringWithStdString:controlNumberString]];
    }
}

#pragma mark - UI Callbacks


-(IBAction) refreshMIDIOutputs:(id)sender
{
    [self updateMIDIOutputs];
}

-(IBAction) refreshMIDIControlNumbers:(id)sender
{
    [self updateMIDIControlNumbers];
}

-(IBAction) midiOutputSelected:(id)sender
{
    BOOST_ASSERT(self.midiOutputPopUpButton);
    VSC::MIDI::OutputManager::SPtr outputManager = VSC::MIDI::OutputManager::singletonManager();
    BOOST_ASSERT(outputManager);
    
    MIDI::OutputOwner::SPtr outputOwner =  ExtractMIDIOutputOwnerForAction([self action]);
    BOOST_ASSERT(outputOwner);
    
    if (outputOwner && outputManager && self.midiOutputPopUpButton)
    {
        NSString* title = [[self.midiOutputPopUpButton selectedItem] title];
        if (title && [title isEqualToString:VSCIMOSXNoMidiOutputString] == NO)
        {
            MIDI::Output::SPtr midiOutput = outputManager->getOutputWithDescription([title stdString]);
            BOOST_ASSERT(midiOutput);
            outputOwner->setMIDIOutput(midiOutput);
        }
        else
        {
            outputOwner->setMIDIOutput(MIDI::Output::SPtr());
            [self.midiOutputPopUpButton selectItemWithTitle:VSCIMOSXNoMidiOutputString];
        }
    }
    else
    {
        [self.midiOutputPopUpButton selectItemWithTitle:VSCIMOSXNoMidiOutputString];
    }
}

-(IBAction) midiControlNumberSelected:(id)sender
{
    BOOST_ASSERT(self.midiControlNumberPopUpButton);
    
    MIDI::ControlNumberOwner::SPtr controlNumberOwner = ExtractMIDIControlNumberOwnerForAction([self action]);
    BOOST_ASSERT(controlNumberOwner);
    
    if (controlNumberOwner && self.midiControlNumberPopUpButton)
    {
        NSString* title = [[self.midiControlNumberPopUpButton selectedItem] title];
        if (title && [title isEqualToString:VSCIMOSXNoMidiControlNumberString] == NO)
        {
            MIDI::ControlNumber controlNumber = MIDI::StringToControlNumber([title stdString]);
            controlNumberOwner->setMIDIControlNumber(controlNumber);
        }
        else
        {
            controlNumberOwner->setMIDIControlNumber(VSC::MIDI::ControlNone);
            [self.midiControlNumberPopUpButton selectItemWithTitle:VSCIMOSXNoMidiControlNumberString];
        }
    }
    else
    {
        [self.midiControlNumberPopUpButton selectItemWithTitle:VSCIMOSXNoMidiControlNumberString];
    }
}

-(IBAction) midiChannelSelected:(id)sender
{
    MIDI::ChannelOwner::SPtr channelOwner = ExtractMIDIChannelOwnerForAction([self action]);
    BOOST_ASSERT(channelOwner);
    
    if (channelOwner)
    {
        BOOST_ASSERT([sender isKindOfClass:[NSTextField class]]);
        if ([sender isKindOfClass:[NSTextField class]])
        {
            int chan = [(NSTextField*)sender intValue];
            BOOST_ASSERT(chan>=0 && chan < 16);
            if (chan>=0 && chan < 16) channelOwner->setMIDIChannel(chan);
        }
    }
}


#pragma mark - NSTableView Delegate/DataSource

- (NSInteger)numberOfRowsInTableView:(NSTableView *)aTableView
{
    BOOST_ASSERT(aTableView == self.mappingTableView);
    
    if (aTableView == self.mappingTableView)
    {
        Event::SPtr event = self.event.lock();
        BOOST_ASSERT(event);
        int targetCount = (int) event->getRequiredMappingTargets().size();
        int triggerCount = (int) AllowedTriggers().size();
        
        /*
         *  We need one mapping per target/collision combination, and a section view
         *  for each trigger type
         */
        
        return ((targetCount + 1) * triggerCount);
    }
    
	return 0;
}

- (NSView *)tableView:(NSTableView *)tableView viewForTableColumn:(NSTableColumn*)tableColumn row:(NSInteger)row
{
    BOOST_ASSERT(tableView == self.mappingTableView);
    
    if (tableView == self.mappingTableView)
    {
        Event::SPtr event = self.event.lock();
        BOOST_ASSERT(event);
        
        if (event)
        {
            const Targets& targets = event->getRequiredMappingTargets();
            int targetIndex = row % ((int)targets.size() + 1);
            
            const Triggers& triggers = AllowedTriggers();
            int triggerIndex = std::floor(row / (targets.size() + 1));
            Trigger trigger = triggers.at(triggerIndex);
            
            if (targetIndex == 0)
            {
                VSCOSXTableSectionView* sectionView = [tableView makeViewWithIdentifier:@"VSCOSXTableSectionView" owner:self];
                BOOST_ASSERT(sectionView);
                
                if (sectionView)
                {
                    [sectionView.nameTextField setStringValue:[NSString stringWithStdString:StringForTrigger(trigger)]];
                    return sectionView;
                }
            }
            
            // if it's not 0 then it should be positive, otherwise modulo function is pretty bad
            
            BOOST_ASSERT(targetIndex > 0);
            
            if (targetIndex > 0)
            {
                Target target = targets.at(targetIndex-1);
                
                Mapping::SPtr mapping = event->getMapping(trigger, target);
                BOOST_ASSERT(mapping);
                
                VSCIMOSXMappingCellView* mappingView = [tableView makeViewWithIdentifier:@"VSCIMOSXMappingCellView" owner:self];
                BOOST_ASSERT(mappingView);
                BOOST_ASSERT([mappingView isKindOfClass:[VSCIMOSXMappingCellView class]]);
                
                if (mappingView)
                {
                    [mappingView setAllowedMappingTypes:AllowedMappingTypeSetForTrigger(trigger)];
                    [mappingView setController:(id)self];
                    [mappingView setMapping:(Mapping::WPtr(mapping))];
                    [mappingView.targetTextField setStringValue:[NSString stringWithStdString:StringForTarget(target)]];
                    return mappingView;
                }
            }
        }
    }
    
	return nil;
}

- (CGFloat)tableView:(NSTableView *)tableView heightOfRow:(NSInteger)row
{
    BOOST_ASSERT(tableView == self.mappingTableView);
    
    if (tableView == self.mappingTableView)
    {
        Event::SPtr event = self.event.lock();
        BOOST_ASSERT(event);
        
        if (event)
        {
            const Targets& targets = event->getRequiredMappingTargets();
            int targetIndex = row % ((int)targets.size() + 1);
            if (targetIndex == 0)
            {
                return [VSCOSXTableSectionView defaultViewHeight];
            }
            else
            {
                return [VSCIMOSXMappingCellView defaultViewHeight];
            }
        
        }
    }
    
    BOOST_ASSERT_MSG(false, "Shouldn't reach this point");
    
    return 0;
}

- (BOOL)tableView:(NSTableView *)aTableView shouldSelectRow:(NSInteger)rowIndex
{
    NSLog(@"%@ tableView: %@ shouldSelectRow: %ld", self, aTableView, (long)rowIndex);
    return YES;
}

@end
