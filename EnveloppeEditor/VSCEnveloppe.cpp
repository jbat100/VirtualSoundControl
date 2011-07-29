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
	
	if (_points.size() == 0) {
		return NULL;
	}
	
	POINT_ITERATOR it = _points.begin();
	
	if (_points.size() == 1) {
		return *it;
	}
	
	double minimumTimeDifference = abs((*it)->getTime - time);
	
	it++;
	
	for (; it != _points.end(); it++) {
		
		
		
	}
	
}

VSCEnveloppePoint* VSCEnveloppe::getFirstPointAfterTime(double time) {
}

VSCEnveloppePoint* VSCEnveloppe::getFirstPointBeforeTime(double time) {
}

std::list<VSCEnveloppePoint*> VSCEnveloppe::getPointsInTimeRange(double lower, double higher) {
}

std::list<VSCEnveloppePoint*> VSCEnveloppe::getAllPoints(void) {
}

int VSCEnveloppe::numberOfPoints(void) {
}


/* value */

double VSCEnveloppe::getValueAtTime(double time) {
}

/* sorting */

void VSCEnveloppe::sortPointsByTime(void) {
}

