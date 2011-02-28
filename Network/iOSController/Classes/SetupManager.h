//
//  SetupManager.h
//  iOSController
//
//  Created by Jonathan Thorpe on 28/02/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface SetupManager : NSObject {
	
	NSMutableArray* controlCenterSetups;

}

@property (nonatomic, retain) NSMutableArray* controlCenterSetups;

@end
