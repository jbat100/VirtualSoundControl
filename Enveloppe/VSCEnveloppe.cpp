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
#include <limits>
#include <cmath>
#include <assert.h>

#include <boost/filesystem.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/nvp.hpp>

const std::string VSCEnveloppe::FileExtension = "vscenv";

#pragma mark - Save/Load

/*
 *	Saving and loading
 */

void VSCEnveloppe::saveToXMLFile(const char * filepath) {
    // make an archive
    std::ofstream ofs(filepath);
    assert(ofs.good());
    boost::archive::xml_oarchive oa(ofs);
    oa << boost::serialization::make_nvp("enveloppe", *this);
}


#pragma mark - Static Factory Methods

VSCEnveloppePtr VSCEnveloppe::createFromXMLFile(const char * filepath) {
    VSCEnveloppePtr envPtr = VSCEnveloppePtr(new VSCEnveloppe());
    // open the archive
    std::ifstream ifs(filepath);
    assert(ifs.good());
    boost::archive::xml_iarchive ia(ifs);
    // restore the schedule from the archive
    ia >> boost::serialization::make_nvp("enveloppe", *envPtr);
    return envPtr;
}

VSCEnveloppePtr VSCEnveloppe::createFlatEnveloppe(VSCSFloat duration, unsigned int numberOfPoints, VSCSFloat value) {

    VSCEnveloppePtr envPtr = VSCEnveloppePtr(new VSCEnveloppe());
    
    VSCSFloat step = duration / (VSCSFloat)(numberOfPoints-1);
    VSCSFloat currentTime = 0.0;
    
    for (unsigned int i = 0; i < numberOfPoints; ++i) {
        VSCEnveloppePointPtr pntPtr = VSCEnveloppePointPtr(new VSCEnveloppePoint(value, currentTime));
        envPtr->addPoint(pntPtr);
        currentTime += step;
    }
    
    return envPtr;
    
}

VSCEnveloppePtr VSCEnveloppe::createADSREnveloppe(VSCSFloat attack, VSCSFloat decay, VSCSFloat sustain, VSCSFloat release, VSCSFloat sustainValue) {
    
    VSCEnveloppePtr envPtr = VSCEnveloppePtr(new VSCEnveloppe());
        
    VSCEnveloppePointPtr pntPtr = VSCEnveloppePointPtr(new VSCEnveloppePoint(0.0, 0.0));
    envPtr->addPoint(pntPtr);
    pntPtr.reset();
    
    pntPtr = VSCEnveloppePointPtr(new VSCEnveloppePoint(attack, 1.0));
    envPtr->addPoint(pntPtr);
    pntPtr.reset();
    
    pntPtr = VSCEnveloppePointPtr(new VSCEnveloppePoint(attack+decay, sustainValue));
    envPtr->addPoint(pntPtr);
    pntPtr.reset();
    
    pntPtr = VSCEnveloppePointPtr(new VSCEnveloppePoint(attack+decay+sustain, sustainValue));
    envPtr->addPoint(pntPtr);
    pntPtr.reset();
    
    pntPtr = VSCEnveloppePointPtr(new VSCEnveloppePoint(attack+decay+sustain+release, 0.0));
    envPtr->addPoint(pntPtr);
    pntPtr.reset();
    
    return envPtr;
    
}

VSCEnveloppePtr VSCEnveloppe::createEmptyEnveloppe(void) {
    
    VSCEnveloppePtr envPtr = VSCEnveloppePtr(new VSCEnveloppe());
    
    return envPtr;
    
}

#pragma mark - Constructor/Destructor/Defaults

