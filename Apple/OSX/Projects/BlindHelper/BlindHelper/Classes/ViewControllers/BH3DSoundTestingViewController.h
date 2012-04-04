//
//  BH3DSoundTestingViewController.h
//  BlindHelper
//
//  Created by Jonathan Thorpe on 2/18/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <UIKit/UIKit.h>

@class BH3DTestingSpatialView;
@class BHOpenALPlayback;

@interface BH3DSoundTestingViewController : UIViewController <UIAccelerometerDelegate>

@property (nonatomic, retain) IBOutlet BH3DTestingSpatialView*   spatialView; 
@property (nonatomic, retain) IBOutlet BHOpenALPlayback*         playback;

@property (strong, nonatomic) IBOutlet UIButton*        playPauseButton;
@property (strong, nonatomic) IBOutlet UISwitch*        musicSwitch;
@property (strong, nonatomic) IBOutlet UISwitch*        tiltListenerSwitch;


- (IBAction)musicSwitchValueChanged:(UISwitch*)sender;
- (IBAction)tiltSwitchValueChanged:(UISwitch*)sender;



@end
