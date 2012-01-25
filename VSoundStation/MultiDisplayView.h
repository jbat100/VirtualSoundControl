//
//  MultiDisplayScrollView.h
//  VSoundStation
//
//  Created by Jonathan Thorpe on 19/02/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "DisplayView.h"


@interface MultiDisplayView : NSView {
	
	NSInteger horizontalSeparator;
	NSMutableArray* displayViews;

}

@property (nonatomic, retain) NSMutableArray* displayViews;

-(void) layoutDisplayViews;

-(DisplayView*) displayViewWithId:(NSString*)viewId;
-(void) addDisplayView:(DisplayView*)dispView;
-(void) removeDisplayView:(DisplayView*)dispView;
-(void) removeDisplayViewWithId:(NSString*)viewId;
-(void) removeAllDisplayViews;

@end
