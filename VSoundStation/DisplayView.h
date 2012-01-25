//
//  DisplayView.h
//  VSoundStation
//
//  Created by Jonathan Thorpe on 19/02/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface DisplayView : NSView {
	
	NSString* viewId;
	
}

@property (nonatomic, retain) NSString* viewId;

@end
