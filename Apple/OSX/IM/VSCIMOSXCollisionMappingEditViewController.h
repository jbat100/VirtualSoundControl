//
//  VSCIMOSXCollisionMappingEditViewController.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/31/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#import "VSCIMOSXCollisionMappingTypes.h"

@interface VSCIMOSXCollisionMappingEditViewController : NSViewController <VSCIMOSXCollisionMappingEditor>

@property (nonatomic, strong) IBOutlet NSTextField* targetTextField;
@property (nonatomic, strong) IBOutlet NSTextField* mappingTypeTextField;

@property (nonatomic, strong) IBOutlet NSTextField* offsetLabelTextField;
@property (nonatomic, strong) IBOutlet NSTextField* scaleFactorLabelTextField;

@property (nonatomic, strong) IBOutlet NSTextField* offsetTextField;
@property (nonatomic, strong) IBOutlet NSTextField* scaleFactorTextField;

@end
