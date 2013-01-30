//
//  VSCIMOSXMappingEditViewController.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/31/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "VSC::IM::MappingTypes.h"

#include "VSC.h"
#include "VSCIMMapping.h"

@interface VSCIMOSXMappingEditViewController : NSViewController

@property (nonatomic, assign) VSC::IM::Mapping::WPtr collisionMapping;

// binding targets

@property (nonatomic, assign) VSC::Float offset;
@property (nonatomic, assign) VSC::Float scaleFactor;

@property (nonatomic, strong) IBOutlet NSTextField* offsetTextField;
@property (nonatomic, strong) IBOutlet NSTextField* scaleFactorTextField;

-(IBAction) dismissEditView:(id)sender;

@end
