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

#include "VSCIMAction.h"

using namespace VSC;
using namespace VSC::IM;

NSString* const VSCIMOSXMappingCellViewReuseIdentifier = @"VSCIMOSXMappingCellViewReuseIdentifier";

@interface VSCIMOSXEventEditorViewController ()

@property (weak) IBOutlet NSButton* backToEventChainViewButton;

-(IBAction) backToEventChainView:(id)sender;
-(BOOL) checkMappingView:(id<VSCIMOSXMappingCellView>)v;

+(VSCIMOSXMappingCellView*) newMappingViewWithOwner:(id)owner;

/*
 *  Action specific stuff
 */

@property (nonatomic, strong) IBOutlet NSView* mainView;
@property (nonatomic, strong) IBOutlet NSTextField* actionTypeTextField;
@property (nonatomic, strong) IBOutlet NSButton* mappingsButton;

@property (nonatomic, strong) IBOutlet NSView* midiOutputView;
@property (nonatomic, strong) IBOutlet NSPopUpButton* midiOutputPopUpButton;

@property (nonatomic, strong) IBOutlet NSView* midiChannelView;
@property (nonatomic, strong) IBOutlet NSTextField* midiChannelTextField;

@property (nonatomic, strong) IBOutlet NSView* midiControlNumberView;
@property (nonatomic, strong) IBOutlet NSPopUpButton* midiControlNumberPopUpButton;

-(IBAction) midiOutputSelected:(id)sender;
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
}

#pragma mark - View Factory Methods

+(VSCIMOSXMappingCellView*) newMappingViewWithOwner:(id)owner
{
    static NSNib* nib = nil;
    static NSString* identifier = [[VSCIMOSXMappingCellView class] description];
    
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        BOOST_ASSERT(!nib);
        nib = [[NSNib alloc] initWithNibNamed:[[VSCIMOSXMappingCellView class] description] bundle:nil];
    });
    BOOST_ASSERT(nib);
    
    NSArray *objects = nil;
    VSCIMOSXMappingCellView* v = nil;
    [nib instantiateNibWithOwner:owner topLevelObjects:&objects];
    for(id object in objects)
    {
        if([object isKindOfClass:[VSCIMOSXMappingCellView class]])
        {
            v = object;
            v.identifier = identifier;
            break;
        }
    }
    BOOST_ASSERT(v);
    return v;
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
    BOOST_ASSERT([self.eventChainController respondsToSelector:@selector(senderRequestsEventCollisionChainView:)]);
    
    [self.eventChainController senderRequestsEventCollisionChainView:self];
}

#pragma mark - Helpers

