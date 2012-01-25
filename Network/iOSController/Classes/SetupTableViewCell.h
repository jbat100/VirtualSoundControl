//
//  SetupTableViewCell.h
//  iOSController
//
//  Created by Jonathan Thorpe on 28/02/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import <UIKit/UIKit.h>

@class SetupSchemaView;
@class ControlCenterSetup;


@interface SetupTableViewCell : UITableViewCell {
	
	ControlCenterSetup* setup;
	
	SetupSchemaView* setupSchemaView;
	UILabel* descriptorLabel;
	
	
}

@property (nonatomic, assign) ControlCenterSetup* setup;

@property (nonatomic, retain) IBOutlet SetupSchemaView* setupSchemaView;
@property (nonatomic, retain) IBOutlet UILabel* descriptorLabel;

-(void) update;

@end
