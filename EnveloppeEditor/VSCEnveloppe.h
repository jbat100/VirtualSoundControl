/*
 *  VSCEnveloppe.h
 *  EnveloppeEditor
 *
 *  Created by Jonathan Thorpe on 29/07/2011.
 *  Copyright 2011 JBAT. All rights reserved.
 *
 */

#include <list>
#include <string>
class VSCEnveloppePoint;

#define POINT_ITERATOR                  std::list<VSCEnveloppePoint*>::iterator 
#define REVERSE_POINT_ITERATOR          std::list<VSCEnveloppePoint*>::reverse_iterator 
#define CONST_POINT_ITERATOR            std::list<VSCEnveloppePoint*>::const_iterator 
#define CONST_REVERSE_POINT_ITERATOR    std::list<VSCEnveloppePoint*>::const_reverse_iterator 

typedef enum _VSCEnveloppeType {
	VSCEnveloppeTypeLinear = 1, // in linear mode, enveloppe point control points are ignored
	VSCEnveloppeTypeBezierCubic
} VSCEnveloppeType;

class VSCEnveloppe {
	
protected:
	
	std::list<VSCEnveloppePoint*> _points;
	
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
	
	void addPoint(VSCEnveloppePoint* point);
	void addPoints(std::list<VSCEnveloppePoint*> points); 
	
	void removePoint(VSCEnveloppePoint* point);
	void removePoint(VSCEnveloppePoint* point, bool clean);
	void removePoints(std::list<VSCEnveloppePoint*> points); 
	void removePoints(std::list<VSCEnveloppePoint*> points, bool clean); 
	void removePointsInTimeRange(double lowerTime, double upperTime);
	void removePointsInTimeRange(double lowerTime, double upperTime, bool clean);
	
	/* get points */
    
    /* NOTE: when copy is true the caller takes responsibility for deleting the point */
	
	VSCEnveloppePoint* getPointClosestToTime(double time) const;
	VSCEnveloppePoint* getPointClosestToTime(double time, bool copy) const;
	VSCEnveloppePoint* getFirstPointAfterTime(double time) const;
	VSCEnveloppePoint* getFirstPointAfterTime(double time, bool copy) const;
	VSCEnveloppePoint* getFirstPointBeforeTime(double time) const;
	VSCEnveloppePoint* getFirstPointBeforeTime(double time, bool copy) const;
	std::list<VSCEnveloppePoint*> getPointsInTimeRange(double lowerTime, double upperTime) const;
	std::list<VSCEnveloppePoint*> getPointsInTimeRange(double lowerTime, double upperTime, bool copy) const;
	std::list<VSCEnveloppePoint*> getAllPoints(void) const;
	std::list<VSCEnveloppePoint*> getAllPoints(bool copy) const;
	int numberOfPoints(void) const;
	
	/* value */
	
	double getValueAtTime(double time) const;
	
	/* sorting */
	
	void sortPointsByTime(void);
	
};

