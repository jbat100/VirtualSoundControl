//
//  Created by Jonathan Thorpe on 10/28/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "PXListViewCell.h"

#include "VSCOBScene.h"
#include "VSCOSXEnvironmentController.h"

@interface VSCOSXOBSceneElementCell : PXListViewCell

@property (nonatomic, assign) VSC::OB::Scene::Element::WPtr element;
@property (assign) id<VSCOSXEnvironmentController> environmentController;

+(CGFloat) defaultViewHeight;

-(void) reload;

@end
