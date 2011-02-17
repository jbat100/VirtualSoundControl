//
//  TouchRelayView.h
//  iOSController
//
//  Created by Jonathan Thorpe on 15/01/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "ControlView.h"
@class TouchDetectionView;

@interface TouchControlView : ControlView {
	
	TouchDetectionView* touchDetectionView;

	// a cache of dictionaries
	NSMutableArray* jsonTouchDictionaries;
	
}

@property (nonatomic, retain) TouchDetectionView* touchDetectionView;


@end
