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
#import "VSCIMOSXMappingView.h"

#include "VSCIMAction.h"

using namespace VSC;
using namespace VSC::IM;

NSString* const VSCIMOSXMappingViewReuseIdentifier = @"VSCIMOSXMappingViewReuseIdentifier";

@interface VSCIMOSXEventEditorViewController ()

@property (weak) IBOutlet NSButton* backToEventChainViewButton;

-(IBAction) backToEventChainView:(id)sender;
-(BOOL) checkMappingView:(id<VSCIMOSXMappingView>)v;

+(VSCIMOSXMappingView*) newMappingViewWithOwner:(id)owner;

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

+(VSCIMOSXMappingView*) newMappingViewWithOwner:(id)owner
{
    static NSNib* nib = nil;
    static NSString* identifier = [[VSCIMOSXMappingView class] description];
    
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        BOOST_ASSERT(!nib);
        nib = [[NSNib alloc] initWithNibNamed:[[VSCIMOSXMappingView class] description] bundle:nil];
    });
    BOOST_ASSERT(nib);
    
    NSArray *objects = nil;
    VSCIMOSXMappingView* v = nil;
    [nib instantiateNibWithOwner:owner topLevelObjects:&objects];
    for(id object in objects)
    {
        if([object isKindOfClass:[VSCIMOSXMappingView class]])
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

-(BOOL) checkMappingView:(id<VSCIMOSXMappingView>)v
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
    BOOST_ASSERT([sender isKindOfClass:[VSCIMOSXMappingView class]]);
    
    if (mapping && [sender isKindOfClass:[VSCIMOSXMappingView class]])
    {
        VSCIMOSXMappingView* mappingView = (VSCIMOSXMappingView*)sender;
        
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


#pragma mark - PXListViewDelegate

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
                
                VSCIMOSXMappingView* mappingView = [tableView makeViewWithIdentifier:[[VSCIMOSXMappingView class] description]
                                                                               owner:self];
                
                if (!mappingView)
                {
                    mappingView = [[self class] newMappingViewWithOwner:self];
                }
                
                BOOST_ASSERT(mappingView);
                BOOST_ASSERT([mappingView isKindOfClass:[VSCIMOSXMappingView class]]);
                
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
                return [VSCIMOSXMappingView defaultHeight];
            }
        
        }
    }
    
    BOOST_ASSERT_MSG(false, "Shouldn't reach this point");
    
    return 0;
}

@end
