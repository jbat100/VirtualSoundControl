//
//  CGColorOperations.m
//  EnvelopeEditor
//
//  Created by Jonathan Thorpe on 15/03/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import "CGColor+VSCAdditions.h"



CGColorRef CGColorCreateFromRGBNSColor (const NSColor *color) {
	CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB ();
	NSColor *deviceColor = [color colorUsingColorSpaceName:NSDeviceRGBColorSpace];
	CGFloat components[4];
	[deviceColor getRed:&components[0] green:&components[1] blue:&components[2] alpha:&components[3]];
	
	return CGColorCreate (colorSpace, components);
}

CGColorRef CGColorCreateFromRGB (const float r, const float g, const float b) {
	CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB ();
	CGFloat components[4] = {r, g, b, 1.0};
	return CGColorCreate (colorSpace, components);
}

CGColorRef CGColorCreateFromRGBA (const float r, const float g, const float b, const float a) {
	CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB ();
	CGFloat components[4] = {r, g, b, a};
	return CGColorCreate (colorSpace, components);
}

CGColorRef CGColorCreateFromVSC::Color (const VSC::Color& colour) {
	return CGColorCreateFromRGBA(colour.r, colour.g, colour.b, colour.a);
}

void drawLinearGradientInContext(CGContextRef context, CGRect rect, CGColorRef startColor, CGColorRef endColor) {
	
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    CGFloat locations[] = {0.0, 1.0};
    NSArray *colors = [NSArray arrayWithObjects:(__bridge_transfer id)startColor, (__bridge_transfer id)endColor, nil];
    CGGradientRef gradient = CGGradientCreateWithColors(colorSpace, (__bridge_retained CFArrayRef) colors, locations);
	
    CGPoint startPoint = CGPointMake(CGRectGetMidX(rect), CGRectGetMinY(rect));
	CGPoint endPoint = CGPointMake(CGRectGetMidX(rect), CGRectGetMaxY(rect));
	
	CGContextSaveGState(context);
	CGContextAddRect(context, rect);
	CGContextClip(context);
	CGContextDrawLinearGradient(context, gradient, startPoint, endPoint, 0);
	CGContextRestoreGState(context);
	
	CGGradientRelease(gradient);
	CGColorSpaceRelease(colorSpace);
}

void drawRectOutline(CGContextRef context, CGRect rect, CGFloat w, CGColorRef color) {
	
	CGMutablePathRef linePath = CGPathCreateMutable(); 
	
	CGPathMoveToPoint(linePath, NULL, rect.origin.x, rect.origin.y);
	CGPathAddLineToPoint(linePath, NULL, rect.origin.x + rect.size.width, rect.origin.y); 
	CGPathAddLineToPoint(linePath, NULL, rect.origin.x + rect.size.width, rect.origin.y + rect.size.height); 
	CGPathAddLineToPoint(linePath, NULL, rect.origin.x, rect.origin.y + rect.size.height); 
	CGPathCloseSubpath(linePath);
	
	CGContextSetLineWidth(context, w);
	CGContextSetStrokeColorWithColor(context, color);
	CGContextAddPath(context, linePath);
	CGContextStrokePath(context);
	
	CGPathRelease(linePath);
	
}

void drawRectInnerOutline(CGContextRef context, CGRect rect, CGFloat w, CGColorRef color) {
	
	CGRect innerRect = rect;
	innerRect.origin.x += w/2.0;
	innerRect.origin.y += w/2.0;
	innerRect.size.width -= w;
	innerRect.size.height -= w;
	
	drawRectOutline(context, innerRect, w, color);
	
}

void drawRectOuterOutline(CGContextRef context, CGRect rect, CGFloat w, CGColorRef color) {
	
	CGRect outerRect = rect;
	outerRect.origin.x -= w/2.0;
	outerRect.origin.y -= w/2.0;
	outerRect.size.width += w;
	outerRect.size.height += w;
	
	drawRectOutline(context, outerRect, w, color);
	
}

void drawRectFill(CGContextRef context, CGRect rect, CGColorRef color) {
	
	CGMutablePathRef linePath = CGPathCreateMutable(); 
	
	CGPathMoveToPoint(linePath, NULL, rect.origin.x, rect.origin.y);
	CGPathAddLineToPoint(linePath, NULL, rect.origin.x + rect.size.width, rect.origin.y); 
	CGPathAddLineToPoint(linePath, NULL, rect.origin.x + rect.size.width, rect.origin.y + rect.size.height); 
	CGPathAddLineToPoint(linePath, NULL, rect.origin.x, rect.origin.y + rect.size.height); 
	CGPathCloseSubpath(linePath);
	
	CGContextSetFillColorWithColor(context, color);
	CGContextAddPath(context, linePath);
	CGContextFillPath(context);
	
	CGPathRelease(linePath);
	
}

