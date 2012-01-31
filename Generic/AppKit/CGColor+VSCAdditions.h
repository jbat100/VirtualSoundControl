//
//  CGColorOperations.h
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 15/03/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "VSCColour.h"

/*
 *	These functions follow the same convention as native CG functions
 *	-> Their names contain "Create" and so they should be released after use.
 */
CGColorRef CGColorCreateFromRGBNSColor (NSColor *color);
CGColorRef CGColorCreateFromRGB (float r, float g, float b);
CGColorRef CGColorCreateFromRGBA (float r, float g, float b, float a);
CGColorRef CGColorCreateFromVSCColour (const VSCColour& colour);

void drawLinearGradientInContext(CGContextRef context, CGRect rect, CGColorRef startColor, CGColorRef  endColor);

void drawRectOutline(CGContextRef context, CGRect rect, CGFloat width, CGColorRef color);
void drawRectInnerOutline(CGContextRef context, CGRect rect, CGFloat width, CGColorRef color);
void drawRectOuterOutline(CGContextRef context, CGRect rect, CGFloat width, CGColorRef color);
void drawRectFill(CGContextRef context, CGRect rect, CGColorRef color);
