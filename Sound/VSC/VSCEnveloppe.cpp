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
#include "VSCException.h"

#include <cmath>
#include <assert.h>

#include <boost/filesystem.hpp>


void VSCEnveloppe::fire(void) {
	
}

void VSCEnveloppe::fireAfterInterval(double intervalTime) {
	
}

void VSCEnveloppe::setCurrentTime(double currentTime) {
	
}

#pragma mark - General Setters/Getters

void VSCEnveloppe::setScaleType(VSCEnveloppe::ScaleType scaleType) {
	_scaleType = scaleType;
}

VSCEnveloppe::ScaleType VSCEnveloppe::getScaleType(void) const {
	return _scaleType;
}

void VSCEnveloppe::setCurveType(VSCEnveloppe::CurveType curveType) {
	_curveType = curveType;
}

VSCEnveloppe::CurveType VSCEnveloppe::getCurveType(void) const {
	return _curveType;
}

void VSCEnveloppe::setState(VSCEnveloppe::State state) {
	_state = state;
}

VSCEnveloppe::State VSCEnveloppe::getState(void) const {
	return _state;
}

void VSCEnveloppe::setRelativePath(std::string relativePath) {
	_relativePath = relativePath;
}

std::string VSCEnveloppe::getRelativePath(void) const {
	return _relativePath;
}

