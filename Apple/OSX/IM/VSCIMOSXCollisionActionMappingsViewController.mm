//
//  VSCIMOSXCollisionActionMappingsViewController.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 11/27/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCIMOSXCollisionActionMappingsViewController.h"

#import "VSCIMOSXCollisionEventChainController.h"

NSString* const VSCIMOSXCollisionMappingViewReuseIdentifier = @"VSCIMOSXCollisionMappingViewReuseIdentifier";

@interface VSCIMOSXCollisionActionMappingsViewController ()

@property (weak) IBOutlet NSButton* backToEventChainViewButton;
@property (strong) VSCIMOSXCollisionMappingEditViewController* mappingEditViewController;
@property (strong) NSPopover

-(IBAction) backToEventChainView:(id)sender;
-(BOOL) checkMappingEditor:(id<VSCIMOSXCollisionMappingEditor>)editor;

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
    [self.eventChainController senderRequestsEventChainView:self];
}

#pragma mark - Helpers

-(BOOL) checkMappingEditor:(id<VSCIMOSXCollisionMappingEditor>)editor
{
    BOOST_ASSERT([editor respondsToSelector:@selector(target)]);
    BOOST_ASSERT([editor respondsToSelector:@selector(mapping)]);
    BOOST_ASSERT([editor respondsToSelector:@selector(setMapping:)]);
    
    if (![editor respondsToSelector:@selector(target)]) return NO;
    if (![editor respondsToSelector:@selector(mapping)]) return NO;
    if (![editor respondsToSelector:@selector(setMapping:)]) return NO;
    
    return YES;
}

#pragma mark - VSCIMOSXCollisionMappingController Methods

-(VSC::IM::CollisionMapping::SPtr) collisionMappingEditor:(id<VSCIMOSXCollisionMappingEditor>)editor
requestsMappingWithType:(VSCIMOSXCollisionMappingType)mappingType
{
    
    if ([self checkMappingEditor:editor])
    {
        VSC::IM::CollisionAction::SPtr collisionAction = self.action.lock();
        
        BOOST_ASSERT(editor.target != VSC::IM::TargetNone);
        if (editor.target == VSC::IM::TargetNone) return VSC::IM::CollisionMapping::SPtr();
        
        VSC::IM::CollisionMapping::SPtr currentMapping = collisionAction->getMappingForTarget(editor.target);
        VSCIMOSXCollisionMappingType currentMappingType = VSCIMOSXCollisionMappingTypeForCollisionMapping(currentMapping);
        if (currentMappingType == mappingType) return currentMapping;
        
        VSC::IM::CollisionMapping::SPtr newMapping = VSCIMOSXCreateCollisionMappingWithType(mappingType);
        collisionAction->setMappingForTarget(editor.target, newMapping);
        
        return newMapping;
    }
    
    return VSC::IM::CollisionMapping::SPtr();
    
}

-(void) collisionMappingViewRequestsEditor:(id<VSCIMOSXCollisionMappingView>)view
{
    
}


#pragma mark - PXListViewDelegate

- (NSUInteger)numberOfRowsInListView:(PXListView*)aListView
{
    if ([[aListView superview] isKindOfClass:[VSCIMOSXCollisionEventChainView class]])
    {
        VSCIMOSXCollisionEventChainView* eventChainView = (VSCIMOSXCollisionEventChainView*)[aListView superview];
        VSC::IM::CollisionEventChain::SPtr eventChain = [eventChainView eventChain].lock();
        BOOST_ASSERT(eventChain);
        if (eventChain) eventChain->getNumberOfEvents();
    }
    
    else if ([aListView isKindOfClass:[VSCIMOSXCollisionMappingListView class]])
    {
        VSCIMOSXCollisionMappingListView* actionMappingsViewController = (VSCIMOSXCollisionMappingListView*)[aListView superview];
        VSC::IM::CollisionAction::SPtr collisionAction = [mappingListView action].lock();
        BOOST_ASSERT(collisionAction);
        if (collisionAction) collisionAction->getExpectedMappingTargets().size();
    }
    
	return 0;
}

