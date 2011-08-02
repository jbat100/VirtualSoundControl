//
//  EnveloppeViewSetup.h
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 11/03/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>




@interface EnveloppeViewSetup : NSObject <NSCoding> {
	
	double minTime;
	double maxTime;
	
	double minValue;
	double maxValue;
	
	//EnveloppeDisplayType enveloppeDisplayType;
	
	double controlPointRadius;
	double lineWidth;
	
	CGColorRef unselectedControlPointColor;
	CGColorRef selectedControlPointColor;

}

@property (nonatomic, assign) double minTime;
@property (nonatomic, assign) double maxTime;

@property (nonatomic, assign) double minValue;
@property (nonatomic, assign) double maxValue;

@property (nonatomic, assign) EnveloppeDisplayType enveloppeDisplayType;

@property (nonatomic, assign) double controlPointRadius;

@property (nonatomic, readwrite) CGColorRef unselectedControlPointColor;
@property (nonatomic, readwrite) CGColorRef selectedControlPointColor;

@end
