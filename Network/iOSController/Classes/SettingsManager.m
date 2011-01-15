//
//  SettingsManager.m
//  iOSController
//
//  Created by Jonathan Thorpe on 15/01/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import "SettingsManager.h"
#import "iOSControllerAppDelegate.h"


@implementation SettingsManager

static SettingsManager* sInstance = nil;

+(SettingsManager*) instance {
	
	@synchronized (self) {
		
		if (!sInstance) {
			
			NSString* filePath = [[iOSControllerAppDelegate sharedAppDelegate] settingsFilePath];
			
			if ([[NSFileManager defaultManager] fileExistsAtPath:filePath]) 
				sInstance = [NSKeyedUnarchiver unarchiveObjectWithFile:filePath];
			
			if (!sInstance) 
				sInstance = [[SettingsManager alloc] init];
			
		}
		
	}
	
	return sInstance;
	
}

-(void) encodeWithCoder:(NSCoder *)coder {
	
	[coder encodeDouble:(double)updateInterval forKey:@"SettingsManagerUpdateInterval"];
	
}

-(id) initWithCoder:(NSCoder *)coder {
	
	updateInterval = [coder decodeDoubleForKey:@"SettingsManagerUpdateInterval"];
	
}

@end
