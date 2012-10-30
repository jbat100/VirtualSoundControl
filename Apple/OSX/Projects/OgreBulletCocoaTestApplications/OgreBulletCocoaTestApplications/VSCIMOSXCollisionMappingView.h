//
//  VSCIMOSXCollisionMappingView.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/28/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#include "VSCIMCollisionMapping.h"

typedef enum _VSCIMOSXCollisionMappingType {
    VSCIMOSXCollisionMappingTypeNone = 0,
    VSCIMOSXCollisionMappingTypeConstant,
    VSCIMOSXCollisionMappingTypeVelocity,
    VSCIMOSXCollisionMappingTypeDistance
} VSCIMOSXCollisionMappingType;

@class VSCIMOSXCollisionMappingView;

@protocol VSCIMOSXCollisionMappingViewDelegate <NSObject>

-(VSC::IM::CollisionMapping::SPtr) collisionMappingView:(VSCIMOSXCollisionMappingView*)view
                                requestsMappingWithType:(VSCIMOSXCollisionMappingType)mappingType;

@end


@interface VSCIMOSXCollisionMappingView : NSView

+(CGFloat) heightOfViewForCollisionMapping:(VSC::IM::CollisionMapping::SPtr)collisionMapping;

@property (nonatomic, assign) VSC::IM::CollisionMapping::WPtr collisionMapping;

/*
 *  Interface elements common to all mappings
 */

@property (nonatomic, strong) NSTextField* offsetLabelTextField;
@property (nonatomic, strong) NSTextField* scaleFactorLabelTextField;

@property (nonatomic, strong) NSTextField* offsetTextField;
@property (nonatomic, strong) NSTextField* scaleFactorTextField;

@end

