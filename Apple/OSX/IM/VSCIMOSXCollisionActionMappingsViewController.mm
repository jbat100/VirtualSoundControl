//
//  VSCIMOSXCollisionActionMappingsViewController.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 11/27/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCIMOSXCollisionActionMappingsViewController.h"
#import "VSCIMOSXCollisionEventChainController.h"
#import "VSCIMOSXCollisionMappingView.h"
#import "PXListView.h"

NSString* const VSCIMOSXCollisionMappingViewReuseIdentifier = @"VSCIMOSXCollisionMappingViewReuseIdentifier";

@interface VSCIMOSXCollisionActionMappingsViewController ()

@property (weak) IBOutlet NSButton* backToEventChainViewButton;
@property (strong) VSCIMOSXCollisionMappingEditViewController* mappingEditViewController;
@property (strong) NSPopover* mappingEditPopover;

-(IBAction) backToEventChainView:(id)sender;
-(BOOL) checkMappingView:(id<VSCIMOSXCollisionMappingView>)v;

@end


@implementation VSCIMOSXCollisionActionMappingsViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Initialization code here.
    }
    
    return self;
}

#pragma mark - View Factory Methods

+(VSCIMOSXCollisionMappingView*) newCollisionMappingView
{
    VSCIMOSXCollisionMappingView* cell = [VSCIMOSXCollisionMappingView cellLoadedFromNibNamed:@"VSCIMOSXCollisionMappingView"
                                                                                       bundle:nil
                                                                           reusableIdentifier:VSCIMOSXCollisionMappingViewReuseIdentifier];
    
    BOOST_ASSERT(cell);
    
    return cell;
}

#pragma mark - UI Callbacks

-(IBAction) backToEventChainView:(id)sender
{
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
    
    VSC::IM::CollisionAction::SPtr collisionAction = self.action.lock();
    
    BOOST_ASSERT(target != VSC::IM::TargetNone);
    if (target == VSC::IM::TargetNone) return VSC::IM::CollisionMapping::SPtr();
    
    VSC::IM::CollisionMapping::SPtr currentMapping = collisionAction->getMappingForTarget(target);
    VSCIMOSXCollisionMappingType currentMappingType = VSCIMOSXCollisionMappingTypeForCollisionMapping(currentMapping);
    if (currentMappingType == mappingType) return currentMapping;
    
    VSC::IM::CollisionMapping::SPtr newMapping = VSCIMOSXCreateCollisionMappingWithType(mappingType);
    BOOST_ASSERT(newMapping);
    collisionAction->setMappingForTarget(target, newMapping);
    
    return newMapping;
}

-(void) collisionMappingViewRequestsEditor:(id<VSCIMOSXCollisionMappingView>)view
{
    
}


#pragma mark - PXListViewDelegate

- (NSUInteger)numberOfRowsInListView:(PXListView*)aListView
{
    BOOST_ASSERT(aListView == self.listView);
    
    if (aListView == self.listView)
    {
        VSC::IM::CollisionAction::SPtr collisionAction = self.action.lock();
        BOOST_ASSERT(collisionAction);
        if (collisionAction) return collisionAction->getExpectedMappingTargets().size();
    }
    
	return 0;
}

- (PXListViewCell*)listView:(PXListView*)aListView cellForRow:(NSUInteger)row
{

    BOOST_ASSERT(aListView == self.listView);
    
    if (aListView == self.listView)
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
                PXListViewCell* cell = [aListView dequeueCellWithReusableIdentifier:VSCIMOSXCollisionMappingViewReuseIdentifier];
                VSCIMOSXCollisionMappingView* mappingView = nil;
                
                if (cell)
                {
                    BOOST_ASSERT([cell isKindOfClass:[VSCIMOSXCollisionMappingView class]]);
                    mappingView = (VSCIMOSXCollisionMappingView*)cell;
                }
                
                if(!mappingView)
                {
                    mappingView = [[self class] newCollisionMappingView];
                }
                [mappingView setMapping:(VSC::IM::CollisionMapping::WPtr(collisionMapping))];
                
                return mappingView;
            }
        }
    }
	
	return nil;
}

- (CGFloat)listView:(PXListView*)aListView heightOfRow:(NSUInteger)row
{
    
    BOOST_ASSERT(aListView == self.listView);
    
    if (aListView == self.listView)
    {
        VSC::IM::CollisionAction::SPtr collisionAction = self.action.lock();
        BOOST_ASSERT(collisionAction);
        
        if (collisionAction)
        {
            const VSC::IM::Targets& targets = collisionAction->getExpectedMappingTargets();
            BOOST_ASSERT(targets.size() > row);
            if (targets.size() <= row) return 0;
            VSC::IM::Target target = targets.at(row);
            VSC::IM::CollisionMapping::SPtr collisionMapping = collisionAction->getMappingForTarget(target);
            return [VSCIMOSXCollisionMappingView heightOfViewForCollisionMapping:collisionMapping];
        }
    }
    
    BOOST_ASSERT_MSG(false, "Shouldn't reach this point");
    
    return 0;
}

- (void)listViewSelectionDidChange:(NSNotification*)aNotification
{
    NSLog(@"%@ selection changed: %@", self, aNotification);
}



@end
