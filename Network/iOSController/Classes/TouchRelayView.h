//
//  TouchRelayView.h
//  iOSController
//
//  Created by Jonathan Thorpe on 15/01/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface TouchRelayView : UIView {
	
	NSMutableArray* currentTouches;
	
	NSInteger xChannel;
	NSInteger yChannel;

}

@property (nonatomic, retain) NSMutableArray* currentTouches;

@property (nonatomic, assign) NSInteger xChannel;
@property (nonatomic, assign) NSInteger yChannel;

@end
