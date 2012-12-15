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

const std::string VSC::Enveloppe::FileExtension = "vscenv";

#pragma mark - Save/Load

/*
 *	Saving and loading
 */

void VSC::Enveloppe::saveToXMLFile(const char * filepath) {
    // make an archive
    std::ofstream ofs(filepath);
    assert(ofs.good());
    boost::archive::xml_oarchive oa(ofs);
    oa << boost::serialization::make_nvp("enveloppe", *this);
}


#pragma mark - Static Factory Methods

VSC::Enveloppe::SPtr VSC::Enveloppe::createFromXMLFile(const char * filepath) {
    Enveloppe::SPtr envPtr = Enveloppe::SPtr(new Enveloppe());
    // open the archive
    std::ifstream ifs(filepath);
    assert(ifs.good());
    boost::archive::xml_iarchive ia(ifs);
    // restore the schedule from the archive
    ia >> boost::serialization::make_nvp("enveloppe", *envPtr);
    return envPtr;
}

VSC::Enveloppe::SPtr VSC::Enveloppe::createFlatEnveloppe(Float duration, unsigned int numberOfPoints, Float value) {

    Enveloppe::SPtr envPtr = Enveloppe::SPtr(new Enveloppe());
    
    Float step = duration / (Float)(numberOfPoints-1);
    Float currentTime = 0.0;
    
    for (unsigned int i = 0; i < numberOfPoints; ++i) {
        EnveloppePoint::SPtr pntPtr = EnveloppePoint::SPtr(new EnveloppePoint(value, currentTime));
        envPtr->addPoint(pntPtr);
        currentTime += step;
    }
    
    return envPtr;
    
}

VSC::Enveloppe::SPtr VSC::Enveloppe::createADSREnveloppe(Float attack, Float decay, Float sustain, Float release, Float sustainValue) {
    
    Enveloppe::SPtr envPtr = Enveloppe::SPtr(new Enveloppe());
        
    EnveloppePoint::SPtr pntPtr = EnveloppePoint::SPtr(new EnveloppePoint(0.0, 0.0));
    envPtr->addPoint(pntPtr);
    pntPtr.reset();
    
    pntPtr = EnveloppePoint::SPtr(new EnveloppePoint(attack, 1.0));
    envPtr->addPoint(pntPtr);
    pntPtr.reset();
    
    pntPtr = EnveloppePoint::SPtr(new EnveloppePoint(attack+decay, sustainValue));
    envPtr->addPoint(pntPtr);
    pntPtr.reset();
    
    pntPtr = EnveloppePoint::SPtr(new EnveloppePoint(attack+decay+sustain, sustainValue));
    envPtr->addPoint(pntPtr);
    pntPtr.reset();
    
    pntPtr = EnveloppePoint::SPtr(new EnveloppePoint(attack+decay+sustain+release, 0.0));
    envPtr->addPoint(pntPtr);
    pntPtr.reset();
    
    return envPtr;
    
}

VSC::Enveloppe::SPtr VSC::Enveloppe::createEmptyEnveloppe(void) {
    
    Enveloppe::SPtr envPtr = Enveloppe::SPtr(new Enveloppe());
    
    return envPtr;
    
}

#pragma mark - Constructor/Destructor/Defaults

VSC::Enveloppe::Enveloppe(void) {
	_minimumTimeStep = 0.01;
    _curveType = CurveTypeLinear;
    _pointDisplacementConflictResolution = PointDisplacementConflictResolutionMix;
    _allowedTimeRange = TimeRange(0.0, 10.0);
    _allowedValueRange = ValueRange(0.0, 1.0);
}

VSC::Enveloppe::~Enveloppe(void) {
	std::cout << "Destroying Enveloppe!" << std::endl;
}


#pragma mark - General Setters/Getters


void VSC::Enveloppe::setCurveType(Enveloppe::CurveType curveType) {
	_curveType = curveType;
}

