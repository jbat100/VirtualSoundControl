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
#include "VSCSound.h"

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

void saveVSCEnveloppeToXML(const VSCEnveloppe &s, const char * filepath) {
    // make an archive
    std::ofstream ofs(filepath);
    assert(ofs.good());
    boost::archive::xml_oarchive oa(ofs);
    oa << boost::serialization::make_nvp("enveloppe", s);
}

void loadVSCEnveloppeFromXML(VSCEnveloppe &s, const char * filepath) {
    // open the archive
    std::ifstream ifs(filepath);
    assert(ifs.good());
    boost::archive::xml_iarchive ia(ifs);
    // restore the schedule from the archive
    ia >> boost::serialization::make_nvp("enveloppe", s);
}

#pragma mark - Constructor/Destructor/Defaults

VSCEnveloppe::VSCEnveloppe(void) {
	_minimumTimeStep = 0.01;
    _curveType = CurveTypeLinear;
    _pointDisplacementConflictResolution = PointDisplacementConflictResolutionClear;
}

VSCEnveloppe::~VSCEnveloppe(void) {
	std::cout << "Destroying Enveloppe!" << std::endl;
}



#pragma mark - General Setters/Getters


void VSCEnveloppe::setCurveType(VSCEnveloppe::CurveType curveType) {
	_curveType = curveType;
}

VSCEnveloppe::CurveType VSCEnveloppe::getCurveType(void) const {
	return _curveType;
}


void VSCEnveloppe::setPointDisplacementConflictResolution(VSCEnveloppe::PointDisplacementConflictResolution r) {
	_pointDisplacementConflictResolution = r;
}

VSCEnveloppe::PointDisplacementConflictResolution VSCEnveloppe::getPointDisplacementConflictResolution(void) const {
	return _pointDisplacementConflictResolution;
}

void VSCEnveloppe::setFilePath(std::string filePath) {
	_filePath = filePath;
}

std::string VSCEnveloppe::getFilePath(void) const {
	return _filePath;
}

std::string VSCEnveloppe::getName(void) const {
	return boost::filesystem::path(_filePath).replace_extension(NULL).filename().string();
}

void VSCEnveloppe::setMinimumTimeStep(VSCSFloat minimumTimeStep) {
	_minimumTimeStep = minimumTimeStep;
}

VSCSFloat VSCEnveloppe::getMinimumTimeStep(void) const {
	return _minimumTimeStep;
}

