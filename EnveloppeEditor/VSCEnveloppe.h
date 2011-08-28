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

class VSCEnveloppePoint;

#define VSCEnveloppePointPtr boost::shared_ptr<VSCEnveloppePoint>

#define ENVPNT_ITER				std::list<VSCEnveloppePointPtr>::iterator 
#define REV_ENVPNT_ITER			std::list<VSCEnveloppePointPtr>::reverse_iterator 
#define CONST_ENVPNT_ITER		std::list<VSCEnveloppePointPtr>::const_iterator 
#define CONST_REV_ENVPNT_ITER	std::list<VSCEnveloppePointPtr>::const_reverse_iterator 

typedef enum _VSCEnveloppeType {
	VSCEnveloppeTypeLinear = 1, // in linear mode, enveloppe point control points are ignored
	VSCEnveloppeTypeBezierCubic
} VSCEnveloppeType;


class VSCEnveloppe {
	
protected:
	
	std::list<VSCEnveloppePointPtr> _points;
	
    VSCEnveloppeType _type;
    
    double _minimumTimeStep;
	std::string _name;
	int _channel;
	
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
	
	CONST_ENVPNT_ITER getPointBeginIterator(void) const;
	CONST_ENVPNT_ITER getPointEndIterator(void) const;
	
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
	
	/* sorting */
	
	void sortPointsByTime(void);
	
};

#endif

