/*
 *  CGPointOperations.h
 *  EnveloppeEditor
 *
 *  Created by Jonathan Thorpe on 12/03/2011.
 *  Copyright 2011 JBAT. All rights reserved.
 *
 */

#import <Foundation/Foundation.h>

#import "VSCMath.h"

/*
 *  Convert point types (expected VSC::Point and CGPoint) to NSPoint
 */

template <typename PointType>
NSPoint NSMakePointFromPoint(PointType const& p) {
    NSPoint point = NSMakePoint((CGFloat)p.x, (CGFloat)p.y);
    return point;
}

/*
 *  Use template form in VSCMath.h instead
 *

CGFloat DistanceBetweenCGPoints(CGPoint point1, CGPoint point2);
CGFloat DistanceBetweenNSPoints(NSPoint point1, NSPoint point2);

 *
 */