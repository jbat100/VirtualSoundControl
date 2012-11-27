//
//  VSCIMOSXCollisionActionMappingsViewController.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 11/27/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCIMOSXCollisionActionMappingsViewController.h"

#import "VSCIMOSXCollisionEventChainController.h"


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

@end
