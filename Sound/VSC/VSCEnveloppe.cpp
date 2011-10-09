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

#include <fstream>
#include <cmath>
#include <assert.h>

#include <boost/filesystem.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/nvp.hpp>

#pragma mark - Save/Load

/*
 *	Saving and loading
 */

void saveVSCEnveloppeToXML(const VSCEnveloppe &s, const char * filename) {
    // make an archive
    std::ofstream ofs(filename);
    assert(ofs.good());
    boost::archive::xml_oarchive oa(ofs);
    oa << boost::serialization::make_nvp("enveloppe", s);
}

void loadVSCEnveloppeFromXML(VSCEnveloppe &s, const char * filename) {
    // open the archive
    std::ifstream ifs(filename);
    assert(ifs.good());
    boost::archive::xml_iarchive ia(ifs);
    // restore the schedule from the archive
    ia >> boost::serialization::make_nvp("enveloppe", s);
}

#pragma mark - Constructor/Destructor/Defaults

VSCEnveloppe::VSCEnveloppe(void) {
	
	setToDefault();
	
}

VSCEnveloppe::~VSCEnveloppe(void) {
	std::cout << "Destroying Enveloppe!" << std::endl;
}

void VSCEnveloppe::setToDefault(void) {
	
	setScaleType(ScaleTypeLinear);
	setCurveType(CurveTypeLinear);
	setState(StateOff);
	setRelativePath("");
	setChannel(0);
	setMinimumTimeStep(0.01);
	removeAllPoints();
	
}

#pragma mark - Audio stuff

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

void VSCEnveloppe::setPointDisplacementConflictResolution(VSCEnveloppe::PointDisplacementConflictResolution r) {
	_pointDisplacementConflictResolution = r;
}

VSCEnveloppe::PointDisplacementConflictResolution VSCEnveloppe::getPointDisplacementConflictResolution(void) const {
	return _pointDisplacementConflictResolution;
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
	
	//std::cout << "Adding " << *(point.get()) << "\n";
	
	EnvPntIter currentIt;
	EnvPntIter nextIt;
	
	std::list<VSCEnveloppePointPtr> pointsToRemove;
	
	// remove close points
	for (currentIt = _points.begin(); currentIt != _points.end(); currentIt++) {
		double difference = std::abs(point->getTime() - (*currentIt)->getTime());
		if (difference < _minimumTimeStep) {
			//std::cout << "Found point " << *(*currentIt) << " closer than minimum time step (" << _minimumTimeStep << ") ";
			//std::cout << "by " << difference << "\n";
			pointsToRemove.push_back(*currentIt);
		}
	}
	removePoints(pointsToRemove);
	
	bool found = false;
	
	for (currentIt = _points.begin(); currentIt != _points.end(); currentIt++) {
		
		// if we already have the point in the enveloppe...
		if (point == *currentIt) {
			return;
		}
		
		// is there a next element
		nextIt = currentIt;
        nextIt++;
		if (nextIt == _points.end()) {
			break;
		}
		
		VSCEnveloppePointPtr p(*currentIt);
		VSCEnveloppePointPtr np(*nextIt);
		
		// insert in the list if the time value is contained between this point's time and next point's time
		if (p->getTime() < point->getTime() && np->getTime() > point->getTime()) {
			found = true;
			break;
		}
	}
	
	if (found) {
		_points.insert(nextIt, point);
	}
	
	else {
		_points.push_back(point);
	}
	
	//std::cout << "After point add, enveloppe is: " << *this << "\n";
    
    assert(isSortedByTime());
	
}

void VSCEnveloppe::removePoint(VSCEnveloppePointPtr point) {
	assert(point);
	//std::cout << "In removePoint: " << *(point.get()) << "\n";
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
	//std::cout << "In removePoints";
	if (pnts.size() == 0) {
		//std::cout << ", no points to remove\n";
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

void VSCEnveloppe::removeAllPoints(void) {
	_points.clear();
}

#pragma mark - Change Notification

/*
 *	Enveloppe changes calls (mostly for subclasses to update cache tables)
 */
void VSCEnveloppe::enveloppeChangedBetweenEnveloppePoints(VSCEnveloppePointPtr begin, VSCEnveloppePointPtr end) {
	std::cout << "In VSCEnveloppe enveloppeChangedBetweenEnveloppePoints " << *begin << " " << *end;
}

void VSCEnveloppe::enveloppeChangedBetweenEnveloppePointAndNext(VSCEnveloppePointPtr point) {
	std::cout << "In VSCEnveloppe enveloppeChangedBetweenEnveloppePointAndNext " << *point;
}

void VSCEnveloppe::enveloppeChanged(void) {
	std::cout << "In VSCEnveloppe enveloppeChanged";
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
	
	double minimumTimeDifference = std::abs((*it)->getTime() - time);
	
	it++;
	
	for (; it != _points.end(); it++) {
		
		double timeDifference = std::abs((*it)->getTime() - time);
		
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

#pragma mark - Point Displacement 

bool VSCEnveloppe::canDisplacePointTime(ConstEnvPntIter pointIt, double deltaTime) {
	
	assert(pointIt != _points.end());
	assert(find(_points.begin(), _points.end(), *pointIt) != _points.end());
	
	VSCEnveloppePointPtr point = *pointIt;
	VSCEnveloppePointPtr nextPoint;
	VSCEnveloppePointPtr previousPoint;
	
	ConstEnvPntIter nextPointIt = pointIt;
	nextPointIt++;
	ConstEnvPntIter previousPointIt = pointIt;
	if (pointIt == _points.begin())
		previousPointIt = _points.end();
	else 
		previousPointIt--;
	
	if (pointIt == _points.begin()) {
		if (point->getTime() - deltaTime < 0.0) 
			return false;
	}
	
	if (nextPointIt != _points.end()) {
		if (point->getTime()+deltaTime > nextPoint->getTime()) 
			return false;
	}
	
	if (previousPointIt != _points.end()) {
		if (point->getTime()+deltaTime < previousPoint->getTime()) 
			return false;
	}
	
	return true;
	
}

bool VSCEnveloppe::canDisplacePointValue(ConstEnvPntIter pointIt, double deltaValue) {
	
	return true;
	
}

void VSCEnveloppe::displacePoint(ConstEnvPntIter pointIt, double deltaTime, double deltaValue) {
	

	
}

void VSCEnveloppe::displacePoint(VSCEnveloppePointPtr point, double deltaTime, double deltaValue) {
	
	
	
}

void VSCEnveloppe::displacePoints(std::list<VSCEnveloppePointPtr>& pts, double deltaTime, double deltaValue) {
	
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