bool VSCEnveloppe::isSortedByTime(void) const {
    
    if (_points.size() < 1) 
        return true;
    
    PointIterator it = _points.begin();
    VSCSFloat currentTime = (*it)->getTime();
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
    // check if the point already is in the enveloppe, no dupicate pointers please ...
	
	PointIterator currentIt;
	PointIterator nextIt;
	
	PointList pointsToRemove;
	
	// remove close points
	for (currentIt = _points.begin(); currentIt != _points.end(); currentIt++) {
		VSCSFloat difference = std::abs(point->getTime() - (*currentIt)->getTime());
		if (difference < _minimumTimeStep) {
			//std::cout << "Found point " << *(*currentIt) << " closer than minimum time step (" << _minimumTimeStep << ") ";
			//std::cout << "by " << difference << "\n";
			pointsToRemove.push_back(*currentIt);
		}
	}
	removePoints(pointsToRemove);
	
    // if the time is less than that of the first point in the enveloppe then add at the beginning
    currentIt = _points.begin();
    if ((*currentIt)->getTime() > point->getTime()) {
        _points.insert(currentIt, point);
    }
    // otherwise find the right spot to insert the point, keeping the points ordered by time 
    else {
        bool found = false;
        for (currentIt = _points.begin(); currentIt != _points.end(); currentIt++) {
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
        if (found) { // if we found a previous and next point
            _points.insert(nextIt, point);
        }
        else { // else push back at the end
            _points.push_back(point);
        }
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

void VSCEnveloppe::addPoints(PointList& points) {
	for (PointIterator it = _points.begin(); it != _points.end(); it++) {
		addPoint(*it);
	}
    assert(isSortedByTime());
}

void VSCEnveloppe::removePoints(PointList& pnts) {
	//std::cout << "In removePoints";
	if (pnts.size() == 0) {
		//std::cout << ", no points to remove\n";
	}
	for (PointIterator it = pnts.begin(); it != pnts.end(); it++) {
		this->removePoint(*it);
	}
    assert(isSortedByTime());
}

void VSCEnveloppe::removePointsInTimeRange(TimeRange range){
	PointList l;
	this->getPointsInTimeRange(l, range);
	this->removePoints(l);
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

VSCEnveloppe::PointIterator VSCEnveloppe::getPointBeginIterator(void) {
	return _points.begin();
}

VSCEnveloppe::PointIterator VSCEnveloppe::getPointEndIterator(void) {
	return _points.end();
}

VSCEnveloppe::ConstPointIterator VSCEnveloppe::getPointBeginConstIterator(void) const {
	return _points.begin();
}

VSCEnveloppe::ConstPointIterator VSCEnveloppe::getPointEndConstIterator(void) const {
	return _points.end();
}

/* get points */

VSCEnveloppePointPtr VSCEnveloppe::getPointClosestToTime(VSCSFloat time) const {
	
	if (_points.size() == 0) 
		return VSCEnveloppePointPtr();
	
	ConstPointIterator it = _points.begin();
	ConstPointIterator closestIt = _points.begin();
	
	VSCSFloat minimumTimeDifference = std::abs((*it)->getTime() - time);
	
	it++;
	
	for (; it != _points.end(); it++) {
		
		VSCSFloat timeDifference = std::abs((*it)->getTime() - time);
		
		if (timeDifference < minimumTimeDifference) {
			minimumTimeDifference = timeDifference;
			closestIt = it;
		}
		
	}
		
	return (*closestIt);
	
}

VSCEnveloppePointPtr VSCEnveloppe::getFirstPointAfterTime(VSCSFloat time) const {
	
	if (_points.size() == 0) 
		return VSCEnveloppePointPtr();
	
	for (ConstPointIterator it = _points.begin(); it != _points.end(); it++) {
		if ((*it)->getTime() > time) {
			return (*it);
		}
	}
	
	return VSCEnveloppePointPtr();
	
}

VSCEnveloppePointPtr VSCEnveloppe::getFirstPointBeforeTime(VSCSFloat time) const {

	if (_points.size() == 0) 
		return VSCEnveloppePointPtr();
	
	for (ConstRevEnvPntIter it = _points.rbegin(); it != _points.rend(); it++) {
		if ((*it)->getTime() < time) {
			return (*it);
		}
	}
	
	return VSCEnveloppePointPtr();
	
}

void VSCEnveloppe::getPointsInTimeRange(PointList& l, TimeRange range) const {
	
	for (ConstEnvPntIter it = _points.begin(); it != _points.end(); it++) {
		if ((*it)->getTime() > range.first && (*it)->getTime() < range.second) {
			l.push_back(*it);
		}
	}

}

void VSCEnveloppe::getAllPoints(PointList& l) const {
    for (ConstPointIterator it = _points.begin(); it != _points.end(); it++) {
        l.push_back(*it);
    }
}

int VSCEnveloppe::numberOfPoints(void) const {
	return (int)_points.size();
}

#pragma mark - Point Displacement 

// this method is necessary in order to validate that all displacements are possible when displacePoints is performed
bool canDisplacePoint(VSCEnveloppePointPtr point, VSCSFloat deltaTime, VSCSFloat deltaValue) const {
    
    if (_pointDisplacementConflictResolution == PointDisplacementConflictResolutionNone) {
        return true;
    }
    
    else if (_pointDisplacementConflictResolution == PointDisplacementConflictResolutionClear) {
        return true;
    }
    
    else if (_pointDisplacementConflictResolution == PointDisplacementConflictResolutionBlock) {
        
        throw VSCNotImplementedException();
        
        /*
        
        PointIterator pointIt = std::find(_points.begin(), _points.end(), point);
        if (pointIt == _points.end()) {
            throw VSCSInvalidArgumentException();
        }
        
        if (pointIt == _points.begin()) {
            PointIterator nextIt = pointIt;
            nextIt++;
        }
        

        
        */
    }
    
    throw VSCSInvalidArgumentException();
    
}

bool VSCEnveloppe::displacePoint(PointIterator pointIt, VSCSFloat deltaTime, VSCSFloat deltaValue) {
	

	
}

bool VSCEnveloppe::displacePoint(VSCEnveloppePointPtr point, VSCSFloat deltaTime, VSCSFloat deltaValue) {
	
	
	
}

bool VSCEnveloppe::displacePoints(PointList& pts, VSCSFloat deltaTime, VSCSFloat deltaValue) {
	
}

#pragma mark - Enveloppe Duration 

VSCSFloat VSCEnveloppe::duration(void) const {
	
	assert(isSortedByTime());
	
	/* 
	 * If there are no points then return 0 duration 
	 */
	if (numberOfPoints() < 1)
		return 0.0;
	
	/* 
	 * Get last pointand return its time value
	 */
	ConstPointIterator rit = _points.rbegin();  
	//rit--;
	
	return (*rit)->getTime();
	
}

#pragma mark - Value Computation

/* value */

VSCSFloat VSCEnveloppe::getValueAtTime(VSCSFloat time) const {
	
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
	
	VSCSFloat normDiff = (up->getTime() - lp->getTime()) / (time - lp->getTime()); 
	
	return lp->getValue() + ((up->getValue() - lp->getValue()) * normDiff);

}

#pragma mark - Sorting

/* sorting */

void VSCEnveloppe::sortPointsByTime(void) {
	
	//_points.sort(compareEnveloppePointTimes);
	
}

#pragma mark - Enveloppe Extremes

/* extremes */
VSCSFloat VSCEnveloppe::minTime(void) const {
    assert(isSortedByTime());
    ConstPointIterator beginIter = getPointBeginConstIterator();
    ConstPointIterator endIter = getPointEndConstIterator();
    if (beginIter != endIter) return (*beginIter)->getTime();
    throw VSCEnveloppeEmptyException();
}

VSCSFloat VSCEnveloppe::maxTime(void) const {
    assert(isSortedByTime());
    ConstPointIterator beginIter = getPointBeginConstIterator();
    ConstPointIterator endIter = getPointEndConstIterator();
    if (beginIter != endIter) {
        endIter--;
        return (*endIter)->getTime();
    }
    throw VSCEnveloppeEmptyException();
}

VSCSFloat VSCEnveloppe::minValue(void) const {
    ConstPointIterator beginIter = getPointBeginConstIterator();
    ConstPointIterator endIter = getPointEndConstIterator();
    if (beginIter == endIter) throw VSCEnveloppeEmptyException();
    VSCSFloat minValue = (*beginIter)->getValue();
    for (ConstPointIterator it = beginIter; it != endIter; it++) {
        if ((*it)->getValue() < minValue) {
            minValue = (*it)->getValue();
        }
    }
    return minValue;
}

VSCSFloat VSCEnveloppe::maxValue(void) const {
    ConstPointIteratorbeginIter = getPointBeginConstIterator();
    ConstPointIterator endIter = getPointEndConstIterator();
    if (beginIter == endIter) throw VSCEnveloppeEmptyException();
    VSCSFloat maxValue = (*beginIter)->getValue();
    for (ConstPointIterator it = beginIter; it != endIter; it++) {
        if ((*it)->getValue() < maxValue) {
            maxValue = (*it)->getValue();
        }
    }
    return maxValue;
}

std::ostream& operator<<(std::ostream& output, VSCEnveloppe& p) {
	output << "VSCEnveloppe with points:";
	for (VSCEnveloppe::ConstPointIterator pntIt = p.getPointBeginIterator(); pntIt != p.getPointEndIterator(); pntIt++) {
		output << "\n	" << *((*pntIt).get());
	}
	return output;
}

