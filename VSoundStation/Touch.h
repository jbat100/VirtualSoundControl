//
//  Touch.h
//  VSoundStation
//
//  Created by Jonathan Thorpe on 03/02/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface Touch : NSObject {
	
	NSString* touchId;
	
	float xp;
	float yp;
	
	float dx;
	float dy;
	
	NSColor* displayColour;
	
	NSInteger colourIndex;

}

@property (nonatomic, retain) NSString* touchId;

@property (nonatomic, assign) float xp;
@property (nonatomic, assign) float yp;

@property (nonatomic, assign) float dx;
@property (nonatomic, assign) float dy;

@property (nonatomic, retain) NSColor* displayColour;

@property (nonatomic, assign) NSInteger colourIndex;

-(BOOL) isEqualToTouch:(Touch*)t;
-(BOOL) isEqual:(id)object;

-(void) setWithJSONDict:(NSDictionary*)jsonDict;

@end
