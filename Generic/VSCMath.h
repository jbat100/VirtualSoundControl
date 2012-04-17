//
//  VSCMath.h
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 8/9/11.
//  Copyright 2011 NXP. All rights reserved.
//

#ifndef _VSC_MATH_H_
#define _VSC_MATH_H_

namespace VSC {
    
    const double kPI = 3.1415926535897932384626433832795028;
    
    typedef struct {
        float x;
        float y;
    } Point;
    
    typedef struct {
        float width;
        float height;
    } Size;
    
    typedef struct {
        Point origin;
        Size  size;
    } Rect;
    
    /*
     *  Degrees / radians 
     */
    
    template <typename T>
    T DegreesToRadians(T const& deg) {return (deg/180.0)*kPI;}
    
    template <typename T>
    T RadiansToDegrees(T const& rad) {return (rad/kPI)*180.0;}
    
    template <typename T>
    T NormalizedToRadians(T const& norm) {return norm*(2.0*kPI);}
    
    template <typename T>
    T RadiansToNormalized(T const& rad) {return rad/(2.0*kPI);}
    
    
    namespace Geom {
        
        /*
         *  Template Distance between points (use VSC::Point, CGPoint or NSPoint)
         */
        
        template <typename DistanceType, typename PointType>
        DistanceType const& DistanceBetweenPoints(PointType const& point1, PointType const& point2) {
            DistanceType dx = point2.x - point1.x;
            DistanceType dy = point2.y - point1.y;
            return sqrt(dx*dx + dy*dy);
        }
        
    }
	
}

#endif