//
//  SettingsManager.h
//  iOSController
//
//  Created by Jonathan Thorpe on 15/01/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface SettingsManager : NSObject <NSCoding> {
	
	NSTimeInterval updateInterval;

}

@property (assign) NSTimeInterval updateInterval;

+(SettingsManager*) instance;

@end
