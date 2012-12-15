/*
 *  VSCEnveloppe.h
 *  EnveloppeEditor
 *
 *  Created by Jonathan Thorpe on 29/07/2011.
 *  Copyright 2011 JBAT. All rights reserved.
 *
 */

#ifndef _VSC_ENVELOPPE_H_
#define _VSC_ENVELOPPE_H_

#include "VSC.h"
#include "VSCSound.h"
#include "VSCEnveloppePoint.h"

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/shared_ptr.hpp>

#include <set>
#include <list>
#include <map>
#include <string>

namespace VSC {

    class Enveloppe {
        
        
    public:
        
        static const std::string FileExtension;
        
        /*
         *	Curve Type
         */
        enum CurveType {
            CurveTypeNone = 0,
            // in linear mode, enveloppe point control points are ignored (and should't exist anyway)
            CurveTypeLinear = 1, 
            CurveTypeBezierQuadratic,
            CurveTypeBezierCubic
        };
        
        struct ValueRange {
            ValueRange() : origin(0.0), size(0.0) {};
            ValueRange(const Float _origin, const Float _size) : origin(_origin), size(_size) {};
            Float origin;
            Float size;
        };
        
        struct TimeRange {
            TimeRange() : origin(0.0), size(0.0) {};
            TimeRange(const Float _origin, const Float _size) : origin(_origin), size(_size) {};
            Float origin;
            Float size;
        };
        
        enum PointDisplacementConflictResolution {
            PointDisplacementConflictResolutionNone = 0,
            PointDisplacementConflictResolutionBlock,
            PointDisplacementConflictResolutionClear,
            PointDisplacementConflictResolutionMix
        };
        
        typedef std::list<Enveloppe::SPtr>                                  List;
        // set shorthand
        typedef std::set<EnveloppePoint::SPtr>                              PointSet;
        // list shorthand and list iterators (as we use them all the time)
        typedef std::list<EnveloppePoint::SPtr>                             PointList;
        typedef PointList::iterator                                             PointIterator;
        typedef PointList::const_iterator                                       ConstPointIterator;
        typedef PointList::reverse_iterator                                     ReversePointIterator;
        typedef PointList::const_reverse_iterator                               ConstReversePointIterator;
        
        Enveloppe(void);
        // Enveloppe copy construct and file construct
        ~Enveloppe(void);
        
        /* Factory */
        
        static Enveloppe::SPtr createFlatEnveloppe(Float duration, unsigned int numberOfPoints, Float value = 0.0);
        static Enveloppe::SPtr createADSREnveloppe(Float attack, Float decay, Float sustain, Float release, Float sustainValue = 0.5);
        static Enveloppe::SPtr createEmptyEnveloppe(void);
        static Enveloppe::SPtr createFromXMLFile(const char * filepath);
        
        void saveToXMLFile(const char * filepath);
        
        /* getters / setters */
        
        void setCurveType(CurveType curveType);
        CurveType getCurveType(void) const;
        void setPointDisplacementConflictResolution(PointDisplacementConflictResolution pointDisplacementConflictResolution);
        PointDisplacementConflictResolution getPointDisplacementConflictResolution(void) const;
        
        void setFilePath(std::string relativePath);
        std::string getFilePath(void) const;
        std::string getName(void) const; /* Get the last component of the file path */
        
        /* enveloppe limitations */
        
        void setMinimumTimeStep(Float minimumTimeStep);
        Float getMinimumTimeStep(void) const;
        void setAllowedTimeRange(TimeRange range);
        TimeRange getAllowedTimeRange(void);
        void setAllowedValueRange(ValueRange range);
        ValueRange getAllowedValueRange(void);
        
        /* edit points */
        
        void addPoint(EnveloppePoint::SPtr point);
        void addPoints(PointList& points); 
        
        void removePoint(EnveloppePoint::SPtr point);
        void removePoints(PointList& points); 
        void removePointsInTimeRange(TimeRange range);
        void removeAllPoints(void);
        
        /* get points iter */
        
        PointIterator getPointBeginIterator(void);
        PointIterator getPointEndIterator(void);
        ConstPointIterator getPointBeginConstIterator(void) const;
        ConstPointIterator getPointEndConstIterator(void) const;
        
