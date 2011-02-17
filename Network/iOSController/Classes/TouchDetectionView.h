//
//  TouchDetectView.h
//  iOSController
//
//  Created by Jonathan Thorpe on 16/02/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface TouchDetectionView : UIView {
	
	NSMutableArray* currentTouches;

}

@property (nonatomic, retain) NSMutableArray* currentTouches;

@end