VSC::Enveloppe::CurveType VSC::Enveloppe::getCurveType(void) const {
	return _curveType;
}


void VSC::Enveloppe::setPointDisplacementConflictResolution(Enveloppe::PointDisplacementConflictResolution r) {
    
    if (_pointDisplacementConflictResolution == PointDisplacementConflictResolutionClear ||
        _pointDisplacementConflictResolution == PointDisplacementConflictResolutionBlock) {
        throw VSCNotImplementedException();
    }
    
	_pointDisplacementConflictResolution = r;
}

VSC::Enveloppe::PointDisplacementConflictResolution VSC::Enveloppe::getPointDisplacementConflictResolution(void) const {
	return _pointDisplacementConflictResolution;
}

void VSC::Enveloppe::setFilePath(std::string filePath) {
	_filePath = filePath;
}

std::string VSC::Enveloppe::getFilePath(void) const {
	return _filePath;
}

std::string VSC::Enveloppe::getName(void) const {
	return boost::filesystem::path(_filePath).replace_extension(NULL).filename().string();
}

void VSC::Enveloppe::setMinimumTimeStep(Float minimumTimeStep) {
	_minimumTimeStep = minimumTimeStep;
}

Float VSC::Enveloppe::getMinimumTimeStep(void) const {
	return _minimumTimeStep;
}


void VSC::Enveloppe::setAllowedTimeRange(TimeRange range) {
    _allowedTimeRange = range;
}

VSC::Enveloppe::TimeRange VSC::Enveloppe::getAllowedTimeRange(void) {
    return _allowedTimeRange;
}

void VSC::Enveloppe::setAllowedValueRange(ValueRange range) {
    _allowedValueRange = range;
}

VSC::Enveloppe::ValueRange VSC::Enveloppe::getAllowedValueRange(void) {
    return _allowedValueRange;
}

bool VSC::Enveloppe::isSortedByTime(void) const {
    
    if (_points.size() < 1) 
        return true;
    
    ConstPointIterator it = _points.begin();
    Float currentTime = (*it)->getTime();
    for (; it != _points.end(); it++) {
        if ((*it)->getTime() < currentTime) {
            return false;
        }
        currentTime = (*it)->getTime();
    }
    
    return true;
    
}

#pragma mark - Add/Remove Enveloppe Points