- (PXListViewCell*)listView:(PXListView*)aListView cellForRow:(NSUInteger)row
{
    if ([[aListView superview] isKindOfClass:[VSCIMOSXCollisionEventChainView class]])
    {
        VSCIMOSXCollisionEventChainView* eventChainView = (VSCIMOSXCollisionEventChainView*)[aListView superview];
        VSC::IM::CollisionEventChain::SPtr eventChain = [eventChainView eventChain].lock();
        BOOST_ASSERT(eventChain);
        VSC::IM::Event::SPtr event = VSC::IM::Event::SPtr();
        if (eventChain) event = eventChain->getEventAtIndex((unsigned int)row);
        
        VSC::IM::CollisionAction::SPtr action = boost::dynamic_pointer_cast<VSC::IM::CollisionAction>(event);
        if (action)
        {
            PXListViewCell* cell = [aListView dequeueCellWithReusableIdentifier:VSCIMOSXCollisionActionViewReuseIdentifier];
            VSCIMOSXCollisionActionView* actionView = nil;
            if (cell)
            {
                BOOST_ASSERT([cell isKindOfClass:[VSCIMOSXCollisionActionView class]]);
                actionView = (VSCIMOSXCollisionActionView*)cell;
            }
            if(!actionView)
            {
                actionView = [self newCollisionActionView];
            }
            [actionView setCollisionAction:(VSC::IM::CollisionAction::WPtr(action))];
            actionView.eventChainController = eventChainView;
            return actionView;
        }
        
        VSC::IM::Delay::SPtr delay = boost::dynamic_pointer_cast<VSC::IM::Delay>(event);
        if (delay)
        {
            PXListViewCell* cell = [aListView dequeueCellWithReusableIdentifier:VSCIMOSXDelayViewReuseIdentifier];
            VSCIMOSXDelayView* delayView = nil;
            if (cell)
            {
                BOOST_ASSERT([cell isKindOfClass:[VSCIMOSXDelayView class]]);
                delayView = (VSCIMOSXDelayView*)cell;
            }
            if(!delayView)
            {
                delayView = [self newDelayView];
            }
            [delayView setDelay:(VSC::IM::Delay::WPtr(delay))];
            return delayView;
        }
        
    }
    
    else if ([aListView isKindOfClass:[VSCIMOSXCollisionMappingListView class]])
    {
        VSCIMOSXCollisionMappingListView* mappingListView = (VSCIMOSXCollisionMappingListView*)[aListView superview];
        VSC::IM::CollisionAction::SPtr collisionAction = [mappingListView action].lock();
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
                    mappingView = [self newCollisionMappingView];
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
    if ([[aListView superview] isKindOfClass:[VSCIMOSXCollisionEventChainView class]])
    {
        VSCIMOSXCollisionEventChainView* eventChainView = (VSCIMOSXCollisionEventChainView*)[aListView superview];
        VSC::IM::CollisionEventChain::SPtr eventChain = [eventChainView eventChain].lock();
        BOOST_ASSERT(eventChain);
        VSC::IM::Event::SPtr event = VSC::IM::Event::SPtr();
        if (eventChain) event = eventChain->getEventAtIndex((unsigned int)row);
        
        VSC::IM::CollisionAction::SPtr action = boost::dynamic_pointer_cast<VSC::IM::CollisionAction>(event);
        if (action) return [VSCIMOSXCollisionActionView heightOfViewForCollisionAction:action];
        
        VSC::IM::Delay::SPtr delay = boost::dynamic_pointer_cast<VSC::IM::Delay>(event);
        if (delay) return [VSCIMOSXDelayView defaultViewHeight];
        
        BOOST_ASSERT_MSG(false, "Expected CollisionAction or Delay");
    }
    
    else if ([aListView isKindOfClass:[VSCIMOSXCollisionMappingListView class]])
    {
        VSCIMOSXCollisionMappingListView* mappingListView = (VSCIMOSXCollisionMappingListView*)[aListView superview];
        VSC::IM::CollisionAction::SPtr collisionAction = [mappingListView action].lock();
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
    NSLog(@"Selection changed");
}



@end
