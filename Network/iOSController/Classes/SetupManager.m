//
//  SetupManager.m
//  iOSController
//
//  Created by Jonathan Thorpe on 28/02/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import "SetupManager.h"
#import "SliderSetup.h"
#import "TouchControlViewSetup.h"
#import "SliderControlViewSetup.h"
#import "ButtonControlViewSetup.h"

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
	
	ControlCenterSetup* controlCenterSetup = [[[ControlCenterSetup alloc] init] autorelease];
	
	if (preset == ControlCenterSetupPresetFullScreenTouch) {
		
		TouchControlViewSetup* viewSetup1 = [[TouchControlViewSetup alloc] init];
		viewSetup1.normalisedFrame = CGRectMake(0.0, 0.0, 1.0, 1.0);
		[controlCenterSetup.controlViewSetups addObject:viewSetup1];
		[viewSetup1 release];
		
	}
	
	else if (preset == ControlCenterSetupPresetVerticalSplitScreenTouch) {
		
		TouchControlViewSetup* viewSetup1 = [[TouchControlViewSetup alloc] init];
		viewSetup1.normalisedFrame = CGRectMake(0.0, 0.0, 0.5, 1.0);
		[controlCenterSetup.controlViewSetups addObject:viewSetup1];
		[viewSetup1 release];
		
		TouchControlViewSetup* viewSetup2 = [[TouchControlViewSetup alloc] init];
		viewSetup2.normalisedFrame = CGRectMake(0.5, 0.0, 0.5, 1.0);
		[controlCenterSetup.controlViewSetups addObject:viewSetup2];
		[viewSetup2 release];
		
	}
	
	else if (preset == ControlCenterSetupPresetHorizontalSplitScreenTouch) {
		
		TouchControlViewSetup* viewSetup1 = [[TouchControlViewSetup alloc] init];
		viewSetup1.normalisedFrame = CGRectMake(0.0, 0.0, 1.0, 0.5);
		[controlCenterSetup.controlViewSetups addObject:viewSetup1];
		[viewSetup1 release];
		
		TouchControlViewSetup* viewSetup2 = [[TouchControlViewSetup alloc] init];
		viewSetup2.normalisedFrame = CGRectMake(0.0, 0.5, 1.0, 0.5);
		[controlCenterSetup.controlViewSetups addObject:viewSetup2];
		[viewSetup2 release];
		
	}
	
	else if (preset == ControlCenterSetupPresetMusicController1) {
		
		TouchControlViewSetup* viewSetup1 = [[TouchControlViewSetup alloc] init];
		viewSetup1.normalisedFrame = CGRectMake(0.0, 0.0, 1.0, 0.7);
		[controlCenterSetup.controlViewSetups addObject:viewSetup1];
		[viewSetup1 release];
		
		SliderControlViewSetup* viewSetup2 = [[SliderControlViewSetup alloc] init];
		viewSetup2.normalisedFrame = CGRectMake(0.0, 0.7, 1.0, 0.3);
		SliderSetup* sliderSetup = [[SliderSetup alloc] init];
		NSArray* sliderCannels = [NSArray arrayWithObjects:
								  [NSNumber numberWithInteger:1],
								  [NSNumber numberWithInteger:2],
								  [NSNumber numberWithInteger:3], nil];
		[viewSetup2 setSliderSetup:sliderSetup forChannels:sliderCannels];
		[controlCenterSetup.controlViewSetups addObject:viewSetup2];
		[viewSetup2 release];
		
	}
	else if (preset == ControlCenterSetupPresetMusicController2) {
		
	}
	
	return controlCenterSetup;
	
}

@end