std::string VSCEnveloppe::getName(void) const {
	return boost::filesystem::path(_relativePath).replace_extension(NULL).filename().string();
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

bool VSCEnveloppe::isSortedByTime(void) const {
    
    if (_points.size() < 1) 
        return true;
    
    ConstEnvPntIter it = _points.begin();
    
    double currentTime = (*it)->getTime();
    
    for (; it != _points.end(); it++) {
        if ((*it)->getTime() < currentTime) {
            return false;
        }
        currentTime = (*it)->getTime();
    }
    
    return true;
    
}

#pragma mark - Add/Remove Enveloppe Points

void VSCEnveloppe::addPoint(VSCEnveloppePointPtr point) {
    
    assert(point);
	
	std::cout << "\nAdding " << *(point.get());
	
	EnvPntIter it;
	
	std::list<VSCEnveloppePointPtr> pointsToRemove;
	
	// remove close points
	for (it = _points.begin(); it != _points.end(); it++) {
		if (abs(point->getTime() - (*it)->getTime()) < _minimumTimeStep) {
			std::cout << "\nFound point closer than minimum time step (" << _minimumTimeStep << ") :";
			std::cout << *((*it).get());
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
		EnvPntIter next = it;
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
    
    assert(isSortedByTime());
	
}

void VSCEnveloppe::removePoint(VSCEnveloppePointPtr point) {
	assert(point);
	std::cout << "\nIn removePoint: " << *(point.get());
	_points.remove(point);
    assert(isSortedByTime());
}

void VSCEnveloppe::addPoints(std::list<VSCEnveloppePointPtr>& points) {
	for (EnvPntIter it = _points.begin(); it != _points.end(); it++) {
		addPoint(*it);
	}
    assert(isSortedByTime());
}

void VSCEnveloppe::removePoints(std::list<VSCEnveloppePointPtr>& pnts) {
	std::cout << "\nIn removePoints";
	if (pnts.size() == 0) {
		std::cout << ", no points to remove";
	}
	for (EnvPntIter it = pnts.begin(); it != pnts.end(); it++) {
		removePoint(*it);
	}
    assert(isSortedByTime());
}

void VSCEnveloppe::removePointsInTimeRange(double lowerTime, double upperTime){
	std::list<VSCEnveloppePointPtr> l;
	getPointsInTimeRange(l, lowerTime, upperTime);
	removePoints(l);
    assert(isSortedByTime());
}

#pragma mark - Change Notification

/*
 *	Enveloppe changes calls (mostly for subclasses to update cache tables)
 */
void VSCEnveloppe::enveloppeChangedBetweenEnveloppePoints(VSCEnveloppePointPtr begin, VSCEnveloppePointPtr end) {
	std::cout << "In VSCEnveloppe enveloppeChangedBetweenEnveloppePoints";
}

void VSCEnveloppe::enveloppeChangedBetweenEnveloppePointAndNext(VSCEnveloppePointPtr point) {
	std::cout << "In VSCEnveloppe enveloppeChangedBetweenEnveloppePointAndNext";
}

#pragma mark - Point Getters

EnvPntIter VSCEnveloppe::getPointBeginIterator(void) {
	return _points.begin();
}

EnvPntIter VSCEnveloppe::getPointEndIterator(void) {
	return _points.end();
}

ConstEnvPntIter VSCEnveloppe::getPointBeginConstIterator(void) const {
	return _points.begin();
}
ConstEnvPntIter VSCEnveloppe::getPointEndConstIterator(void) const {
	return _points.end();
}

/* get points */

VSCEnveloppePointPtr VSCEnveloppe::getPointClosestToTime(double time) const {
	return getPointClosestToTime(time, false);
}

VSCEnveloppePointPtr VSCEnveloppe::getPointClosestToTime(double time, bool copy) const {
	
	if (_points.size() == 0) 
		return VSCEnveloppePointPtr();
	
	ConstEnvPntIter it = _points.begin();
	ConstEnvPntIter closestIt = _points.begin();
	
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
	
	for (ConstEnvPntIter it = _points.begin(); it != _points.end(); it++) {
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
	
	for (ConstRevEnvPntIter it = _points.rbegin(); it != _points.rend(); it++) {
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
	
	for (ConstEnvPntIter it = _points.begin(); it != _points.end(); it++) {
		
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
        for (ConstEnvPntIter it = _points.begin(); it != _points.end(); it++) {
            VSCEnveloppePointPtr newPoint (new VSCEnveloppePoint(*(*it)));
            l.push_back(newPoint);
        }
    }
    
    else {
		for (ConstEnvPntIter it = _points.begin(); it != _points.end(); it++) {
            l.push_back(*it);
        }
	}
    
}

int VSCEnveloppe::numberOfPoints(void) const {
	return _points.size();
}

#pragma mark - Enveloppe Duration 

double VSCEnveloppe::duration(void) const {
	
	assert(isSortedByTime());
	
	/* 
	 * If there are no points then return 0 duration 
	 */
	if (numberOfPoints() < 1)
		return 0.0;
	
	/* 
	 * Get last pointand return its time value
	 */
	ConstRevEnvPntIter rit = _points.rbegin();
	//rit--;
	
	return (*rit)->getTime();
	
}

#pragma mark - Value Computation

/* value */

double VSCEnveloppe::getValueAtTime(double time) const {
	
	VSCEnveloppePointPtr lp = getFirstPointBeforeTime(time, false);
	VSCEnveloppePointPtr up = getFirstPointAfterTime(time, false);
	//VSCEnveloppePointPtr up = lp;
	//up++;
	
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

#pragma mark - Sorting

/* sorting */

void VSCEnveloppe::sortPointsByTime(void) {
	
	//_points.sort(compareEnveloppePointTimes);
	
}

#pragma mark - Enveloppe Extremes

/* extremes */
double VSCEnveloppe::minTime(void) const {
    
    assert(isSortedByTime());
    ConstEnvPntIter beginIter = getPointBeginConstIterator();
    ConstEnvPntIter endIter = getPointEndConstIterator();
    if (beginIter != endIter) return (*beginIter)->getTime();
    throw VSCEnveloppeEmptyException();
    
}

double VSCEnveloppe::maxTime(void) const {
    
    assert(isSortedByTime());
    ConstEnvPntIter beginIter = getPointBeginConstIterator();
    ConstEnvPntIter endIter = getPointEndConstIterator();
    if (beginIter != endIter) {
        endIter--;
        return (*endIter)->getTime();
    }
    throw VSCEnveloppeEmptyException();
    
}

double VSCEnveloppe::minValue(void) const {
    
    ConstEnvPntIter beginIter = getPointBeginConstIterator();
    ConstEnvPntIter endIter = getPointEndConstIterator();
    if (beginIter == endIter) throw VSCEnveloppeEmptyException();
    
    double minValue = (*beginIter)->getValue();
    for (ConstEnvPntIter it = beginIter; it != endIter; it++) {
        if ((*it)->getValue() < minValue) {
            minValue = (*it)->getValue();
        }
    }
    
    return minValue;
}

double VSCEnveloppe::maxValue(void) const {
    
    ConstEnvPntIter beginIter = getPointBeginConstIterator();
    ConstEnvPntIter endIter = getPointEndConstIterator();
    if (beginIter == endIter) throw VSCEnveloppeEmptyException();
    
    double maxValue = (*beginIter)->getValue();
    for (ConstEnvPntIter it = beginIter; it != endIter; it++) {
        if ((*it)->getValue() < maxValue) {
            maxValue = (*it)->getValue();
        }
    }
    
    return maxValue;
}

std::ostream& operator<<(std::ostream& output, VSCEnveloppe& p) {
	output << "VSCEnveloppe with points:";
	for (EnvPntIter pntIt = p.getPointBeginIterator(); pntIt != p.getPointEndIterator(); pntIt++) {
		output << "\n	" << *((*pntIt).get());
	}
	return output;
}

