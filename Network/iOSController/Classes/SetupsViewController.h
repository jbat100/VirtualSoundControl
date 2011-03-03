//
//  SetupsViewController.h
//  iOSController
//
//  Created by Jonathan Thorpe on 28/02/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "SetupTableViewCell.h"

@class SetupManager;


@interface SetupsViewController : UIViewController <UITableViewDelegate, UITableViewDataSource> {
	
	SetupTableViewCell* setupTableViewCell;
	UITableView* setupsTableView;
	
	SetupManager* setupManager;

}

@property (nonatomic, retain) IBOutlet SetupTableViewCell* setupTableViewCell;
@property (nonatomic, retain) IBOutlet UITableView* setupsTableView;

@property (nonatomic, assign) NSMutableArray* setups;

@end
