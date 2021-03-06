
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
    
    template <typename PointType>
    Point MakePointFromPoint(const PointType& p) {
        Point vscPoint = {p.x, p.y};
        return vscPoint;
    }
    
    template <typename SizeType>
    Size MakeSizeFromSize(const SizeType& s) {
        Size vscSize = {s.width, s.height};
        return vscSize;
    }
    
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
        DistanceType DistanceBetweenPoints(PointType const& point1, PointType const& point2) {
            DistanceType dx = point2.x - point1.x;
            DistanceType dy = point2.y - point1.y;
            return (DistanceType)sqrt(dx*dx + dy*dy);
        }
        
    }
	
}

#endif