VSCEnveloppe::VSCEnveloppe(void) {
	_minimumTimeStep = 0.01;
    _curveType = CurveTypeLinear;
    _pointDisplacementConflictResolution = PointDisplacementConflictResolutionMix;
    _allowedTimeRange = TimeRange(0.0, 10.0);
    _allowedValueRange = ValueRange(0.0, 1.0);
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
    
    if (_pointDisplacementConflictResolution == PointDisplacementConflictResolutionClear ||
        _pointDisplacementConflictResolution == PointDisplacementConflictResolutionBlock) {
        throw VSCNotImplementedException();
    }
    
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


void VSCEnveloppe::setAllowedTimeRange(TimeRange range) {
    _allowedTimeRange = range;
}

VSCEnveloppe::TimeRange VSCEnveloppe::getAllowedTimeRange(void) {
    return _allowedTimeRange;
}

void VSCEnveloppe::setAllowedValueRange(ValueRange range) {
    _allowedValueRange = range;
}

VSCEnveloppe::ValueRange VSCEnveloppe::getAllowedValueRange(void) {
    return _allowedValueRange;
}

bool VSCEnveloppe::isSortedByTime(void) const {
    
    if (_points.size() < 1) 
        return true;
    
    ConstPointIterator it = _points.begin();
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
	
    std::cout << "Adding " << *(point.get()) << "\n";
    // check if the point already is in the enveloppe, no dupicate pointers please ...
    
    // first, if there are no points yet in the enveloppe then we just push back the new point and return
    if (_points.size() == 0) {
        _points.push_back(point);
        return;
    }
	
	PointIterator currentIt;
	PointIterator nextIt;
	
	PointList pointsToRemove;
	
	// remove close points
	for (currentIt = _points.begin(); currentIt != _points.end(); currentIt++) {
		VSCSFloat difference = std::abs(point->getTime() - (*currentIt)->getTime());
		if (difference < _minimumTimeStep) {
			std::cout << "Found point " << *(*currentIt) << " closer than minimum time step (" << _minimumTimeStep << ") ";
			std::cout << "by " << difference << "\n";
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
	std::cout << "In removePoint: " << *point << std::endl;
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
    this->sortPointsByTime();
    //assert(isSortedByTime());
}

void VSCEnveloppe::removePointsInTimeRange(TimeRange range){
	PointList l;
    std::cout << "Removing points in time range " << range.origin << " - " << range.size << std::endl;
	this->getPointsInTimeRange(l, range);
	this->removePoints(l);
    assert(isSortedByTime());
}

void VSCEnveloppe::removeAllPoints(void) {
	_points.clear();
}

#pragma mark - Change Notification

void VSCEnveloppe::enveloppeChangedBetweenEnveloppePoints(ConstPointIterator begin, ConstPointIterator end) {
    std::cout << "In VSCEnveloppe enveloppeChangedBetweenEnveloppePoints " << **begin << " " << **end;
}

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
	
	VSCEnveloppePointPtr closestPointBefore = this->getFirstPointBeforeTime(time);
    VSCEnveloppePointPtr closestPointAfter = this->getFirstPointAfterTime(time);
    
    if (!closestPointAfter && !closestPointBefore) return VSCEnveloppePointPtr(); // return NULL pointer
    if (!closestPointBefore) return closestPointAfter;
    if (!closestPointAfter) return closestPointBefore;
    
    VSCSFloat beforeInterval = std::abs( closestPointBefore->getTime() - time );
    VSCSFloat afterInterval = std::abs( closestPointAfter->getTime() - time );
    
    if (beforeInterval > afterInterval) return closestPointAfter;
		
	return closestPointBefore;
	
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
	
	for (ConstReversePointIterator it = _points.rbegin(); it != _points.rend(); it++) {
		if ((*it)->getTime() < time) {
			return (*it);
		}
	}
	
	return VSCEnveloppePointPtr();
	
}

void VSCEnveloppe::getPointsInTimeRange(PointList& l, TimeRange range) const {
	
	for (ConstPointIterator it = _points.begin(); it != _points.end(); it++) {
		if ((*it)->getTime() > range.origin && (*it)->getTime() < range.origin+ range.size) {
			l.push_back(*it);
		}
	}

}

const VSCEnveloppe::PointList& VSCEnveloppe::getPoints(void) const {
    return _points;
}

int VSCEnveloppe::numberOfPoints(void) const {
	return (int)_points.size();
}

#pragma mark - Point Displacement 

// this method is necessary in order to validate that all displacements are possible when displacePoints is performed
bool VSCEnveloppe::canDisplacePoint(VSCEnveloppePointPtr point, VSCSFloat deltaTime, VSCSFloat deltaValue) const {
    
    /*
     *  Check allowed time and value limits 
     */
    if (deltaTime > 0 && point->getTime() + deltaTime > _allowedTimeRange.origin + _allowedTimeRange.size) {
        std::cout << "Cannot move point " << *point << " by time " << deltaTime  << "allowed range ( ";
        std::cout << _allowedTimeRange.origin << " " << _allowedTimeRange.origin + _allowedTimeRange.size << ")" << std::endl; 
        return false;
    }
    if (deltaTime < 0 && point->getTime() + deltaTime < _allowedTimeRange.origin)  {
        std::cout << "Cannot move point " << *point << " by time " << deltaTime  << "allowed range ( ";
        std::cout << _allowedTimeRange.origin << " " << _allowedTimeRange.origin + _allowedTimeRange.size << ")" << std::endl; 
        return false;
    }
    if (deltaValue > 0 && point->getValue() + deltaValue > _allowedValueRange.origin + _allowedValueRange.size) {
        std::cout << "Cannot move point " << *point << " by value " << deltaValue  << "allowed range ( ";
        std::cout << _allowedValueRange.origin << " " << _allowedValueRange.origin + _allowedValueRange.size << ")" << std::endl; 
        return false;
    }
    if (deltaValue < 0 && point->getValue() + deltaValue < _allowedValueRange.origin) {
        std::cout << "Cannot move point " << *point << " by value " << deltaValue  << "allowed range ( ";
        std::cout << _allowedValueRange.origin << " " << _allowedValueRange.origin + _allowedValueRange.size << ")" << std::endl;
        return false;
    }
    /*
     *  Check point collision according to resolution method
     */
    if (_pointDisplacementConflictResolution == PointDisplacementConflictResolutionNone) return true;
    if (_pointDisplacementConflictResolution == PointDisplacementConflictResolutionMix) return true;
    
    if (_pointDisplacementConflictResolution == PointDisplacementConflictResolutionClear) {
        
        throw VSCNotImplementedException();
        
    }
    
    if (_pointDisplacementConflictResolution == PointDisplacementConflictResolutionBlock) {
        
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
    
    throw VSCInternalInconsistencyException();
    
}

// this iterator must be an iterator into _points
bool VSCEnveloppe::displacePoint(PointIterator pointIt, VSCSFloat deltaTime, VSCSFloat deltaValue) {
	
    VSCEnveloppePointPtr point = *pointIt;
    
#ifdef VSC_DEBUG
    PointIterator selfPointIt = std::find(_points.begin(), _points.end(), point);
    assert(selfPointIt != _points.end());
#endif
    
    if (this->canDisplacePoint(point, deltaTime, deltaValue)) 
    {
        
        if (_pointDisplacementConflictResolution == PointDisplacementConflictResolutionClear) 
        {
            if (deltaTime > 0) {
                //this->removePointsInTimeRange(TimeRange(point->getTime(), deltaTime + _minimumTimeStep));
            }
            if (deltaTime < 0) {
                //this->removePointsInTimeRange(TimeRange(point->getTime() + deltaTime - _minimumTimeStep, - deltaTime + _minimumTimeStep));
            }
        }
        
        point->setTime(point->getTime()+deltaTime);
        point->setValue(point->getValue()+deltaValue);
        
    }
    
    return true;
	
}

bool VSCEnveloppe::displacePoint(VSCEnveloppePointPtr point, VSCSFloat deltaTime, VSCSFloat deltaValue) {
	PointIterator pointIt = std::find(_points.begin(), _points.end(), point);
    return this->displacePoint(pointIt, deltaTime, deltaValue);
}

bool VSCEnveloppe::displacePoints(PointList& pts, VSCSFloat deltaTime, VSCSFloat deltaValue) {
	
    // the list is most likely not the same as _points so we can't just use it with displacePoints which requires
    // the iterator to be so.
    
    // first we need to check that all the points CAN be displaced
    
    for (ConstPointIterator pointIt = pts.begin(); pointIt != pts.end(); pointIt++) {
        VSCEnveloppePointPtr point = *pointIt;
        if (this->canDisplacePoint(point, deltaTime, deltaValue) == false) {
            return false;
        }
    }
    
    // then we actually displace all the points and return true ...
    for (PointIterator pointIt = pts.begin(); pointIt != pts.end(); pointIt++) {
        VSCEnveloppePointPtr point = *pointIt;
        if (this->displacePoint(point, deltaTime, deltaValue) == false) {
            // we should not be getting back false here as we have previously checked that
            // all the points can be displaced
            throw VSCInternalInconsistencyException();
        }
    }
    
    this->sortPointsByTime();
    
    return true;
    
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
	ConstReversePointIterator rit = _points.rbegin();  
	
	return (*rit)->getTime();
	
}

#pragma mark - Value Computation

/* value */

VSCSFloat VSCEnveloppe::getValueAtTime(VSCSFloat time) const {
	
	VSCEnveloppePointPtr lp = getFirstPointBeforeTime(time);
	VSCEnveloppePointPtr up = getFirstPointAfterTime(time);
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
	_points.sort(compareEnveloppePointTimes);
}

#pragma mark - Enveloppe Extremes

/* extremes */
VSCSFloat VSCEnveloppe::minTime(void) const {
    assert(isSortedByTime());
    if (_points.size() == 0) throw VSCEnveloppeEmptyException();
    return (*(_points.begin()))->getTime();
}

VSCSFloat VSCEnveloppe::maxTime(void) const {
    assert(isSortedByTime());
    if (_points.size() == 0) throw VSCEnveloppeEmptyException();
    ConstPointIterator endIter = _points.end();
    endIter--;
    return (*endIter)->getTime();
}

VSCSFloat VSCEnveloppe::minValue(void) const {
    if (_points.size() == 0) throw VSCEnveloppeEmptyException();
    VSCSFloat minValue = std::numeric_limits<VSCSFloat>::max();
    for (ConstPointIterator it = _points.begin(); it != _points.end(); it++) {
        if ((*it)->getValue() < minValue) minValue = (*it)->getValue();
    }
    return minValue;
}

VSCSFloat VSCEnveloppe::maxValue(void) const {
    if (_points.size() == 0) throw VSCEnveloppeEmptyException();
    VSCSFloat maxValue = std::numeric_limits<VSCSFloat>::min();
    for (ConstPointIterator it = _points.begin(); it != _points.end(); it++) {
        if ((*it)->getValue() > maxValue) maxValue = (*it)->getValue();
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

void sortEnveloppePointListByTime(VSCEnveloppe::PointList& points) {
    points.sort(compareEnveloppePointTimes);
}



