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

NSString* const VSCIMOSXMappingViewReuseIdentifier = @"VSCIMOSXMappingViewReuseIdentifier";

@interface VSCIMOSXEventEditorViewController ()

@property (weak) IBOutlet NSButton* backToEventChainViewButton;
@property (strong) VSCIMOSXMappingEditViewController* mappingEditViewController;
@property (strong) NSPopover* mappingEditPopover;

-(IBAction) backToEventChainView:(id)sender;
-(BOOL) checkMappingView:(id<VSCIMOSXMappingView>)v;

+(VSCIMOSXMappingView*) newMappingViewWithOwner:(id)owner;

@end


@implementation VSCIMOSXEventEditorViewController

@synthesize action = _action;

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

-(VSC::IM::Mapping::SPtr) sender:(id)sender
                  requestsMappingWithType:(VSC::IM::MappingType)mappingType
                      forTarget:(VSC::IM::Target)target;
{
    
    VSC::IM::Action::SPtr collisionAction = [self action].lock();
    
    BOOST_ASSERT(target != VSC::IM::TargetNone);
    if (target == VSC::IM::TargetNone) return VSC::IM::Mapping::SPtr();
    
    VSC::IM::Mapping::SPtr currentMapping = collisionAction->getMappingForTarget(target);
    VSC::IM::MappingType currentMappingType = VSC::IM::MappingTypeForMapping(currentMapping);
    if (currentMappingType == mappingType) return currentMapping;
    
    VSC::IM::Mapping::SPtr newMapping = VSC::IM::createMappingWithType(mappingType);
    BOOST_ASSERT(newMapping);
    collisionAction->setMappingForTarget(target, newMapping);
    
    VSC::IM::CollisionVelocityMapping::SPtr velMapping = boost::dynamic_pointer_cast<VSC::IM::CollisionVelocityMapping>(newMapping);
    if (velMapping)
    {
        velMapping->setOffset(0.0);
        velMapping->setScaleFactor(3.0);
    }
    
    return newMapping;
}

-(void) sender:(id)sender requestsEditorForMapping:(VSC::IM::Mapping::SPtr)mapping
{
    BOOST_ASSERT(mapping);
    BOOST_ASSERT(sender);
    BOOST_ASSERT([sender isKindOfClass:[VSCIMOSXMappingView class]]);
    
    if (mapping && [sender isKindOfClass:[VSCIMOSXMappingView class]])
    {
        VSCIMOSXMappingView* mappingView = (VSCIMOSXMappingView*)sender;
        
        if (self.collisionMappingEditPopover == nil)
        {
            self.collisionMappingEditPopover = [[NSPopover alloc] init];
            self.collisionMappingEditPopover.appearance = NSPopoverAppearanceHUD;
            self.collisionMappingEditPopover.behavior = NSPopoverBehaviorTransient;
        }
        
        if (self.collisionMappingEditViewController == nil)
        {
            self.collisionMappingEditViewController = [[VSCIMOSXMappingEditViewController alloc]
                                                       initWithNibName:@"VSCIMOSXMappingEditViewController"
                                                       bundle:nil];
            
            BOOST_ASSERT(self.collisionMappingEditViewController);
            
            BOOST_ASSERT(self.collisionMappingEditViewController.view);
            BOOST_ASSERT(self.collisionMappingEditViewController.offsetTextField);
            BOOST_ASSERT(self.collisionMappingEditViewController.scaleFactorTextField);
        }
        
        self.collisionMappingEditViewController.collisionMapping = VSC::IM::Mapping::WPtr(mapping);
        
        self.collisionMappingEditPopover.contentViewController = self.collisionMappingEditViewController;
        self.collisionMappingEditPopover.contentSize = NSMakeSize(213.0, 112.0);
        
        [self.collisionMappingEditPopover showRelativeToRect:mappingView.editButton.frame
                                                      ofView:mappingView
                                               preferredEdge:NSMinXEdge];
        
        /*
        NSLog(@"view.frame %@, offsetTextField.frame %@, scaleFactorTextField.frame %@",
              NSStringFromRect(self.collisionMappingEditViewController.view.frame),
              NSStringFromRect(self.collisionMappingEditViewController.offsetTextField.frame),
              NSStringFromRect(self.collisionMappingEditViewController.scaleFactorTextField.frame));
         */
    }
}


#pragma mark - PXListViewDelegate

- (NSInteger)numberOfRowsInTableView:(NSTableView *)aTableView
{
    BOOST_ASSERT(aTableView == self.mappingTableView);
    
    if (aTableView == self.mappingTableView)
    {
        VSC::IM::Action::SPtr collisionAction = self.action.lock();
        BOOST_ASSERT(collisionAction);
        if (collisionAction) return collisionAction->getExpectedMappingTargets().size();
    }
    
	return 0;
}

- (NSView *)tableView:(NSTableView *)tableView viewForTableColumn:(NSTableColumn*)tableColumn row:(NSInteger)row
{
    BOOST_ASSERT(tableView == self.mappingTableView);
    
    if (tableView == self.mappingTableView)
    {
        VSC::IM::Action::SPtr collisionAction = self.action.lock();
        BOOST_ASSERT(collisionAction);
        
        if (collisionAction)
        {
            const VSC::IM::Targets& targets = collisionAction->getExpectedMappingTargets();
            BOOST_ASSERT(targets.size() > row);
            if (targets.size() <= row) return nil;
            VSC::IM::Target target = targets.at(row);
            VSC::IM::Mapping::SPtr collisionMapping = collisionAction->getMappingForTarget(target);
            
            if (collisionMapping)
            {
                VSCIMOSXMappingView* mappingView = [tableView makeViewWithIdentifier:[[VSCIMOSXMappingView class] description]
                                                                                        owner:self];
                
                
                
                BOOST_ASSERT(mappingView);
                
                [mappingView setController:(id)self];
                
                if (mappingView) BOOST_ASSERT([mappingView isKindOfClass:[VSCIMOSXMappingView class]]);
                else mappingView = [[self class] newMappingViewWithOwner:self];
                
                [mappingView setMapping:(VSC::IM::Mapping::WPtr(collisionMapping))];
                
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
        return [VSCIMOSXMappingView defaultHeight];
    }
    
    BOOST_ASSERT_MSG(false, "Shouldn't reach this point");
    
    return 0;
}

@end
