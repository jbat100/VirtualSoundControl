//
//  TouchViewController.h
//  iOSController
//
//  Created by Jonathan Thorpe on 15/01/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import <UIKit/UIKit.h>
@class TouchRelayView;


@interface TouchViewController : UIViewController {
	
	UIImageView* backgroundImageView;
	TouchRelayView* touchRelayView;

}

@property (nonatomic, retain) UIImageView* backgroundImageView;
@property (nonatomic, retain) TouchRelayView* touchRelayView;

@end
