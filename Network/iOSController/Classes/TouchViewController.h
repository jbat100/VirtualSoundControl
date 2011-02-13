//
//  TouchViewController.h
//  iOSController
//
//  Created by Jonathan Thorpe on 15/01/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import <UIKit/UIKit.h>
@class TouchControlView;


@interface TouchViewController : UIViewController {
	
	UIImageView* backgroundImageView;
	TouchControlView* touchRelayView;

}

@property (nonatomic, retain) UIImageView* backgroundImageView;
@property (nonatomic, retain) TouchControlView* touchRelayView;

@end