void VSC::Enveloppe::addPoint(EnveloppePoint::SPtr point) {
    
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
		Float difference = std::abs(point->getTime() - (*currentIt)->getTime());
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
            EnveloppePoint::SPtr p(*currentIt);
            EnveloppePoint::SPtr np(*nextIt);
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

void VSC::Enveloppe::removePoint(EnveloppePoint::SPtr point) {
	assert(point);
	std::cout << "In removePoint: " << *point << std::endl;
	_points.remove(point);
    assert(isSortedByTime());
}

void VSC::Enveloppe::addPoints(PointList& points) {
	for (PointIterator it = _points.begin(); it != _points.end(); it++) {
		addPoint(*it);
	}
    assert(isSortedByTime());
}

void VSC::Enveloppe::removePoints(PointList& pnts) {
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

void VSC::Enveloppe::removePointsInTimeRange(TimeRange range){
	PointList l;
    std::cout << "Removing points in time range " << range.origin << " - " << range.size << std::endl;
	this->getPointsInTimeRange(l, range);
	this->removePoints(l);
    assert(isSortedByTime());
}

void VSC::Enveloppe::removeAllPoints(void) {
	_points.clear();
}

#pragma mark - Change Notification

void VSC::Enveloppe::enveloppeChangedBetweenEnveloppePoints(ConstPointIterator begin, ConstPointIterator end) {
    std::cout << "In Enveloppe enveloppeChangedBetweenEnveloppePoints " << **begin << " " << **end;
}

/*
 *	Enveloppe changes calls (mostly for subclasses to update cache tables)
 */
void VSC::Enveloppe::enveloppeChangedBetweenEnveloppePoints(EnveloppePoint::SPtr begin, EnveloppePoint::SPtr end) {
	std::cout << "In Enveloppe enveloppeChangedBetweenEnveloppePoints " << *begin << " " << *end;
}

void VSC::Enveloppe::enveloppeChangedBetweenEnveloppePointAndNext(EnveloppePoint::SPtr point) {
	std::cout << "In Enveloppe enveloppeChangedBetweenEnveloppePointAndNext " << *point;
}

void VSC::Enveloppe::enveloppeChanged(void) {
	std::cout << "In Enveloppe enveloppeChanged";
}

#pragma mark - Point Getters

VSC::Enveloppe::PointIterator VSC::Enveloppe::getPointBeginIterator(void) {
	return _points.begin();
}

VSC::Enveloppe::PointIterator VSC::Enveloppe::getPointEndIterator(void) {
	return _points.end();
}

VSC::Enveloppe::ConstPointIterator VSC::Enveloppe::getPointBeginConstIterator(void) const {
	return _points.begin();
}

VSC::Enveloppe::ConstPointIterator VSC::Enveloppe::getPointEndConstIterator(void) const {
	return _points.end();
}

/* get points */

VSC::EnveloppePoint::SPtr VSC::Enveloppe::getPointClosestToTime(Float time) const {
	
	EnveloppePoint::SPtr closestPointBefore = this->getFirstPointBeforeTime(time);
    EnveloppePoint::SPtr closestPointAfter = this->getFirstPointAfterTime(time);
    
    if (!closestPointAfter && !closestPointBefore) return EnveloppePoint::SPtr(); // return NULL pointer
    if (!closestPointBefore) return closestPointAfter;
    if (!closestPointAfter) return closestPointBefore;
    
    Float beforeInterval = std::abs( closestPointBefore->getTime() - time );
    Float afterInterval = std::abs( closestPointAfter->getTime() - time );
    
    if (beforeInterval > afterInterval) return closestPointAfter;
		
	return closestPointBefore;
	
}

VSC::EnveloppePoint::SPtr VSC::Enveloppe::getFirstPointAfterTime(Float time) const {
	
	if (_points.size() == 0) 
		return EnveloppePoint::SPtr();
	
	for (ConstPointIterator it = _points.begin(); it != _points.end(); it++) {
		if ((*it)->getTime() > time) {
			return (*it);
		}
	}
	
	return EnveloppePoint::SPtr();
	
}

VSC::EnveloppePoint::SPtr VSC::Enveloppe::getFirstPointBeforeTime(Float time) const {

	if (_points.size() == 0) 
		return EnveloppePoint::SPtr();
	
	for (ConstReversePointIterator it = _points.rbegin(); it != _points.rend(); it++) {
		if ((*it)->getTime() < time) {
			return (*it);
		}
	}
	
	return EnveloppePoint::SPtr();
	
}

void VSC::Enveloppe::getPointsInTimeRange(PointList& l, TimeRange range) const {
	
	for (ConstPointIterator it = _points.begin(); it != _points.end(); it++) {
		if ((*it)->getTime() > range.origin && (*it)->getTime() < range.origin+ range.size) {
			l.push_back(*it);
		}
	}

}

const VSC::Enveloppe::PointList& VSC::Enveloppe::getPoints(void) const {
    return _points;
}

int VSC::Enveloppe::numberOfPoints(void) const {
	return (int)_points.size();
}

#pragma mark - Point Displacement 

// this method is necessary in order to validate that all displacements are possible when displacePoints is performed
bool VSC::Enveloppe::canDisplacePoint(EnveloppePoint::SPtr point, Float deltaTime, Float deltaValue) const {
    
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
bool VSC::Enveloppe::displacePoint(PointIterator pointIt, Float deltaTime, Float deltaValue) {
	
    EnveloppePoint::SPtr point = *pointIt;
    
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

bool VSC::Enveloppe::displacePoint(EnveloppePoint::SPtr point, Float deltaTime, Float deltaValue) {
	PointIterator pointIt = std::find(_points.begin(), _points.end(), point);
    return this->displacePoint(pointIt, deltaTime, deltaValue);
}

bool VSC::Enveloppe::displacePoints(PointList& pts, Float deltaTime, Float deltaValue) {
	
    // the list is most likely not the same as _points so we can't just use it with displacePoints which requires
    // the iterator to be so.
    
    // first we need to check that all the points CAN be displaced
    
    for (ConstPointIterator pointIt = pts.begin(); pointIt != pts.end(); pointIt++) {
        EnveloppePoint::SPtr point = *pointIt;
        if (this->canDisplacePoint(point, deltaTime, deltaValue) == false) {
            return false;
        }
    }
    
    // then we actually displace all the points and return true ...
    for (PointIterator pointIt = pts.begin(); pointIt != pts.end(); pointIt++) {
        EnveloppePoint::SPtr point = *pointIt;
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

VSC::Float VSC::Enveloppe::duration(void) const {
	
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

VSC::Float VSC::Enveloppe::getValueAtTime(Float time) const {
	
	EnveloppePoint::SPtr lp = getFirstPointBeforeTime(time);
	EnveloppePoint::SPtr up = getFirstPointAfterTime(time);
	//EnveloppePoint::SPtr up = lp;
	//up++;
	
	if (lp == NULL || up == NULL) 
		return NAN;
	/*
	if (lp == NULL)
		return up->getValue();
	if (up == NULL)
		return lp->getValue();
	 */
	
	Float normDiff = (up->getTime() - lp->getTime()) / (time - lp->getTime()); 
	
	return lp->getValue() + ((up->getValue() - lp->getValue()) * normDiff);

}

#pragma mark - Sorting

/* sorting */

void VSC::Enveloppe::sortPointsByTime(void) {
	_points.sort(compareEnveloppePointTimes);
}

#pragma mark - Enveloppe Extremes

/* extremes */
VSC::Float VSC::Enveloppe::minTime(void) const {
    assert(isSortedByTime());
    if (_points.size() == 0) throw EnveloppeEmptyException();
    return (*(_points.begin()))->getTime();
}

VSC::Float VSC::Enveloppe::maxTime(void) const {
    assert(isSortedByTime());
    if (_points.size() == 0) throw EnveloppeEmptyException();
    ConstPointIterator endIter = _points.end();
    endIter--;
    return (*endIter)->getTime();
}

VSC::Float VSC::Enveloppe::minValue(void) const {
    if (_points.size() == 0) throw EnveloppeEmptyException();
    Float minValue = std::numeric_limits<Float>::max();
    for (ConstPointIterator it = _points.begin(); it != _points.end(); it++) {
        if ((*it)->getValue() < minValue) minValue = (*it)->getValue();
    }
    return minValue;
}

Float VSC::Enveloppe::maxValue(void) const {
    if (_points.size() == 0) throw EnveloppeEmptyException();
    Float maxValue = std::numeric_limits<Float>::min();
    for (ConstPointIterator it = _points.begin(); it != _points.end(); it++) {
        if ((*it)->getValue() > maxValue) maxValue = (*it)->getValue();
    }
    return maxValue;
}

std::ostream& VSC::operator<<(std::ostream& output, Enveloppe& p) {
	output << "Enveloppe with points:";
	for (Enveloppe::ConstPointIterator pntIt = p.getPointBeginIterator(); pntIt != p.getPointEndIterator(); pntIt++) {
		output << "\n	" << *((*pntIt).get());
	}
	return output;
}

void VSC::sortEnveloppePointListByTime(Enveloppe::PointList& points) {
    points.sort(compareEnveloppePointTimes);
}



