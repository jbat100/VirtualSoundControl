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

class VSCEnveloppe {
	
protected:
	
	std::list<VSCEnveloppePoint*> _points;
	double _minimumTimeStep;
	bool _copyAddedPoints;
	std::string _name;
	int _channel;
	
public:
	
	/* getters / setters */
	
	void setName(std::string name);
	std::string getName(void);
	void setChannel(int channel);
	int getChannel(void);
	void setMinimumTimeStep(double minimumTimeStep);
	double getMinimumTimeStep(void);
	
	/* edit */
	
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
	
	VSCEnveloppePoint* getPointClosestToTime(double time);
	VSCEnveloppePoint* getPointClosestToTime(double time, bool copy);
	VSCEnveloppePoint* getFirstPointAfterTime(double time);
	VSCEnveloppePoint* getFirstPointAfterTime(double time, bool copy);
	VSCEnveloppePoint* getFirstPointBeforeTime(double time);
	VSCEnveloppePoint* getFirstPointBeforeTime(double time, bool copy);
	std::list<VSCEnveloppePoint*> getPointsInTimeRange(double lowerTime, double upperTime);
	std::list<VSCEnveloppePoint*> getPointsInTimeRange(double lowerTime, double upperTime, bool copy);
	std::list<VSCEnveloppePoint*> getAllPoints(void);
	std::list<VSCEnveloppePoint*> getAllPoints(bool copy);
	int numberOfPoints(void);
	
	/* value */
	
	double getValueAtTime(double time);
	
	/* sorting */
	
	void sortPointsByTime(void);
	
};

