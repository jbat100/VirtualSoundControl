//
//  EnveloppeControlPoint.h
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 10/03/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface EnveloppeControlPoint : NSObject <NSCoding> {
	
	double value;
	NSTimeInterval timeStamp;

}

@property (nonatomic, assign) double value;
@property (nonatomic, assign) NSTimeInterval timeStamp;

+(EnveloppeControlPoint*) controlPointWithValue:(double)v andTimeStamp:(NSTimeInterval)t;
-(id) initWithValue:(double)v andTimeStamp:(NSTimeInterval)t;

@end
