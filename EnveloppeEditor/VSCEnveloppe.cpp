/*
 *  VSCEnveloppe.cpp
 *  EnveloppeEditor
 *
 *  Created by Jonathan Thorpe on 29/07/2011.
 *  Copyright 2011 JBAT. All rights reserved.
 *
 */

#include "VSCEnveloppe.h"
#include "VSCEnveloppePoint.h"

#include <cmath>

#define std::list<VSCEnveloppePoint*>::iterator POINT_ITERATOR
#define std::list<VSCEnveloppePoint*>::reverse_iterator REVERSE_POINT_ITERATOR

void VSCEnveloppe::setName(std::string name) {
	_name = name;
}

std::string VSCEnveloppe::getName(void) {
	return _name;
}

void VSCEnveloppe::setChannel(int channel) {
	_channel = channel;
}

int VSCEnveloppe::getChannel(void) {
	return channel;
}

void setMinimumTimeStep(double minimumTimeStep) {
	_minimumTimeStep = minimumTimeStep;
}

double getMinimumTimeStep(void) {
	return _minimumTimeStep;
}

void VSCEnveloppe::addPoint(VSCEnveloppePoint* point) {
	
	POINT_ITERATOR it;
	
	std::list<VSCEnveloppePoint*> pointsToRemove;
	
	// remove close points
	for (it = _points.begin(); it != _points.end(); it++) {
		if (abs(point->getTime() - (*it)->getTime()) < _minimumTimeStep) {
			pointsToRemove.push_back(*it);
		}
	}
	removePoints(pointsToRemove);
	
	bool found = false;
	
	for (it = _points.begin(); it != _points.end(); it++) {
		
		// if we already have the point in the enveloppe...
		if (point == *it) {
			return;
		}
		
		// is there a next element
		POINT_ITERATOR next = it + 1;
		if (next == _points.end()) {
			break;
		}
		
		// insert in the list if the time value is contained between this point's time and next point's time
		VSCEnveloppePoint* p = *it;
		VSCEnveloppePoint* np = *next;
		if (p->time < point->time && next->time > point->time) {
			found = true;
			break;
		}
	}
	
	if (found) {
		_points.insert(it, point);
	}
	
	else {
		_points.push_back(point);
	}
	
}

void VSCEnveloppe::removePoint(VSCEnveloppePoint* point) {
	
	removePoint(point, true);
	
}

void VSCEnveloppe::removePoint(VSCEnveloppePoint* point, bool clean) {
	
	if (clean) {
		
		for (POINT_ITERATOR it = _points.begin(); it != _points.end(); it++) {
			
			if (point == *it) {
				delete point;
				break;
			}
			
		}
		
	}
	
	_points.remove(point);
}

void VSCEnveloppe::addPoints(std::list<VSCEnveloppePoint*> points) {
	
	for (POINT_ITERATOR it = _points.begin(); it != _points.end(); it++) {
		addPoint(*it);
	}
	
}

void VSCEnveloppe::removePoints(std::list<VSCEnveloppePoint*> points) {
	for (POINT_ITERATOR it = _points.begin(); it != _points.end(); it++) {
		removePoint(*it);
	}
}

void VSCEnveloppe::removePoints(std::list<VSCEnveloppePoint*> points, bool clean) {
	for (POINT_ITERATOR it = _points.begin(); it != _points.end(); it++) {
		removePoint(*it, clean);
	}
	
}

void VSCEnveloppe::removePointsInTimeRange(double lowerTime, double upperTime){
	removePoints(getPointsInTimeRange(lower, higher));
}


/* get points */

VSCEnveloppePoint* VSCEnveloppe::getPointClosestToTime(double time) {
	getPointClosestToTime(time, true);
}

VSCEnveloppePoint* VSCEnveloppe::getPointClosestToTime(double time, bool copy) {
	
	if (_points.size() == 0) 
		return NULL;
	
	POINT_ITERATOR it = _points.begin();
	POINT_ITERATOR closestIt = _points.begin();
	
	double minimumTimeDifference = abs((*it)->getTime - time);
	
	it++;
	
	for (; it != _points.end(); it++) {
		
		double timeDifference = abs((*it)->getTime - time);
		
		if (timeDifference < minimumTimeDifference) {
			minimumTimeDifference = timeDifference;
			closestIt = it;
		}
		
	}
	
	if (copy)
		return new VSCEnveloppePoint(*closestIt);
		
	return (*closestIt);
	
}

VSCEnveloppePoint* VSCEnveloppe::getFirstPointAfterTime(double time) {
	getFirstPointAfterTime(time, true);
}

VSCEnveloppePoint* VSCEnveloppe::getFirstPointAfterTime(double time, bool copy) {
	
	if (_points.size() == 0) 
		return NULL;
	
	for (POINT_ITERATOR it = _points.begin(); it != _points.end(); it++) {
		
		if ((*it)->getTime() > time) {
			
			if (copy)
				return new VSCEnveloppePoint(*it);
			
			else 
				return (*it);
			
		}
		
	}
	
	return NULL;
	
}

VSCEnveloppePoint* VSCEnveloppe::getFirstPointBeforeTime(double time) {
	getFirstPointBeforeTime(time, true);
}

VSCEnveloppePoint* VSCEnveloppe::getFirstPointBeforeTime(double time, bool copy) {
	
	if (_points.size() == 0) 
		return NULL;
	
	for (REVERSE_POINT_ITERATOR it = _points.rbegin(); it != _points.rend(); it++) {
		
		if ((*it)->getTime*() < time) {
			
			if (copy)
				return new VSCEnveloppePoint(*it);
			
			else 
				return (*it);
			
		}
		
	}
	
	return NULL;
	
}

std::list<VSCEnveloppePoint*> VSCEnveloppe::getPointsInTimeRange(double lowerTime, double upperTime) {
	getPointsInTimeRange(lowerTime, upperTime, true);
}


std::list<VSCEnveloppePoint*> VSCEnveloppe::getPointsInTimeRange(double lowerTime, double upperTime, bool copy) {
	
	std::list<VSCEnveloppePoint*> ps;
	
	for (POINT_ITERATOR it = _points.begin(); it != _points.end(); it++) {
		
		if ((*it)->getTime() > lowerTime && (*it)->getTime() < upperTime) {
			if (copy)
				ps.push_back(new VSCEnveloppePoint(*it));
			else 
				ps.push_back(*it);
		}
		
	}

	return ps;
}

std::list<VSCEnveloppePoint*> VSCEnveloppe::getAllPoints(bool copy) {
}

int VSCEnveloppe::numberOfPoints(void) {
	return _points.size();
}


/* value */

double VSCEnveloppe::getValueAtTime(double time) {
	
	VSCEnveloppePoint* lp = getFirstPointBeforeTime(time, false);
	VSCEnveloppePoint* up = getFirstPointAfterTime(time, false);
	
	if (lp == NULL || up == NULL) 
		return NAN;
	/*
	if (lp == NULL)
		return up->getValue();
	if (up == NULL)
		return lp->getValue();
	 */
	
	double normDiff = (up->getTime() - lp->getTime()) / (time - lp->getTime()); 
	
	return lp->getValue() + ((up->getValue() - lp->getValue()) * normDiff);

}

/* sorting */

void VSCEnveloppe::sortPointsByTime(void) {
	
	_points.sort(compareEnveloppePointTimes);
	
}

