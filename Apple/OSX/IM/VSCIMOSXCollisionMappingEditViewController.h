//
//  VSCIMOSXCollisionMappingEditViewController.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/31/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "VSCIMOSXCollisionMappingTypes.h"

#include "VSC.h"
#include "VSCIMCollisionMapping.h"

@interface VSCIMOSXCollisionMappingEditViewController : NSViewController

@property (nonatomic, assign) VSC::IM::CollisionMapping::WPtr collisionMapping;

// binding targets

@property (nonatomic, assign) VSC::Float offset;
@property (nonatomic, assign) VSC::Float scaleFactor;

@property (nonatomic, strong) IBOutlet NSTextField* offsetTextField;
@property (nonatomic, strong) IBOutlet NSTextField* scaleFactorTextField;

-(IBAction) dismissEditView:(id)sender;

@end
