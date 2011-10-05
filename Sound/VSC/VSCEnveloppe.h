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
#include <string>
#include <boost/shared_ptr.hpp>

#include <boost/serialization/list.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/split_member.hpp>

#include "VSCEnveloppePoint.h"

#define VSCEnveloppePtr		boost::shared_ptr<VSCEnveloppe>


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
	
	/*
	 *	Scale Type
	 */
	enum ScaleType {
		ScaleTypeNone = 0,
		ScaleTypeLinear = 1,
		ScaleTypeLinearDB
	};
	
	/*
	 *	State
	 */
	enum State {
		StateNone = 0,
		StateOff = 1,
		StateScheduled,
		StateStarted,
		StateEnded
	};
	
	VSCEnveloppe(void);
	// VSCEnveloppe copy construct and file construct
	
	void setToDefault(void);
	
	/* FIRE !!! */
	virtual void fire(void);
	virtual void fireAfterInterval(double intervalTime);
	virtual void setCurrentTime(double currentTime);
	
	/* getters / setters */
	
	void setScaleType(ScaleType scaleType);
	ScaleType getScaleType(void) const;
	void setCurveType(CurveType curveType);
	CurveType getCurveType(void) const;
	void setState(State state);
	State getState(void) const;
	
	void setRelativePath(std::string relativePath);
	std::string getRelativePath(void) const;
	std::string getName(void) const; /* Get the last component of the file path */
	
	void setChannel(int channel);
	int getChannel(void) const;
	void setMinimumTimeStep(double minimumTimeStep);
	double getMinimumTimeStep(void) const;
	
	/* edit points */
	
	void addPoint(VSCEnveloppePointPtr point);
	void addPoints(std::list<VSCEnveloppePointPtr>& points); 
	
	void removePoint(VSCEnveloppePointPtr point);
	void removePoints(std::list<VSCEnveloppePointPtr>& points); 
	void removePointsInTimeRange(double lowerTime, double upperTime);
	void removeAllPoints(void);
	
	/* get points iter */
	
	EnvPntIter getPointBeginIterator(void);
	EnvPntIter getPointEndIterator(void);
	ConstEnvPntIter getPointBeginConstIterator(void) const;
	ConstEnvPntIter getPointEndConstIterator(void) const;
	
	/* get points */
	
	VSCEnveloppePointPtr getPointClosestToTime(double time) const;
	VSCEnveloppePointPtr getPointClosestToTime(double time, bool copy) const;
	VSCEnveloppePointPtr getFirstPointAfterTime(double time) const;
	VSCEnveloppePointPtr getFirstPointAfterTime(double time, bool copy) const;
	VSCEnveloppePointPtr getFirstPointBeforeTime(double time) const;
	VSCEnveloppePointPtr getFirstPointBeforeTime(double time, bool copy) const;
	
	void getPointsInTimeRange(std::list<VSCEnveloppePointPtr>& pts, double lowerTime, double upperTime) const;
	void getPointsInTimeRange(std::list<VSCEnveloppePointPtr>& pts, double lowerTime, double upperTime, bool copy) const;
	void getAllPoints(std::list<VSCEnveloppePointPtr>& pts) const;
	void getAllPoints(std::list<VSCEnveloppePointPtr>& pts, bool copy) const;
	
	int numberOfPoints(void) const;
	
	/* move points (disallow manggling...) */
	bool canDisplacePointTime(ConstEnvPntIter pointIt, double deltaTime);
	bool canDisplacePointValue(ConstEnvPntIter pointIt, double deltaValue);
	void displacePoint(VSCEnveloppePointPtr point, double deltaTime, double deltaValue);
	void displacePoint(ConstEnvPntIter pointIt, double deltaTime, double deltaValue);
	void displacePoints(std::list<VSCEnveloppePointPtr>& pts, double deltaTime, double deltaValue);
	
	/* values */
	
	double getValueAtTime(double time) const;
	double duration(void) const;
    
    /* extremes */
	
    double minTime(void) const;
    double maxTime(void) const;
    double minValue(void) const;
    double maxValue(void) const;
	
	
	
	
private:
	/*
	 *	Print out and serialization (private)
	 */
	
	friend std::ostream& operator<<(std::ostream& output, VSCEnveloppe& p);
	
	friend class boost::serialization::access;
    template<class Archive>
    void save(Archive & ar, const unsigned int version) const
    {
        // note, version is always the latest when saving
        ar  & _points;
        ar  & _scaleType;
    }
    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
        ar  & _points;
        ar  & _scaleType;
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
	 *	The minimum time step between two adjascent enveloppe points, if a point is added to the enveloppe,
	 *	it's neighbourghs which are closer than this time step will be removed from the enveloppe
	 */
    double _minimumTimeStep;
	
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

#endif

