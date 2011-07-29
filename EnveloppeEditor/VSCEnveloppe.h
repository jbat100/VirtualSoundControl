/*
 *  VSCEnveloppe.h
 *  EnveloppeEditor
 *
 *  Created by Jonathan Thorpe on 29/07/2011.
 *  Copyright 2011 JBAT. All rights reserved.
 *
 */

#include <list>
class VSCEnveloppePoint;

class VSCEnveloppe {
	
protected:
	
	std::list<VSCEnveloppePoint*> _points;
	double _minimumTimeStep;
	
public:
	
	void setMinimumTimeStep(double minimumTimeStep);
	double getMinimumTimeStep(void);
	
	/* edit */
	
	void addPoint(VSCEnveloppePoint* point);
	void removePoint(VSCEnveloppePoint* point);
	void removePoint(VSCEnveloppePoint* point, bool clean);
	void addPoints(std::list<VSCEnveloppePoint*> points); 
	void removePoints(std::list<VSCEnveloppePoint*> points); 
	void removePoints(std::list<VSCEnveloppePoint*> points, bool clean); 
	void removePointsInTimeRange(double lowerTime, double upperTime);
	
	/* get points */
	
	VSCEnveloppePoint* getPointClosestToTime(double time);
	VSCEnveloppePoint* getFirstPointAfterTime(double time);
	VSCEnveloppePoint* getFirstPointBeforeTime(double time);
	std::list<VSCEnveloppePoint*> getPointsInTimeRange(double lowerTime, double upperTime);
	std::list<VSCEnveloppePoint*> getAllPoints(void);
	int numberOfPoints(void);
	
	/* value */
	
	double getValueAtTime(double time);
	
	/* sorting */
	
	void sortPointsByTime(void);
	
};

