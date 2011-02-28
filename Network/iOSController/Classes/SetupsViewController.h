//
//  SetupsViewController.h
//  iOSController
//
//  Created by Jonathan Thorpe on 28/02/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface SetupsViewController : UIViewController {
	
	UITableView* setupsTableView;

}

@property (nonatomic, retain) IBOutlet UITableView* setupsTableView;

@end
