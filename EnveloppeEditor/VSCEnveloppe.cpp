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

void VSCEnveloppe::setName(std::string name) {
	_name = name;
}

std::string VSCEnveloppe::getName(void) const {
	return _name;
}

void VSCEnveloppe::setChannel(int channel) {
	_channel = channel;
}

int VSCEnveloppe::getChannel(void) const {
	return _channel;
}

void VSCEnveloppe::setMinimumTimeStep(double minimumTimeStep) {
	_minimumTimeStep = minimumTimeStep;
}

double VSCEnveloppe::getMinimumTimeStep(void) const {
	return _minimumTimeStep;
}

void VSCEnveloppe::addPoint(VSCEnveloppePointPtr point) {
	
	POINT_ITERATOR it;
	
	std::list<VSCEnveloppePointPtr> pointsToRemove;
	
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
		POINT_ITERATOR next = it;
        next++;
		if (next == _points.end()) {
			break;
		}
		
		VSCEnveloppePointPtr p(*it);
		VSCEnveloppePointPtr np(*next);
		
		// insert in the list if the time value is contained between this point's time and next point's time
		if (p->getTime() < point->getTime() && np->getTime() > point->getTime()) {
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

void VSCEnveloppe::removePoint(VSCEnveloppePointPtr point) {
	_points.remove(point);
}

void VSCEnveloppe::addPoints(std::list<VSCEnveloppePointPtr> points) {
	for (POINT_ITERATOR it = _points.begin(); it != _points.end(); it++) {
		addPoint(*it);
	}
}

void VSCEnveloppe::removePoints(std::list<VSCEnveloppePointPtr> points) {
	for (POINT_ITERATOR it = _points.begin(); it != _points.end(); it++) {
		removePoint(*it);
	}
}

void VSCEnveloppe::removePointsInTimeRange(double lowerTime, double upperTime){
	removePoints(getPointsInTimeRange(lowerTime, upperTime));
}


/* get points */

VSCEnveloppePointPtr VSCEnveloppe::getPointClosestToTime(double time) const {
	return getPointClosestToTime(time, false);
}

VSCEnveloppePointPtr VSCEnveloppe::getPointClosestToTime(double time, bool copy) const {
	
	if (_points.size() == 0) 
		return NULL;
	
	CONST_POINT_ITERATOR it = _points.begin();
	CONST_POINT_ITERATOR closestIt = _points.begin();
	
	double minimumTimeDifference = abs((*it)->getTime() - time);
	
	it++;
	
	for (; it != _points.end(); it++) {
		
		double timeDifference = abs((*it)->getTime() - time);
		
		if (timeDifference < minimumTimeDifference) {
			minimumTimeDifference = timeDifference;
			closestIt = it;
		}
		
	}
	
	if (copy)
		return VSCEnveloppePointPtr(new VSCEnveloppePoint(*(*closestIt)));
		
	return (*closestIt);
	
}

VSCEnveloppePointPtr VSCEnveloppe::getFirstPointAfterTime(double time) const {
	return getFirstPointAfterTime(time, false);
}

VSCEnveloppePointPtr VSCEnveloppe::getFirstPointAfterTime(double time, bool copy) const {
	
	if (_points.size() == 0) 
		return NULL;
	
	for (CONST_POINT_ITERATOR it = _points.begin(); it != _points.end(); it++) {
		
		if ((*it)->getTime() > time) {
			
			if (copy)
				return new VSCEnveloppePoint(*(*it));
			
			else 
				return (*it);
			
		}
		
	}
	
	return NULL;
	
}

VSCEnveloppePointPtr VSCEnveloppe::getFirstPointBeforeTime(double time) const {
	return getFirstPointBeforeTime(time, false);
}

VSCEnveloppePointPtr VSCEnveloppe::getFirstPointBeforeTime(double time, bool copy) const {
	
	if (_points.size() == 0) 
		return NULL;
	
	for (CONST_REVERSE_POINT_ITERATOR it = _points.rbegin(); it != _points.rend(); it++) {
		
		if ((*it)->getTime() < time) {
			
			if (copy)
				return new VSCEnveloppePoint(*(*it));
			
			else 
				return (*it);
			
		}
		
	}
	
	return NULL;
	
}

std::list<VSCEnveloppePointPtr> VSCEnveloppe::getPointsInTimeRange(double lowerTime, double upperTime) const {
	return getPointsInTimeRange(lowerTime, upperTime, false);
}


std::list<VSCEnveloppePointPtr> VSCEnveloppe::getPointsInTimeRange(double lowerTime, double upperTime, bool copy) const {
	
	std::list<VSCEnveloppePointPtr> ps;
	
	for (CONST_POINT_ITERATOR it = _points.begin(); it != _points.end(); it++) {
		
		if ((*it)->getTime() > lowerTime && (*it)->getTime() < upperTime) {
			if (copy)
				ps.push_back(new VSCEnveloppePoint(*(*it)));
			else 
				ps.push_back(*it);
		}
		
	}

	return ps;
}

std::list<VSCEnveloppePointPtr> VSCEnveloppe::getAllPoints(void) const {
    return getAllPoints(false);
}

std::list<VSCEnveloppePointPtr> VSCEnveloppe::getAllPoints(bool copy) const {
    
    if (copy) {
        
        std::list<VSCEnveloppePointPtr> copiedPoints;
        
        for (CONST_POINT_ITERATOR it = _points.begin(); it != _points.end(); it++) {
            VSCEnveloppePointPtr newPoint (new VSCEnveloppePoint(*(*it)));
            copiedPoints.push_back(newPoint);
        }
        
        return copiedPoints;
        
    }
    
    return _points;
    
}

int VSCEnveloppe::numberOfPoints(void) const {
	return _points.size();
}


/* value */

double VSCEnveloppe::getValueAtTime(double time) const {
	
	VSCEnveloppePointPtr lp = getFirstPointBeforeTime(time, false);
	VSCEnveloppePointPtr up = getFirstPointAfterTime(time, false);
	
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

