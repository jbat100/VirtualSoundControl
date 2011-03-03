//
//  SetupPresets.h
//  iOSController
//
//  Created by Jonathan Thorpe on 03/03/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

typedef enum _ControlCenterScreenLayoutPreset {
	ControlCenterSetupScreenLayoutFullScreen,
	ControlCenterSetupScreenLayoutVerticalSplitScreen,
	ControlCenterSetupScreenLayoutHorizontalSplitScreen,
	ControlCenterSetupScreenLayoutQuad,
	ControlCenterSetupScreenLayoutGameController1,
	ControlCenterSetupScreenLayoutGameController2,
	ControlCenterSetupScreenLayoutMusicController1,
	ControlCenterSetupScreenLayoutMusicController2,
} ControlCenterScreenLayoutPreset;


typedef enum _ControlCenterSetupPreset {
	ControlCenterSetupPresetEmpty = 0,
	ControlCenterSetupPresetFullScreenTouch,
	ControlCenterSetupPresetVerticalSplitScreenTouch,
	ControlCenterSetupPresetHorizontalSplitScreenTouch,
	ControlCenterSetupPresetMusicController1,
	ControlCenterSetupPresetMusicController2
} ControlCenterSetupPreset;