        /* get points */
        
        EnveloppePoint::SPtr getPointClosestToTime(Float time) const;
        EnveloppePoint::SPtr getFirstPointAfterTime(Float time) const;
        EnveloppePoint::SPtr getFirstPointBeforeTime(Float time) const;
        
        void getPointsInTimeRange(PointList& pts, TimeRange range) const;
        void getPointsInValueRange(PointList& pts, ValueRange range) const;
        
        int numberOfPoints(void) const;
        
        // display groups of points simultanuously, one block stops all displacement (single point displace is private)
        // this is because the enveloppe is checked for time order and this should be done once all the points in a group 
        // have been moved
        bool displacePoints(PointList& pts, Float deltaTime, Float deltaValue); 
        
        /* values */
        
        Float getValueAtTime(Float time) const;
        Float duration(void) const;
        
        /* extremes */
        
        Float minTime(void) const;
        Float maxTime(void) const;
        Float minValue(void) const;
        Float maxValue(void) const;
        
        const PointList& getPoints(void) const;
        
    private:
        
        /*
         *	Contains all the enveloppe points
         */
        PointList _points;
        
        /*
         *	The interpolation type determine how values between two enveloppe points are calculated
         */
        CurveType _curveType;
        
        /*
         *	When points are being displaced so that they overlap neighboring points this determines
         *	how the conflict is resolved (block movements, clear neighboring points)
         */
        PointDisplacementConflictResolution _pointDisplacementConflictResolution;
        
        /*
         *	The minimum time step between two adjascent enveloppe points, if a point is added to the enveloppe,
         *	it's neighbourghs which are closer than this time step will be removed from the enveloppe
         */
        Float _minimumTimeStep;
        TimeRange _allowedTimeRange;
        ValueRange _allowedValueRange;
        
        /*
         *	VSC project data directories will have an enveloppe sub-directory which will serve as bas for the 
         *	enveloppe's relative path
         */
        std::string _filePath;
        
        
        bool isSortedByTime(void) const;
        bool canDisplacePoint(EnveloppePoint::SPtr point, Float deltaTime, Float deltaValue) const;
        
        /* sorting */
        void sortPointsByTime(void);
        
        /* move points (disallow manggling...) */
        bool displacePoint(EnveloppePoint::SPtr point, Float deltaTime, Float deltaValue);
        bool displacePoint(PointIterator pointIt, Float deltaTime, Float deltaValue);
        
        /*
         *	Enveloppe changes calls (mostly for subclasses to update cache tables)
         */
        virtual void enveloppeChangedBetweenEnveloppePoints(ConstPointIterator begin, ConstPointIterator end);
        virtual void enveloppeChangedBetweenEnveloppePoints(EnveloppePoint::SPtr begin, EnveloppePoint::SPtr end);
        virtual void enveloppeChangedBetweenEnveloppePointAndNext(EnveloppePoint::SPtr point);
        virtual void enveloppeChanged(void);
        
        /*
         *	Print out and serialization (private)
         */
        
        friend std::ostream& operator<<(std::ostream& output, Enveloppe& p);
        
        friend class boost::serialization::access;
        template<class Archive>
        void save(Archive & ar, const unsigned int version) const
        {
            using boost::serialization::make_nvp;
            // note, version is always the latest when saving
            ar  & make_nvp("points", _points);
            ar  & make_nvp("curve_type", _curveType);
            ar  & make_nvp("point_displacement_conflict_resolution", _pointDisplacementConflictResolution);
            ar  & make_nvp("minimum_time_step", _minimumTimeStep);
        }
        template<class Archive>
        void load(Archive & ar, const unsigned int version)
        {
            using boost::serialization::make_nvp;
            ar  & make_nvp("points", _points);
            ar  & make_nvp("curve_type", _curveType);
            ar  & make_nvp("point_displacement_conflict_resolution", _pointDisplacementConflictResolution);
            ar  & make_nvp("minimum_time_step", _minimumTimeStep);
        }
        
        BOOST_SERIALIZATION_SPLIT_MEMBER()
            
    };

    /*
     *  Helpers
     */

    void sortPointListByTime(Enveloppe::PointList& points);
    
}

#endif

