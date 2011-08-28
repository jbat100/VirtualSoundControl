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
#include "VSCBoost.h"

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
	
	ENVPNT_ITER it;
	
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
		ENVPNT_ITER next = it;
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

void VSCEnveloppe::addPoints(std::list<VSCEnveloppePointPtr>& points) {
	for (ENVPNT_ITER it = _points.begin(); it != _points.end(); it++) {
		addPoint(*it);
	}
}

void VSCEnveloppe::removePoints(std::list<VSCEnveloppePointPtr>& points) {
	for (ENVPNT_ITER it = _points.begin(); it != _points.end(); it++) {
		removePoint(*it);
	}
}

void VSCEnveloppe::removePointsInTimeRange(double lowerTime, double upperTime){
	std::list<VSCEnveloppePointPtr> l;
	getPointsInTimeRange(l, lowerTime, upperTime);
	removePoints(l);
}


CONST_ENVPNT_ITER VSCEnveloppe::getPointBeginIterator(void) const {
	return _points.begin();
}

CONST_ENVPNT_ITER VSCEnveloppe::getPointEndIterator(void) const {
	return _points.end();
}

/* get points */

VSCEnveloppePointPtr VSCEnveloppe::getPointClosestToTime(double time) const {
	return getPointClosestToTime(time, false);
}

VSCEnveloppePointPtr VSCEnveloppe::getPointClosestToTime(double time, bool copy) const {
	
	if (_points.size() == 0) 
		return VSCEnveloppePointPtr();
	
	CONST_ENVPNT_ITER it = _points.begin();
	CONST_ENVPNT_ITER closestIt = _points.begin();
	
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
		return VSCEnveloppePointPtr();
	
	for (CONST_ENVPNT_ITER it = _points.begin(); it != _points.end(); it++) {
		
		if ((*it)->getTime() > time) {
			
			if (copy)
				return VSCEnveloppePointPtr(new VSCEnveloppePoint(*(*it)));
			
			else 
				return (*it);
			
		}
		
	}
	
	return VSCEnveloppePointPtr();
	
}

VSCEnveloppePointPtr VSCEnveloppe::getFirstPointBeforeTime(double time) const {
	return getFirstPointBeforeTime(time, false);
}

VSCEnveloppePointPtr VSCEnveloppe::getFirstPointBeforeTime(double time, bool copy) const {
	
	if (_points.size() == 0) 
		return VSCEnveloppePointPtr();
	
	for (CONST_REV_ENVPNT_ITER it = _points.rbegin(); it != _points.rend(); it++) {
		
		if ((*it)->getTime() < time) {
			
			if (copy)
				return VSCEnveloppePointPtr(new VSCEnveloppePoint(*(*it)));
			
			else 
				return (*it);
			
		}
		
	}
	
	return VSCEnveloppePointPtr();
	
}

void VSCEnveloppe::getPointsInTimeRange(std::list<VSCEnveloppePointPtr>& l, double lowerTime, double upperTime) const {
	getPointsInTimeRange(l, lowerTime, upperTime, false);
}


void VSCEnveloppe::getPointsInTimeRange(std::list<VSCEnveloppePointPtr>& l, double lowerTime, double upperTime, bool copy) const {
	
	for (CONST_ENVPNT_ITER it = _points.begin(); it != _points.end(); it++) {
		
		if ((*it)->getTime() > lowerTime && (*it)->getTime() < upperTime) {
			if (copy)
				l.push_back(VSCEnveloppePointPtr(new VSCEnveloppePoint(*(*it))));
			else 
				l.push_back(*it);
		}
		
	}

}

void VSCEnveloppe::getAllPoints(std::list<VSCEnveloppePointPtr>& l) const {
    getAllPoints(l, false);
}

void VSCEnveloppe::getAllPoints(std::list<VSCEnveloppePointPtr>& l, bool copy) const {
    
    if (copy) {
        
        for (CONST_ENVPNT_ITER it = _points.begin(); it != _points.end(); it++) {
            VSCEnveloppePointPtr newPoint (new VSCEnveloppePoint(*(*it)));
            l.push_back(newPoint);
        }
        
    }
    
    else {
		
		for (CONST_ENVPNT_ITER it = _points.begin(); it != _points.end(); it++) {
            l.push_back(*it);
        }
		
	}
    
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
	
	//_points.sort(compareEnveloppePointTimes);
	
}

