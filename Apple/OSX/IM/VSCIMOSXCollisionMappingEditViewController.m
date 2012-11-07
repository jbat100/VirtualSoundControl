//
//  VSCIMOSXCollisionMappingEditViewController.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/31/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCIMOSXCollisionMappingEditViewController.h"

@interface VSCIMOSXCollisionMappingEditViewController ()

@property (nonatomic, strong) NSTextField* offsetLabelTextField;
@property (nonatomic, strong) NSTextField* scaleFactorLabelTextField;

@property (nonatomic, strong) NSTextField* offsetTextField;
@property (nonatomic, strong) NSTextField* scaleFactorTextField;

@end

@implementation VSCIMOSXCollisionMappingEditViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Initialization code here.
    }
    
    return self;
}

@end
