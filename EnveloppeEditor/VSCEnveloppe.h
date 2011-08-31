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

#define VSCEnveloppePtr         boost::shared_ptr<VSCEnveloppe>


typedef enum _VSCEnveloppeType {
	VSCEnveloppeTypeLinear = 1, // in linear mode, enveloppe point control points are ignored (and should't exist anyway)
    VSCEnveloppeTypeBezierQuadratic,
	VSCEnveloppeTypeBezierCubic
} VSCEnveloppeType;


class VSCEnveloppe {
    
    friend std::ostream& operator<<(std::ostream& output, const VSCEnveloppe& p);
	
protected:
	
	std::list<VSCEnveloppePointPtr> _points;
	
    VSCEnveloppeType _type;
    
    double _minimumTimeStep;
	std::string _name;
	int _channel;
    
    bool isSortedByTime(void) const;
    
    /* sorting */
	void sortPointsByTime(void);
	
public:
	
	/* getters / setters */
	
    
	void setName(std::string name);
	std::string getName(void) const;
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

