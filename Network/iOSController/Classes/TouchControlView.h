//
//  TouchRelayView.h
//  iOSController
//
//  Created by Jonathan Thorpe on 15/01/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "ControlView.h"

@interface TouchControlView : ControlView {
	
	NSMutableArray* currentTouches;
	
	NSInteger xChannel;
	NSInteger yChannel;

	// a cache of dictionaries
	NSMutableArray* jsonTouchDictionaries;
}

@property (nonatomic, retain) NSMutableArray* currentTouches;

@property (nonatomic, assign) NSInteger xChannel;
@property (nonatomic, assign) NSInteger yChannel;

@end
