//
//  TouchViewDescription.h
//  iOSController
//
//  Created by Jonathan Thorpe on 30/01/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface TouchViewDescription : NSObject <NSCoding> {
	
	// Normalized frame parameters should be [0-1] describing the proportions in a superview
	CGRect normalizedFrame;
	
	// Channels which will be controlled by the touch view
	NSInteger xChannel;
	NSInteger yChannel;
	
	// Path to the image which should be used as background for the view
	NSString* backgroundImagePath;

}

@property (nonatomic, assign) CGRect normalizedFrame;
@property (nonatomic, assign) NSInteger xChannel;
@property (nonatomic, assign) NSInteger yChannel;
@property (nonatomic, retain) NSString* backgroundImagePath;

@end
