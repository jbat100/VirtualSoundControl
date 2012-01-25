//
//  RelayView.h
//  iOSController
//
//  Created by Jonathan Thorpe on 09/02/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface ControlView : UIView  {
	
	UIImageView* backgroundImageView;
	
	BOOL changedSinceLastUpdate;

	NSMutableDictionary* jsonDescriptionDictionary;
	NSObject* setup;
	
}

@property (nonatomic, readonly) NSMutableDictionary* jsonDescriptionDictionary;
@property (nonatomic, assign) NSObject* setup;

-(void) updateJSONDescriptionDictionary;
-(void) applySetup;

@end
