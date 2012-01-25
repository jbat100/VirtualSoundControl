//
//  TouchDetectView.h
//  iOSController
//
//  Created by Jonathan Thorpe on 16/02/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import <UIKit/UIKit.h>

@protocol TouchDetectionViewDelegate

-(void) eventOccuredForTouchDetectionView:(TouchDetectionView*)view;

@end



@interface TouchDetectionView : UIView {
	
	NSMutableArray* currentTouches;
	
	id<TouchDetectionViewDelegate> delegate;

}

@property (nonatomic, retain) NSMutableArray* currentTouches;
@property (assign) id<TouchDetectionViewDelegate> delegate;

@end
