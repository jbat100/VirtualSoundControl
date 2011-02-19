//
//  ControlSetup.h
//  iOSController
//
//  Created by Jonathan Thorpe on 09/02/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import <UIKit/UIKit.h>

typedef enum _ControlCenterScreenLayoutPreset {
	ControlCenterSetupScreenLayoutFullScreen,
	ControlCenterSetupScreenLayoutVerticalSplitScreen,
	ControlCenterSetupScreenLayoutHorizontalSplitScreen,
	ControlCenterSetupScreenLayoutQuad,
	ControlCenterSetupScreenLayoutGameController1,
	ControlCenterSetupScreenLayoutGameController2,
	ControlCenterSetupScreenLayoutMusicController1,
	ControlCenterSetupScreenLayoutMusicController2,
} _ControlCenterScreenLayoutPreset;


typedef enum _ControlCenterSetupPreset {
	ControlCenterSetupPresetEmpty = 0,
	ControlCenterSetupPresetFullScreenTouch,
	ControlCenterSetupPresetVerticalSplitScreenTouch,
	ControlCenterSetupPresetHorizontalSplitScreenTouch,
	ControlCenterSetupPresetMusicController1,
	ControlCenterSetupPresetMusicController2
} ControlCenterSetupPreset;


@interface ControlCenterSetup : NSObject {
	
	NSString* descriptor;
	
	NSMutableArray* controlViewSetups;

}

@property (nonatomic, retain) NSString* descriptor;
@property (nonatomic, retain) NSMutableArray* controlViewSetups;

-(void) setPreset:(ControlCenterSetupPreset)preset;
-(void) applySetupToView:(UIView*)view;

@end
