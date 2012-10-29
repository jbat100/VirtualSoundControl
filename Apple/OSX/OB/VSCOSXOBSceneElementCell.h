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

/*
 *  Environment controller
 */

@property (weak) id<VSCOSXEnvironmentController> environmentController;

/*
 *  Basic info and interface
 */

@property (nonatomic, strong) IBOutlet NSTextField* nameTextField;
@property (nonatomic, strong) IBOutlet NSButton* inspectorButton;

-(void) reload;

-(IBAction) showElementInspector;

@end
