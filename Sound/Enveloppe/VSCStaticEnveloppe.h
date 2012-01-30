//
//  VSCStaticEnveloppe.h
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 1/26/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#ifndef VSC_STATIC_ENVELOPPE_H
#define VSC_STATIC_ENVELOPPE_H



#include <list>
#include <string>

#include <boost/shared_ptr.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/shared_ptr.hpp>

#include "VSCSound.h"
#include "VSCStaticEnveloppePoint.h"

#define ENVELOPPE_FILE_EXTENSION			"vscxenv"
#define VSCEnveloppePtr						boost::shared_ptr<VSCEnveloppe>


class VSCStaticEnveloppe {
	
	
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
	
    typedef std::list<VSCStaticEnveloppePoint>::iterator PointIterator 
    
	VSCStaticEnveloppe(void);
	// VSCEnveloppe copy construct and file construct
	~VSCStaticEnveloppe(void);
	
	void setToDefault(void);
	
	
	/* getters / setters */
	
	void setCurveType(CurveType curveType);
	CurveType getCurveType(void) const;
	void setPointDisplacementConflictResolution(PointDisplacementConflictResolution pointDisplacementConflictResolution);
	PointDisplacementConflictResolution getPointDisplacementConflictResolution(void) const;
	
	void setRelativePath(std::string relativePath);
	std::string getRelativePath(void) const;
	std::string getName(void) const; /* Get the last component of the file path */
	
	void setMinimumTimeStep(VSCSFloat minimumTimeStep);
	VSCSFloat getMinimumTimeStep(void) const;
	
	/* edit points */
	
	void addPoint(VSCStaticEnveloppePoint point);
	void addPoints(std::list<VSCStaticEnveloppePoint>& points); 
	
	void removePointsInRange(std::pair<VSCSFloat> timeRange, std::pair<VSCSFloat> valueRange);
	void removeAllPoints(void);
	
	/* get points iter */
	
	EnvPntIter getPointBeginIterator(void);
	EnvPntIter getPointEndIterator(void);
	ConstEnvPntIter getPointBeginConstIterator(void) const;
	ConstEnvPntIter getPointEndConstIterator(void) const;
	
	/* get points */
	
	VSCEnveloppePointPtr getPointClosestToTime(VSCSFloat time) const;
	VSCEnveloppePointPtr getPointClosestToTime(VSCSFloat time, bool copy) const;
	VSCEnveloppePointPtr getFirstPointAfterTime(VSCSFloat time) const;
	VSCEnveloppePointPtr getFirstPointAfterTime(VSCSFloat time, bool copy) const;
	VSCEnveloppePointPtr getFirstPointBeforeTime(VSCSFloat time) const;
	VSCEnveloppePointPtr getFirstPointBeforeTime(VSCSFloat time, bool copy) const;
	
	void getPointsInTimeRange(std::list<VSCEnveloppePointPtr>& pts, VSCSFloat lowerTime, VSCSFloat upperTime) const;
	void getPointsInTimeRange(std::list<VSCEnveloppePointPtr>& pts, VSCSFloat lowerTime, VSCSFloat upperTime, bool copy) const;
	void getAllPoints(std::list<VSCEnveloppePointPtr>& pts) const;
	void getAllPoints(std::list<VSCEnveloppePointPtr>& pts, bool copy) const;
	
	int numberOfPoints(void) const;
	
	/* move points (disallow manggling...) */
	bool canDisplacePointTime(ConstEnvPntIter pointIt, VSCSFloat deltaTime);
	bool canDisplacePointValue(ConstEnvPntIter pointIt, VSCSFloat deltaValue);
	void displacePoint(VSCEnveloppePointPtr point, VSCSFloat deltaTime, VSCSFloat deltaValue);
	void displacePoint(ConstEnvPntIter pointIt, VSCSFloat deltaTime, VSCSFloat deltaValue);
	void displacePoints(std::list<VSCEnveloppePointPtr>& pts, VSCSFloat deltaTime, VSCSFloat deltaValue);
	
	/* values */
	
	VSCSFloat getValueAtTime(VSCSFloat time) const;
	VSCSFloat duration(void) const;
    
    /* extremes */
	
    VSCSFloat minTime(void) const;
    VSCSFloat maxTime(void) const;
    VSCSFloat minValue(void) const;
    VSCSFloat maxValue(void) const;
	
private:
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
		ar  & make_nvp("scale_type", _scaleType);
		ar  & make_nvp("curve_type", _curveType);
		ar  & make_nvp("state", _state);
		ar  & make_nvp("point_displacement_conflict_resolution", _pointDisplacementConflictResolution);
		ar  & make_nvp("minimum_time_step", _minimumTimeStep);
		ar  & make_nvp("relative_path", _relativePath);
		ar  & make_nvp("channel", _channel);
    }
    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
		using boost::serialization::make_nvp;
		ar  & make_nvp("points", _points);
		ar  & make_nvp("scale_type", _scaleType);
		ar  & make_nvp("curve_type", _curveType);
		ar  & make_nvp("state", _state);
		ar  & make_nvp("point_displacement_conflict_resolution", _pointDisplacementConflictResolution);
		ar  & make_nvp("minimum_time_step", _minimumTimeStep);
		ar  & make_nvp("relative_path", _relativePath);
		ar  & make_nvp("channel", _channel);
    }
	
    BOOST_SERIALIZATION_SPLIT_MEMBER()
	
protected:
	
	/*
	 *	Contains all the enveloppe points
	 */
	std::list<VSCEnveloppePointPtr> _points;
	
	/*
	 *	The scale type determines the scaling undergone by generated enveloppe points during firing
	 *	(if linear then no scaling, if DB then 0.1pow(v,10))
	 */
	ScaleType _scaleType;
	
	/*
	 *	The interpolation type determine how values between two enveloppe points are calculated
	 */
    CurveType _curveType;
	
	/*
	 *	State (defines the fire state during audio computations)
	 */
    State _state;
	
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
	std::string _relativePath;
	
	/*
	 *	The enveloppe has an associated int channel which can be used to define where update signals are sent 
	 *	when the enveloppe is fired (consider also adding observers)
	 */
	int _channel;
	
    
    bool isSortedByTime(void) const;
    
    /* sorting */
	void sortPointsByTime(void);
	
	/*
	 *	Enveloppe changes calls (mostly for subclasses to update cache tables)
	 */
	virtual void enveloppeChangedBetweenEnveloppePoints(VSCEnveloppePointPtr begin, VSCEnveloppePointPtr end);
	virtual void enveloppeChangedBetweenEnveloppePointAndNext(VSCEnveloppePointPtr point);
	virtual void enveloppeChanged(void);
    
};

/*
 *	Save/Load Enveloppes from XML
 */
void saveVSCEnveloppeToXML(const VSCEnveloppe &s, const char * filename);
void loadVSCEnveloppeFromXML(VSCEnveloppe &s, const char * filename);



#endif
