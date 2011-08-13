//
//  CGColorOperations.h
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 15/03/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "VSCColour.h";

CGColorRef CGColorCreateFromRGBNSColor (NSColor *color);
CGColorRef CGColorCreateFromRGB (float r, float g, float b);
CGColorRef CGColorCreateFromRGBA (float r, float g, float b, float a);


CGColorRef CGColorCreateFromVSCColour (const VSCColour& colour);


