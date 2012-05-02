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

#include <set>
#include <list>
#include <map>
#include <string>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/shared_ptr.hpp>

#include "VSCSound.h"
#include "VSCEnveloppePoint.h"

class VSCEnveloppe;

typedef boost::shared_ptr<VSCEnveloppe>     VSCEnveloppePtr;
typedef boost::weak_ptr<VSCEnveloppe>       VSCEnveloppeWeakPtr;

class VSCEnveloppe {
	
	
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
        ValueRange(const VSCSFloat _origin, const VSCSFloat _size) : origin(_origin), size(_size) {};
        VSCSFloat origin;
        VSCSFloat size;
    };
    
    struct TimeRange {
        TimeRange() : origin(0.0), size(0.0) {};
        TimeRange(const VSCSFloat _origin, const VSCSFloat _size) : origin(_origin), size(_size) {};
        VSCSFloat origin;
        VSCSFloat size;
    };
	
	enum PointDisplacementConflictResolution {
		PointDisplacementConflictResolutionNone = 0,
		PointDisplacementConflictResolutionBlock,
		PointDisplacementConflictResolutionClear,
        PointDisplacementConflictResolutionMix
	};
	
    typedef std::list<VSCEnveloppePtr>                                  List;
    // set shorthand
    typedef std::set<VSCEnveloppePointPtr>                              PointSet;
    // list shorthand and list iterators (as we use them all the time)
    typedef std::list<VSCEnveloppePointPtr>                             PointList;
    typedef PointList::iterator                                         PointIterator;
    typedef PointList::const_iterator                                   ConstPointIterator;
    typedef PointList::reverse_iterator                                 ReversePointIterator;
    typedef PointList::const_reverse_iterator                           ConstReversePointIterator;
    
	VSCEnveloppe(void);
	// VSCEnveloppe copy construct and file construct
	~VSCEnveloppe(void);
    
    /* Factory */
    
    static VSCEnveloppePtr createFlatEnveloppe(VSCSFloat duration, unsigned int numberOfPoints, VSCSFloat value = 0.0);
    static VSCEnveloppePtr createADSREnveloppe(VSCSFloat attack, VSCSFloat decay, VSCSFloat sustain, VSCSFloat release, VSCSFloat sustainValue = 0.5);
    static VSCEnveloppePtr createEmptyEnveloppe(void);
    static VSCEnveloppePtr createFromXMLFile(const char * filepath);
    
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
    
	void setMinimumTimeStep(VSCSFloat minimumTimeStep);
	VSCSFloat getMinimumTimeStep(void) const;
    void setAllowedTimeRange(TimeRange range);
    TimeRange getAllowedTimeRange(void);
    void setAllowedValueRange(ValueRange range);
    ValueRange getAllowedValueRange(void);
	
	/* edit points */
	
	void addPoint(VSCEnveloppePointPtr point);
	void addPoints(PointList& points); 
	
	void removePoint(VSCEnveloppePointPtr point);
	void removePoints(PointList& points); 
	void removePointsInTimeRange(TimeRange range);
	void removeAllPoints(void);
	
	/* get points iter */
	
	PointIterator getPointBeginIterator(void);
	PointIterator getPointEndIterator(void);
	ConstPointIterator getPointBeginConstIterator(void) const;
	ConstPointIterator getPointEndConstIterator(void) const;
	
	/* get points */
	
	VSCEnveloppePointPtr getPointClosestToTime(VSCSFloat time) const;
	VSCEnveloppePointPtr getFirstPointAfterTime(VSCSFloat time) const;
	VSCEnveloppePointPtr getFirstPointBeforeTime(VSCSFloat time) const;
	
	void getPointsInTimeRange(PointList& pts, TimeRange range) const;
    void getPointsInValueRange(PointList& pts, ValueRange range) const;
	
	int numberOfPoints(void) const;
	
    // display groups of points simultanuously, one block stops all displacement (single point displace is private)
    // this is because the enveloppe is checked for time order and this should be done once all the points in a group 
    // have been moved
	bool displacePoints(PointList& pts, VSCSFloat deltaTime, VSCSFloat deltaValue); 
	
	/* values */
	
	VSCSFloat getValueAtTime(VSCSFloat time) const;
	VSCSFloat duration(void) const;
    
    /* extremes */
	
    VSCSFloat minTime(void) const;
    VSCSFloat maxTime(void) const;
    VSCSFloat minValue(void) const;
    VSCSFloat maxValue(void) const;
	
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
    VSCSFloat _minimumTimeStep;
    TimeRange _allowedTimeRange;
    ValueRange _allowedValueRange;
	
	/*
	 *	VSC project data directories will have an enveloppe sub-directory which will serve as bas for the 
	 *	enveloppe's relative path
	 */
	std::string _filePath;
	
    
    bool isSortedByTime(void) const;
    bool canDisplacePoint(VSCEnveloppePointPtr point, VSCSFloat deltaTime, VSCSFloat deltaValue) const;
    
    /* sorting */
	void sortPointsByTime(void);
    
    /* move points (disallow manggling...) */
	bool displacePoint(VSCEnveloppePointPtr point, VSCSFloat deltaTime, VSCSFloat deltaValue);
    bool displacePoint(PointIterator pointIt, VSCSFloat deltaTime, VSCSFloat deltaValue);
    
    /*
	 *	Enveloppe changes calls (mostly for subclasses to update cache tables)
	 */
    virtual void enveloppeChangedBetweenEnveloppePoints(ConstPointIterator begin, ConstPointIterator end);
	virtual void enveloppeChangedBetweenEnveloppePoints(VSCEnveloppePointPtr begin, VSCEnveloppePointPtr end);
	virtual void enveloppeChangedBetweenEnveloppePointAndNext(VSCEnveloppePointPtr point);
	virtual void enveloppeChanged(void);
    
    /*
	 *	Print out and serialization (private)
	 */
	
	friend std::ostream& operator<<(std::ostream& output, VSCEnveloppe& p);
	
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

void sortPointListByTime(VSCEnveloppe::PointList& points);

#endif

