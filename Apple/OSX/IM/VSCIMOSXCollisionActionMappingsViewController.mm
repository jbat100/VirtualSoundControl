//
//  VSCIMOSXCollisionActionMappingsViewController.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 11/27/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCIMOSXCollisionActionMappingsViewController.h"
#import "VSCIMOSXCollisionMappingEditViewController.h"
#import "VSCIMOSXCollisionEventChainController.h"
#import "VSCIMOSXCollisionMappingView.h"

#include "VSCIMCollisionAction.h"

NSString* const VSCIMOSXCollisionMappingViewReuseIdentifier = @"VSCIMOSXCollisionMappingViewReuseIdentifier";

@interface VSCIMOSXCollisionActionMappingsViewController ()

@property (weak) IBOutlet NSButton* backToEventChainViewButton;
@property (strong) VSCIMOSXCollisionMappingEditViewController* mappingEditViewController;
@property (strong) NSPopover* mappingEditPopover;

-(IBAction) backToEventChainView:(id)sender;
-(BOOL) checkMappingView:(id<VSCIMOSXCollisionMappingView>)v;

+(VSCIMOSXCollisionMappingView*) newCollisionMappingViewWithOwner:(id)owner;

@end


@implementation VSCIMOSXCollisionActionMappingsViewController

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

+(VSCIMOSXCollisionMappingView*) newCollisionMappingViewWithOwner:(id)owner
{
    static NSNib* nib = nil;
    static NSString* identifier = [[VSCIMOSXCollisionMappingView class] description];
    
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        BOOST_ASSERT(!nib);
        nib = [[NSNib alloc] initWithNibNamed:[[VSCIMOSXCollisionMappingView class] description] bundle:nil];
    });
    BOOST_ASSERT(nib);
    
    NSArray *objects = nil;
    VSCIMOSXCollisionMappingView* v = nil;
    [nib instantiateNibWithOwner:owner topLevelObjects:&objects];
    for(id object in objects)
    {
        if([object isKindOfClass:[VSCIMOSXCollisionMappingView class]])
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

-(BOOL) checkMappingView:(id<VSCIMOSXCollisionMappingView>)v
{
    BOOST_ASSERT([v respondsToSelector:@selector(target)]);
    BOOST_ASSERT([v respondsToSelector:@selector(mapping)]);
    BOOST_ASSERT([v respondsToSelector:@selector(setMapping:)]);
    
    if (![v respondsToSelector:@selector(target)]) return NO;
    if (![v respondsToSelector:@selector(mapping)]) return NO;
    if (![v respondsToSelector:@selector(setMapping:)]) return NO;
    
    return YES;
}

#pragma mark - VSCIMOSXCollisionMappingController Methods

-(VSC::IM::CollisionMapping::SPtr) sender:(id)sender
                  requestsMappingWithType:(VSCIMOSXCollisionMappingType)mappingType
                      forTarget:(VSC::IM::Target)target;
{
    
    VSC::IM::CollisionAction::SPtr collisionAction = [self action].lock();
    
    BOOST_ASSERT(target != VSC::IM::TargetNone);
    if (target == VSC::IM::TargetNone) return VSC::IM::CollisionMapping::SPtr();
    
    VSC::IM::CollisionMapping::SPtr currentMapping = collisionAction->getMappingForTarget(target);
    VSCIMOSXCollisionMappingType currentMappingType = VSCIMOSXCollisionMappingTypeForCollisionMapping(currentMapping);
    if (currentMappingType == mappingType) return currentMapping;
    
    VSC::IM::CollisionMapping::SPtr newMapping = VSCIMOSXCreateCollisionMappingWithType(mappingType);
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

-(void) sender:(id)sender requestsEditorForMapping:(VSC::IM::CollisionMapping::SPtr)mapping
{
    BOOST_ASSERT(mapping);
    BOOST_ASSERT(sender);
    BOOST_ASSERT([sender isKindOfClass:[VSCIMOSXCollisionMappingView class]]);
    
    if (mapping && [sender isKindOfClass:[VSCIMOSXCollisionMappingView class]])
    {
        VSCIMOSXCollisionMappingView* mappingView = (VSCIMOSXCollisionMappingView*)sender;
        
        if (self.collisionMappingEditPopover == nil)
        {
            self.collisionMappingEditPopover = [[NSPopover alloc] init];
            self.collisionMappingEditPopover.appearance = NSPopoverAppearanceHUD;
            self.collisionMappingEditPopover.behavior = NSPopoverBehaviorTransient;
        }
        
        if (self.collisionMappingEditViewController == nil)
        {
            self.collisionMappingEditViewController = [[VSCIMOSXCollisionMappingEditViewController alloc]
                                                       initWithNibName:@"VSCIMOSXCollisionMappingEditViewController"
                                                       bundle:nil];
            
            BOOST_ASSERT(self.collisionMappingEditViewController);
            
            BOOST_ASSERT(self.collisionMappingEditViewController.view);
            BOOST_ASSERT(self.collisionMappingEditViewController.offsetTextField);
            BOOST_ASSERT(self.collisionMappingEditViewController.scaleFactorTextField);
        }
        
        self.collisionMappingEditViewController.collisionMapping = VSC::IM::CollisionMapping::WPtr(mapping);
        
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
        VSC::IM::CollisionAction::SPtr collisionAction = self.action.lock();
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
        VSC::IM::CollisionAction::SPtr collisionAction = self.action.lock();
        BOOST_ASSERT(collisionAction);
        
        if (collisionAction)
        {
            const VSC::IM::Targets& targets = collisionAction->getExpectedMappingTargets();
            BOOST_ASSERT(targets.size() > row);
            if (targets.size() <= row) return nil;
            VSC::IM::Target target = targets.at(row);
            VSC::IM::CollisionMapping::SPtr collisionMapping = collisionAction->getMappingForTarget(target);
            
            if (collisionMapping)
            {
                VSCIMOSXCollisionMappingView* mappingView = [tableView makeViewWithIdentifier:[[VSCIMOSXCollisionMappingView class] description]
                                                                                        owner:self];
                
                
                
                BOOST_ASSERT(mappingView);
                
                [mappingView setController:(id)self];
                
                if (mappingView) BOOST_ASSERT([mappingView isKindOfClass:[VSCIMOSXCollisionMappingView class]]);
                else mappingView = [[self class] newCollisionMappingViewWithOwner:self];
                
                [mappingView setMapping:(VSC::IM::CollisionMapping::WPtr(collisionMapping))];
                
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
        return [VSCIMOSXCollisionMappingView defaultHeight];
    }
    
    BOOST_ASSERT_MSG(false, "Shouldn't reach this point");
    
    return 0;
}

@end
