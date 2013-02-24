//
//  VSCIMOSXMappingEditViewController.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/31/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#include "VSC.h"
#include "VSCIM.h"

@class VSCOSXKeyed1FieldView;

@interface VSCIMOSXMappingEditViewController : NSViewController <NSTextFieldDelegate>

@property (nonatomic, assign) VSC::IM::Mapping_WPtr mapping;

@property (nonatomic, strong) VSCOSXKeyed1FieldView* offsetKeyedFieldView;
@property (nonatomic, strong) VSCOSXKeyed1FieldView* scaleFactorKeyedFieldView;

-(IBAction) dismissEditView:(id)sender;

@end