-(BOOL) checkMappingView:(id<VSCIMOSXMappingCellView>)v
{
    BOOST_ASSERT([v respondsToSelector:@selector(target)]);
    BOOST_ASSERT([v respondsToSelector:@selector(mapping)]);
    BOOST_ASSERT([v respondsToSelector:@selector(setMapping:)]);
    
    if (![v respondsToSelector:@selector(target)]) return NO;
    if (![v respondsToSelector:@selector(mapping)]) return NO;
    if (![v respondsToSelector:@selector(setMapping:)]) return NO;
    
    return YES;
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
            BOOST_ASSERT(self.mappingEditViewController.offsetTextField);
            BOOST_ASSERT(self.mappingEditViewController.scaleFactorTextField);
        }
        
        self.mappingEditViewController.mapping = Mapping::WPtr(mapping);
        self.mappingEditPopover.contentViewController = self.mappingEditViewController;
        self.mappingEditPopover.contentSize = NSMakeSize(213.0, 112.0);
        
        [self.mappingEditPopover showRelativeToRect:mappingView.editButton.frame
                                                      ofView:mappingView
                                               preferredEdge:NSMinXEdge];
        
        /*
        NSLog(@"view.frame %@, offsetTextField.frame %@, scaleFactorTextField.frame %@",
              NSStringFromRect(self.mappingEditViewController.view.frame),
              NSStringFromRect(self.mappingEditViewController.offsetTextField.frame),
              NSStringFromRect(self.mappingEditViewController.scaleFactorTextField.frame));
         */
    }
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
    
    /*
     *  Update type interface
     */
    
    [self.actionTypeTextField setStringValue:[NSString stringWithStdString:StringForActionType([self action]->getActionType())]];
    
    /*
     *  Handle different types of implementations
     */
    
    Action::Implementation::SPtr implementation = action->getImplementation();
    
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
        [self addSubview:self.midiOutputView];
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
        }
        BOOST_ASSERT(self.midiChannelView);
        [self addSubview:self.midiChannelView];
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
        [self addSubview:self.midiControlNumberView];
        [viewsForHorizontalConstraints addObject:self.midiControlNumberView];
        [viewBindingsDictionary setValue:self.midiControlNumberView forKey:@"midiControlNumberView"];
        [verticalLayoutVisualFormat appendString:@"-4-[midiControlNumberView]"];
        [self updateMIDIControlNumbers];
    }
    
    /*
     *  Bottom constraint
     */
    
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
        
        MIDI::Output::SPtr midiOutput = [[self class] extractMIDIOutputForAction:[self action]];
        if (midiOutput)
        {
            NSString* title = [NSString stringWithStdString:midiOutput->getDescription()];
            [self.midiOutputPopUpButton selectItemWithTitle:title];
            return;
        }
    }
    
    [self.midiOutputPopUpButton selectItemWithTitle:VSCIMOSXNoMidiOutputString];
}

-(void) updateMIDIChannel
{
    Action::SPtr action = [self action];
    unsigned int channel = [[self class] extractMIDIChannelForAction:action];
    
    [self.midiChannelTextField setIntegerValue:(NSInteger)channel];
    
    //[self.midiChannelTextField setStringValue:@"No channel"];
    
}

-(void) updateMIDIControlNumbers
{
    Action::SPtr action = [self action];
    BOOST_ASSERT(action);
    
    MIDI::Output::SPtr midiOutput = [[self class] extractMIDIOutputForAction:action];
    
    [self.midiControlNumberPopUpButton removeAllItems];
    
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
    
    MIDI::ControlNumber controlNumber = [[self class] extractMIDIControlNumberForAction:action];
    std::string controlNumberString = MIDI::ControlNumberToString(controlNumber);
    [self.midiControlNumberPopUpButton selectItemWithTitle:[NSString stringWithStdString:controlNumberString]];
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
        
        return (targetCount * triggerCount) + triggerCount;
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
            
            if (targetIndex == 0)
            {
                // make and reuturn section view
            }
            
            // if it's not 0 then it should be positive, otherwise modulo function is pretty bad
            
            BOOST_ASSERT(targetIndex > 0);
            
            if (targetIndex > 0)
            {
                const Triggers& triggers = AllowedTriggers();
                
                int triggerIndex = std::floor(row / (targets.size() + 1));
                
                Target target = targets.at(targetIndex-1);
                Trigger trigger = triggers.at(triggerIndex);
                
                Mapping::SPtr mapping = event->getMapping(trigger, target);
                
                VSCIMOSXMappingCellView* mappingView = [tableView makeViewWithIdentifier:[[VSCIMOSXMappingCellView class] description]
                                                                               owner:self];
                
                if (!mappingView)
                {
                    mappingView = [[self class] newMappingViewWithOwner:self];
                }
                
                BOOST_ASSERT(mappingView);
                BOOST_ASSERT([mappingView isKindOfClass:[VSCIMOSXMappingCellView class]]);
                
                [mappingView setController:(id)self];
                [mappingView setMapping:(Mapping::WPtr(mapping))];
                [mappingView setTarget:target];
                
                return mappingView;
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
                
            }
            else
            {
                return [VSCIMOSXMappingCellView defaultHeight];
            }
        
        }
    }
    
    BOOST_ASSERT_MSG(false, "Shouldn't reach this point");
    
    return 0;
}

@end
