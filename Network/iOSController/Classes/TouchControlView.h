//
//  TouchRelayView.h
//  iOSController
//
//  Created by Jonathan Thorpe on 15/01/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "ControlView.h"
#import "TouchDetectionView.h"


@interface TouchControlView : ControlView <TouchDetectionViewDelegate> {
	
	TouchDetectionView* touchDetectionView;

	// a cache of dictionaries
	NSMutableArray* jsonTouchDictionaries;
	
}

@property (nonatomic, retain) TouchDetectionView* touchDetectionView;


@end
