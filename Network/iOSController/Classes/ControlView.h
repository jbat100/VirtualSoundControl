//
//  RelayView.h
//  iOSController
//
//  Created by Jonathan Thorpe on 09/02/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface ControlView : UIView {
	
	NSString* descriptor;
	NSString* backgroundImageName;

	NSMutableDictionary* jsonDescriptionDictionary;
}

@property (nonatomic, retain) NSString* descriptor;
@property (nonatomic, retain) NSString* backgroundImageName;

@property (nonatomic, readonly) NSMutableDictionary* jsonDescriptionDictionary;

-(void) updateJSONDescriptionDictionary;


@end
