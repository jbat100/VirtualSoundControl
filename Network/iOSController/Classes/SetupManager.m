//
//  SetupManager.m
//  iOSController
//
//  Created by Jonathan Thorpe on 28/02/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import "SetupManager.h"

@interface SetupManager () 

-(void) generatePresets;
-(ControlCenterSetup*) controlCenterSetupPreset:(ControlCenterSetupPreset)preset;

@end


@implementation SetupManager

@synthesize presetSetups, userSetups;

static SetupManager* sInstance = nil;

+(SettingsManager*) instance {
	
	@synchronized (self) {
		if (!sInstance) 
			sInstance = [[SettingsManager alloc] init];
	}
	
	return sInstance;
	
}

-(id) init {
	
	if ((self = [super init])) {
		self.presetSetups = [NSMutableArray arrayWithCapacity:10];
		self.userSetups = [NSMutableArray arrayWithCapacity:10];
		[self generatePresets];
	}
	
	return self;
	
}

-(void) dealloc {
	
	self.presetSetups = nil;
	self.userSetups = nil;
	
	[super dealloc];
	
}

-(void) generatePresets {
	
	[presetSetups addObject:[self controlCenterSetupPreset:ControlCenterSetupPresetFullScreenTouch]];
	[presetSetups addObject:[self controlCenterSetupPreset:ControlCenterSetupPresetVerticalSplitScreenTouch]];
	[presetSetups addObject:[self controlCenterSetupPreset:ControlCenterSetupPresetHorizontalSplitScreenTouch]];
	[presetSetups addObject:[self controlCenterSetupPreset:ControlCenterSetupPresetMusicController1]];
	[presetSetups addObject:[self controlCenterSetupPreset:ControlCenterSetupPresetMusicController2]];
	
}

-(ControlCenterSetup*) controlCenterSetupPreset:(ControlCenterSetupPreset)preset {
	
	if (preset == ControlCenterSetupPresetFullScreenTouch) {
		
	}
	else if (preset == ControlCenterSetupPresetVerticalSplitScreenTouch) {
		
	}
	else if (preset == ControlCenterSetupPresetHorizontalSplitScreenTouch) {
		
	}
	else if (preset == ControlCenterSetupPresetMusicController1) {
		
	}
	else if (preset == ControlCenterSetupPresetMusicController2) {
		
	}
	
}

@end
