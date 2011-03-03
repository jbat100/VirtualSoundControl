//
//  ControlSetup.h
//  iOSController
//
//  Created by Jonathan Thorpe on 09/02/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface ControlCenterSetup : NSObject {
	
	NSString* descriptor;
	
	NSMutableArray* controlViewSetups;

}

@property (nonatomic, retain) NSString* descriptor;
@property (nonatomic, retain) NSMutableArray* controlViewSetups;

-(void) applySetupToView:(UIView*)view;

@end
