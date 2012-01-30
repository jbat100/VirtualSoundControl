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

#include <list>
#include <map>
#include <string>

#include <boost/shared_ptr.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/shared_ptr.hpp>

#include "VSCSound.h"
#include "VSCEnveloppePoint.h"

#define ENVELOPPE_FILE_EXTENSION			"vscxenv"
#define VSCEnveloppePtr						boost::shared_ptr<VSCEnveloppe>


class VSCEnveloppe {
	
	
public:
	
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
	
	enum PointDisplacementConflictResolution {
		PointDisplacementConflictResolutionNone = 0,
		PointDisplacementConflictResolutionBlock,
		PointDisplacementConflictResolutionClear
	};
	
    typedef std::list<VSCEnveloppePointPtr>                             PointList;
    typedef std::list<VSCEnveloppePointPtr>::iterator                   PointIterator;
    typedef std::list<VSCEnveloppePointPtr>::const_iterator             ConstPointIterator;
    typedef std::list<VSCEnveloppePointPtr>::reverse_iterator           ReversePointIterator;
    typedef std::list<VSCEnveloppePointPtr>::const_reverse_iterator     ConstReversePointIterator;
    typedef std::pair<VSCSFloat>                                        ValueRange;
    typedef std::pair<VSCSFloat>                                        TimeRange;
    
	VSCEnveloppe(void);
	// VSCEnveloppe copy construct and file construct
	~VSCEnveloppe(void);
	
	/* getters / setters */
	
	void setCurveType(CurveType curveType);
	CurveType getCurveType(void) const;
	void setPointDisplacementConflictResolution(PointDisplacementConflictResolution pointDisplacementConflictResolution);
	PointDisplacementConflictResolution getPointDisplacementConflictResolution(void) const;
	
	void setFilePath(std::string relativePath);
	std::string getFilePath(void) const;
	std::string getName(void) const; /* Get the last component of the file path */
	
	void setMinimumTimeStep(VSCSFloat minimumTimeStep);
	VSCSFloat getMinimumTimeStep(void) const;
	
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
	
	/* move points (disallow manggling...) */
	bool displacePoint(VSCEnveloppePointPtr point, VSCSFloat deltaTime, VSCSFloat deltaValue);
    bool displacePoint(PointIterator pointIt, VSCSFloat deltaTime, VSCSFloat deltaValue)
	bool displacePoints(PointList& pts, VSCSFloat deltaTime, VSCSFloat deltaValue);
	
	/* values */
	
	VSCSFloat getValueAtTime(VSCSFloat time) const;
	VSCSFloat duration(void) const;
    
    /* extremes */
	
    VSCSFloat minTime(void) const;
    VSCSFloat maxTime(void) const;
    VSCSFloat minValue(void) const;
    VSCSFloat maxValue(void) const;
	
    const PointList& getPoints(void);
    
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
	
	/*
	 *	VSC project data directories will have an enveloppe sub-directory which will serve as bas for the 
	 *	enveloppe's relative path
	 */
	std::string _filePath;
	
    
    bool isSortedByTime(void) const;
    bool canDisplacePoint(VSCEnveloppePointPtr point, VSCSFloat deltaTime, VSCSFloat deltaValue) const;
    
    /* sorting */
	void sortPointsByTime(void);
    
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
		ar  & make_nvp("channel", _channel);
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
 *	Save/Load Enveloppes from XML
 */
void saveVSCEnveloppeToXML(const VSCEnveloppe &s, const char * filepath);
void loadVSCEnveloppeFromXML(VSCEnveloppe &s, const char * filepath);

#endif

