//
//  ControlViewSetup.h
//  iOSController
//
//  Created by Jonathan Thorpe on 13/02/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ControlView.h";

@protocol ControlViewSetup

@property (nonatomic, assign) CGRect normalisedFrame;
@property (nonatomic, retain) NSString* descriptor;
@property (nonatomic, retain) NSString* backgroundImageName;
@property (nonatomic, retain) ControlView* view;

@end



@interface ControlViewSetup : NSObject <ControlViewSetup> {
	
	CGRect normalisedFrame; // Relative frame (all values [0-1])
	NSString* descriptor;
	NSString* backgroundImageName;
	ControlView* view;

}

@property (nonatomic, assign) CGRect normalisedFrame;
@property (nonatomic, retain) NSString* descriptor;
@property (nonatomic, retain) NSString* backgroundImageName;
@property (nonatomic, retain) ControlView* view;

@end
