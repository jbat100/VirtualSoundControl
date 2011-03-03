//
//  SetupManager.h
//  iOSController
//
//  Created by Jonathan Thorpe on 28/02/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ControlCenterSetup.h"
#import "SetupPresets.h"

@interface SetupManager : NSObject {
	
	NSMutableArray* presetSetups;
	NSMutableArray* userSetups;

}

@property (nonatomic, retain) NSMutableArray* presetSetups;
@property (nonatomic, retain) NSMutableArray* userSetups;

+(SetupManager*) instance;

@end
