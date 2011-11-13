/*
 *  CGPointOperations.c
 *  EnveloppeEditor
 *
 *  Created by Jonathan Thorpe on 12/03/2011.
 *  Copyright 2011 JBAT. All rights reserved.
 *
 */

#include "NS+VSCGeomOperations.h"
#include <math.h>

CGFloat DistanceBetweenCGPoints(NSPoint point1, NSPoint point2) {
	CGFloat dx = point2.x - point1.x;
	CGFloat dy = point2.y - point1.y;
	return sqrt(dx*dx + dy*dy );
}