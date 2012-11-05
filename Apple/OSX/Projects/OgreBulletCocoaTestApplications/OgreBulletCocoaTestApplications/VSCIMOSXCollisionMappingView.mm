//
//  VSCIMOSXCollisionMappingView.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/28/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCIMOSXCollisionMappingView.h"
#import "VSCIMOSXCollisionMappingGlobal.h"

@interface VSCIMOSXCollisionMappingView ()

/*
 *  Interface elements common to all mappings
 */

@property (nonatomic, strong) IBOutlet NSTextField* targetTextField;
@property (nonatomic, strong) IBOutlet NSTextField* mappingTypeTextField;

@property (nonatomic, strong) IBOutlet NSTextField* offsetLabelTextField;
@property (nonatomic, strong) IBOutlet NSTextField* scaleFactorLabelTextField;

@property (nonatomic, strong) IBOutlet NSTextField* offsetTextField;
@property (nonatomic, strong) IBOutlet NSTextField* scaleFactorTextField;

@property (nonatomic, assign) VSCIMOSXCollisionMappingType collisionMappingType;

-(void) updateInterface;

@end

@implementation VSCIMOSXCollisionMappingView


- (id)initWithFrame:(NSRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code here.
    }
    
    return self;
}

- (void)drawRect:(NSRect)dirtyRect
{
    // Drawing code here.
}

-(void) setCollisionMapping:(VSC::IM::CollisionMapping::WPtr)collisionMapping
{
    _collisionMapping = collisionMapping;
    self.collisionMappingType = VSCIMOSXCollisionMappingTypeForCollisionMapping(collisionMapping.lock());
    
    [self updateInterface];
}

-(void) updateInterface
{
    switch (self.collisionMappingType) {
            
        case VSCIMOSXCollisionMappingTypeConstant:
            [self.mappingTypeTextField setStringValue:@"Constant"];
            break;
            
        case VSCIMOSXCollisionMappingTypeVelocity:
            [self.mappingTypeTextField setStringValue:@"Velocity"];
            break;
            
        case VSCIMOSXCollisionMappingTypeDistance:
            [self.mappingTypeTextField setStringValue:@"Distance"];
            break;
            
        default:
            [self.mappingTypeTextField setStringValue:@"None"];
            break;
    }
    
}


@end
