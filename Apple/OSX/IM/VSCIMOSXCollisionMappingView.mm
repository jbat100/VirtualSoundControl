//
//  VSCIMOSXCollisionMappingView.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/28/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCIMOSXCollisionMappingView.h"
#import "VSCIMOSXCollisionMappingGlobal.h"

NSDictionary* mappingTypeMenuItemStringDict = nil;

const CGFloat VSCIMOSXCollisionMappingViewHorizontalMargin      = 5.0;
const CGFloat VSCIMOSXCollisionMappingViewVerticalMargin        = 5.0;

const CGFloat VSCIMOSXCollisionMappingViewTargetLabelHeight     = 10.0;
const CGFloat VSCIMOSXCollisionMappingViewPopUpButtonHeight     = 10.0;

const CGFloat VSCIMOSXCollisionMappingViewEditButtonWidth       = 10.0;
const CGFloat VSCIMOSXCollisionMappingViewEditButtonHeight      = 10.0;


@interface VSCIMOSXCollisionMappingView ()

/*
 *  Interface elements common to all mappings
 */

@property (nonatomic, strong) NSTextField* targetTextField;
@property (nonatomic, strong) NSPopUpButton* mappingPopUpButton;

@property (nonatomic, strong) NSButton* editButton;

@property (nonatomic, assign) VSCIMOSXCollisionMappingType collisionMappingType;

-(void) setupCollisionMappingChoice;

-(void) updateInterfaceForNewCollisionMapping;
-(void) updateInterfaceForNewTarget;

@end

@implementation VSCIMOSXCollisionMappingView

@synthesize collisionMapping = _collisionMapping;

+(void) initialize
{
    BOOST_ASSERT(!mappingTypeMenuItemStringDict);
    
    mappingTypeMenuItemStringDict  = @{
    @((int)VSCIMOSXCollisionMappingTypeNone)        : @"No Mapping",
    @((int)VSCIMOSXCollisionMappingTypeConstant)    : @"Constant Mapping",
    @((int)VSCIMOSXCollisionMappingTypeVelocity)    : @"Velocity Mapping",
    @((int)VSCIMOSXCollisionMappingTypeDistance)    : @"Distance Mapping"
    };
}

+(CGFloat) defaultHeightOfView
{
    return 40.0;
}

+(CGFloat) heightOfViewForCollisionMapping:(VSC::IM::CollisionMapping::SPtr)collisionMapping
{
    return 40.0;
}

+(NSString*) menuItemStringForCollisionMappingType:(VSCIMOSXCollisionMappingType)mappingType
{
    BOOST_ASSERT(mappingTypeMenuItemStringDict);
    
    return [mappingTypeMenuItemStringDict objectForKey:@((int)mappingType)];
}

+(VSCIMOSXCollisionMappingType) collisionMappingTypeForMenuItemString:(NSString*)menuItemString
{
    BOOST_ASSERT(mappingTypeMenuItemStringDict);
    
    NSSet* keys = [mappingTypeMenuItemStringDict keysOfEntriesPassingTest:^BOOL(id key, id obj, BOOL *stop) {
        if ([obj isKindOfClass:[NSString class]] == NO) return false;
        if ([(NSString*)obj isEqualToString:menuItemString]) return true;
        return false;
    }];
    
    BOOST_ASSERT([keys count] < 2);
    
    if ([keys count] == 1) return (VSCIMOSXCollisionMappingType)[[keys anyObject] intValue];
    
    return VSCIMOSXCollisionMappingTypeNone;
}


- (id)initWithFrame:(NSRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code here.
        
    }
    
    return self;
}

-(void) awakeFromNib
{
    [self setupCollisionMappingChoice];
}

- (void)drawRect:(NSRect)dirtyRect
{
    // Drawing code here.
}

-(void) setCollisionMapping:(VSC::IM::CollisionMapping::WPtr)collisionMapping
{
    _collisionMapping = collisionMapping;
    self.collisionMappingType = VSCIMOSXCollisionMappingTypeForCollisionMapping(collisionMapping.lock());
    
    [self updateInterfaceForNewCollisionMapping];
}

-(void) setTarget:(VSC::IM::Target)t
{
    _target = t;
    
    [self updateInterfaceForNewTarget];
    
}

-(void) updateInterfaceForNewTarget
{
    
}

-(void) updateInterfaceForNewCollisionMapping
{
    NSString* title = [[self class] menuItemStringForCollisionMappingType:self.collisionMappingType];
    
    [self.mappingPopUpButton selectItemWithTitle:title];
    
    switch (self.collisionMappingType) {
            
        case VSCIMOSXCollisionMappingTypeConstant:
            break;
            
        case VSCIMOSXCollisionMappingTypeVelocity:
            break;
            
        case VSCIMOSXCollisionMappingTypeDistance:
            break;
            
        default:
            break;
    }
    
}

-(void) setupCollisionMappingChoice
{
    
}


@end
