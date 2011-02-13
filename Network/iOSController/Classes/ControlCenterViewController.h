//
//  ControlViewController.h
//  iOSController
//
//  Created by Jonathan Thorpe on 13/02/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import <UIKit/UIKit.h>
@class ControlCenterSetup;


@interface ControlCenterViewController : UIViewController {
	
	ControlCenterSetup* controlSetup;

}

@property (nonatomic, retain) ControlCenterSetup* controlSetup;

@end
