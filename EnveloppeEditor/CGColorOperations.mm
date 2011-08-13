//
//  CGColorOperations.m
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 15/03/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import "CGColorOperations.h"



CGColorRef CGColorCreateFromRGBNSColor (NSColor *color) {
	
	CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB ();
	
	NSColor *deviceColor = [color colorUsingColorSpaceName:NSDeviceRGBColorSpace];
	CGFloat components[4];
	[deviceColor getRed:&components[0] green:&components[1] blue:&components[2] alpha:&components[3]];
	
	return CGColorCreate (colorSpace, components);
	
}

CGColorRef CGColorCreateFromRGB (float r, float g, float b) {
	
	CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB ();
	
	CGFloat components[4] = {r, g, b, 1.0};
	
	return CGColorCreate (colorSpace, components);

}

CGColorRef CGColorCreateFromRGBA (float r, float g, float b, float a) {
	
	CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB ();
	
	CGFloat components[4] = {r, g, b, a};
	
	return CGColorCreate (colorSpace, components);

}

CGColorRef CGColorCreateFromVSCColour (const VSCColour& colour) {
	return CGColorCreateFromRGBA(colour.r, colour.g, colour.b, colour.a);
}