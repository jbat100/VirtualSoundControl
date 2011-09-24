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

#include "VSCEnveloppePoint.h"

#define VSCEnveloppePtr		boost::shared_ptr<VSCEnveloppe>

typedef enum _VSCEnveloppeInterpolationType {
	VSCEnveloppeInterpolationTypeNone = 0,
	// in linear mode, enveloppe point control points are ignored (and should't exist anyway)
	VSCEnveloppeInterpolationTypeLinear = 1, 
    VSCEnveloppeInterpolationTypeBezierQuadratic,
	VSCEnveloppeInterpolationTypeBezierCubic
} VSCEnveloppeInterpolationType;

typedef enum _VSCEnveloppeScaleType {
	VSCEnveloppeScaleTypeNone = 0,
	VSCEnveloppeScaleTypeLinear = 1,
	VSCEnveloppeScaleTypeLinearDB
} VSCEnveloppeScaleType;


class VSCEnveloppe {
    
    friend std::ostream& operator<<(std::ostream& output, const VSCEnveloppe& p);
	
protected:
	
	/*
	 *	Contains all the enveloppe points
	 */
	std::list<VSCEnveloppePointPtr> _points;
	
	/*
	 *	The scale type determines the scaling undergone by generated enveloppe points during firing
	 *	(if linear then no scaling, if DB then 0.1pow(v,10))
	 */
	VSCEnveloppeScaleType _scaleType;
	
	/*
	 *	The interpolation type determine how values between two enveloppe points are calculated
	 */
    VSCEnveloppeInterpolationType _interpolationType;
    
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
	
public:
	
	/* FIRE !!! */
	void fire(void);
	void fireAfterInterval(double time);
	
	/* getters / setters */
	
	void setScaleType(VSCEnveloppeScaleType scaleType);
	VSCEnveloppeScaleType getScaleType(void) const;
	void setInterpolationType(VSCEnveloppeInterpolationType interpolationType);
	VSCEnveloppeInterpolationType getInterpolationType(void) const;
	
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
	
	/* get points iter */
	
	ConstEnvPntIter getPointBeginIterator(void) const;
	ConstEnvPntIter getPointEndIterator(void) const;
	
	/* get points */
	
	VSCEnveloppePointPtr getPointClosestToTime(double time) const;
	VSCEnveloppePointPtr getPointClosestToTime(double time, bool copy) const;
	VSCEnveloppePointPtr getFirstPointAfterTime(double time) const;
	VSCEnveloppePointPtr getFirstPointAfterTime(double time, bool copy) const;
	VSCEnveloppePointPtr getFirstPointBeforeTime(double time) const;
	VSCEnveloppePointPtr getFirstPointBeforeTime(double time, bool copy) const;
	
	void getPointsInTimeRange(std::list<VSCEnveloppePointPtr>&, double lowerTime, double upperTime) const;
	void getPointsInTimeRange(std::list<VSCEnveloppePointPtr>&, double lowerTime, double upperTime, bool copy) const;
	void getAllPoints(std::list<VSCEnveloppePointPtr>&) const;
	void getAllPoints(std::list<VSCEnveloppePointPtr>&, bool copy) const;
	
	int numberOfPoints(void) const;
	
	/* value */
	
	double getValueAtTime(double time) const;
    
    /* extremes */
	
    double minTime(void) const;
    double maxTime(void) const;
    double minValue(void) const;
    double maxValue(void) const;
	
};

#endif

