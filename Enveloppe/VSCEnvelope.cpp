
#include "VSCEnvelope.h"
#include "VSCEnvelopePoint.h"
#include "VSCBoost.h"
#include "VSCException.h"
#include "VSCSound.h"

#include <fstream>
#include <limits>
#include <cmath>
#include <assert.h>

#include <boost/assert.hpp>
#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/nvp.hpp>

const std::string VSC::Envelope::FileExtension = "vscenv";

#pragma mark - Save/Load

/*
 *	Saving and loading
 */

void VSC::Envelope::saveToXMLFile(const char * filepath)
{
    // make an archive
    std::ofstream ofs(filepath);
    assert(ofs.good());
    boost::archive::xml_oarchive oa(ofs);
    oa << boost::serialization::make_nvp("envelope", *this);
}


#pragma mark - Static Factory Methods

VSC::Envelope::SPtr VSC::Envelope::createFromXMLFile(const char * filepath)
{
    Envelope::SPtr envPtr = Envelope::SPtr(new Envelope());
    // open the archive
    std::ifstream ifs(filepath);
    assert(ifs.good());
    boost::archive::xml_iarchive ia(ifs);
    // restore the schedule from the archive
    ia >> boost::serialization::make_nvp("envelope", *envPtr);
    return envPtr;
}

VSC::Envelope::SPtr VSC::Envelope::createFlatEnvelope(Float duration, unsigned int numberOfPoints, Float value)
{
    Envelope::SPtr envPtr = Envelope::SPtr(new Envelope());
    
    Float step = duration / (Float)(numberOfPoints-1);
    Float currentTime = 0.0;
    
    for (unsigned int i = 0; i < numberOfPoints; ++i) {
        EnvelopePoint::SPtr pntPtr = EnvelopePoint::SPtr(new EnvelopePoint(value, currentTime));
        envPtr->addPoint(pntPtr);
        currentTime += step;
    }
    
    return envPtr;
}

VSC::Envelope::SPtr VSC::Envelope::createADSREnvelope(Float attack, Float decay, Float sustain, Float release, Float sustainValue)
{
    Envelope::SPtr envPtr = Envelope::SPtr(new Envelope());
        
    EnvelopePoint::SPtr pntPtr = EnvelopePoint::SPtr(new EnvelopePoint(0.0, 0.0));
    envPtr->addPoint(pntPtr);
    pntPtr.reset();
    
    pntPtr = EnvelopePoint::SPtr(new EnvelopePoint(attack, 1.0));
    envPtr->addPoint(pntPtr);
    pntPtr.reset();
    
    pntPtr = EnvelopePoint::SPtr(new EnvelopePoint(attack+decay, sustainValue));
    envPtr->addPoint(pntPtr);
    pntPtr.reset();
    
    pntPtr = EnvelopePoint::SPtr(new EnvelopePoint(attack+decay+sustain, sustainValue));
    envPtr->addPoint(pntPtr);
    pntPtr.reset();
    
    pntPtr = EnvelopePoint::SPtr(new EnvelopePoint(attack+decay+sustain+release, 0.0));
    envPtr->addPoint(pntPtr);
    pntPtr.reset();
    
    return envPtr;
}

VSC::Envelope::SPtr VSC::Envelope::createEmptyEnvelope(void)
{
    
    Envelope::SPtr envPtr = Envelope::SPtr(new Envelope());
    return envPtr;
}

#pragma mark - Constructor/Destructor/Defaults

VSC::Envelope::Envelope(void)
{
	mMinimumTimeStep = 0.01;
    mCurveType = CurveTypeLinear;
    mPointDisplacementConflictResolution = PointDisplacementConflictResolutionMix;
    mAllowedTimeRange = TimeRange(0.0, 10.0);
    mAllowedValueRange = ValueRange(0.0, 1.0);
}

VSC::Envelope::~Envelope(void)
{
	std::cout << "Destroying Envelope!" << std::endl;
}


#pragma mark - General Setters/Getters

void VSC::Envelope::setPointDisplacementConflictResolution(Envelope::PointDisplacementConflictResolution r)
{
    if (mPointDisplacementConflictResolution == PointDisplacementConflictResolutionClear ||
        mPointDisplacementConflictResolution == PointDisplacementConflictResolutionBlock)
    {
        throw VSCNotImplementedException();
    }
    
	mPointDisplacementConflictResolution = r;
}

VSC::Envelope::PointDisplacementConflictResolution VSC::Envelope::getPointDisplacementConflictResolution(void) const
{
	return mPointDisplacementConflictResolution;
}

void VSC::Envelope::setFilePath(std::string filePath)
{
	mLoadedFromFilePath = filePath;
}

std::string VSC::Envelope::getFilePath(void) const
{
	return mLoadedFromFilePath;
}

std::string VSC::Envelope::getName(void) const
{
	return boost::filesystem::path(mLoadedFromFilePath).replace_extension(NULL).filename().string();
}


bool VSC::Envelope::isSortedByTime(void) const
{    
    if (mPoints.size() < 1) return true;
    Points::iterator it = mPoints.begin();
    Float currentTime = (*it)->getTime();
    for (; it != mPoints.end(); it++) {
        if ((*it)->getTime() < currentTime) {
            return false;
        }
        currentTime = (*it)->getTime();
    }
    return true;
}

#pragma mark - Add/Remove Envelope Points

void VSC::Envelope::addPoint(EnvelopePoint::SPtr point) {
    
    BOOST_ASSERT(point);
	
    std::cout << "Envelope::addPoint " << *(point.get()) << std::endl;
    // check if the point already is in the envelope, no dupicate pointers please ...
    
    // first, if there are no points yet in the envelope then we just push back the new point and return
    if (mPoints.size() == 0) {
        mPoints.push_back(point);
        return;
    }
	
    Points::iterator currentIt;
	Points::iterator nextIt;
	
	Points pointsToRemove;
	
	// remove close points
	for (currentIt = mPoints.begin(); currentIt != mPoints.end(); currentIt++) {
		Float difference = std::abs(point->getTime() - (*currentIt)->getTime());
		if (difference < mMinimumTimeStep) {
			std::cout << "Found point " << *(*currentIt) << " closer than minimum time step (" << mMinimumTimeStep << ") ";
			std::cout << "by " << difference << "\n";
			pointsToRemove.push_back(*currentIt);
		}
	}
	this->removePoints(pointsToRemove);
	
    // if the time is less than that of the first point in the envelope then add at the beginning
    currentIt = mPoints.begin();
    if ((*currentIt)->getTime() > point->getTime()) {
        mPoints.insert(currentIt, point);
    }
    // otherwise find the right spot to insert the point, keeping the points ordered by time 
    else {
        bool found = false;
        for (currentIt = mPoints.begin(); currentIt != mPoints.end(); currentIt++) {
            // is there a next element
            nextIt = currentIt;
            nextIt++;
            if (nextIt == mPoints.end()) {
                break;
            }
            const EnvelopePoint::SPtr& p = (*currentIt);
            const EnvelopePoint::SPtr& np = (*nextIt);
            // insert in the list if the time value is contained between this point's time and next point's time
            if (p->getTime() < point->getTime() && np->getTime() > point->getTime()) {
                found = true;
                break;
            }
        }
        if (found) { // if we found a previous and next point
            mPoints.insert(nextIt, point);
        }
        else { // else push back at the end
            mPoints.push_back(point);
        }
    }
	
	//std::cout << "After point add, envelope is: " << *this << "\n";
    
    BOOST_ASSERT(this->isSortedByTime());
	
}

void VSC::Envelope::removePoint(EnvelopePoint::SPtr point) {
	BOOST_ASSERT(point);
	std::cout << "Envelope::removePoint: " << *point << std::endl;
    Points::iterator it = mPoints.find(point);
    BOOST_ASSERT_MSG(it != mPoints.end(), "Attempted to remove absent point");
	mPoints.erase(it);
    BOOST_ASSERT(this->isSortedByTime());
}

void VSC::Envelope::addPoints(Points& points) {
    
    BOOST_FOREACH(EnvelopePoint::SPtr point, points)
    {
        this->addPoint(point);
    }
    BOOST_ASSERT(isSortedByTime());
}

void VSC::Envelope::removePoints(Points& points) {
	//std::cout << "In removePoints";
	if (points.size() == 0)
    {
		//std::cout << ", no points to remove\n";
	}
	BOOST_FOREACH(EnvelopePoint::SPtr point, points)
    {
		this->removePoint(point);
	}
    BOOST_ASSERT(this->isSortedByTime());
}

void VSC::Envelope::removePointsInTimeRange(TimeRange range){
	Points points = this->getPointsInTimeRange(range);
    //std::cout << "Removing points in time range " << range.origin << " - " << range.size << std::endl;
	this->removePoints(l);
    BOOST_ASSERT(this->isSortedByTime());
}

void VSC::Envelope::removeAllPoints(void) {
	mPoints.clear();
}

#pragma mark - Change Notification

void VSC::Envelope::envelopeChangedBetweenEnvelopePoints(Points::iterator begin, Points::iterator end)
{
    std::cout << "In Envelope envelopeChangedBetweenEnvelopePoints " << **begin << " " << **end;
}

/*
 *	Envelope changes calls (mostly for subclasses to update cache tables)
 */
void VSC::Envelope::envelopeChangedBetweenEnvelopePoints(EnvelopePoint::SPtr begin, EnvelopePoint::SPtr end)
{
	std::cout << "In Envelope envelopeChangedBetweenEnvelopePoints " << *begin << " " << *end;
}

void VSC::Envelope::envelopeChangedBetweenEnvelopePointAndNext(EnvelopePoint::SPtr point)
{
	std::cout << "In Envelope envelopeChangedBetweenEnvelopePointAndNext " << *point;
}

void VSC::Envelope::envelopeChanged(void) {
	std::cout << "In Envelope envelopeChanged";
}

#pragma mark - Point Getters


/* get points */

VSC::EnvelopePoint::SPtr VSC::Envelope::getPointClosestToTime(Float time) const
{
	
	EnvelopePoint::SPtr closestPointBefore = this->getFirstPointBeforeTime(time);
    EnvelopePoint::SPtr closestPointAfter = this->getFirstPointAfterTime(time);
    
    if (!closestPointAfter && !closestPointBefore) return EnvelopePoint::SPtr(); // return NULL pointer
    if (!closestPointBefore) return closestPointAfter;
    if (!closestPointAfter) return closestPointBefore;
    
    Float beforeInterval = std::abs( closestPointBefore->getTime() - time );
    Float afterInterval = std::abs( closestPointAfter->getTime() - time );
    
    if (beforeInterval > afterInterval) return closestPointAfter;
		
	return closestPointBefore;
	
}

VSC::EnvelopePoint::SPtr VSC::Envelope::getFirstPointAfterTime(Float time) const
{
	
	if (mPoints.size() == 0) 
		return EnvelopePoint::SPtr();
	
	for (ConstPointIterator it = mPoints.begin(); it != mPoints.end(); it++) {
		if ((*it)->getTime() > time) {
			return (*it);
		}
	}
	
	return EnvelopePoint::SPtr();
	
}

VSC::EnvelopePoint::SPtr VSC::Envelope::getFirstPointBeforeTime(Float time) const
{

	if (mPoints.size() == 0) return EnvelopePoint::SPtr();
	
	for (Points::reverse_iterator it = mPoints.rbegin(); it != mPoints.rend(); it++)
    {
		if ((*it)->getTime() < time)
        {
			return (*it);
		}
	}
	
	return EnvelopePoint::SPtr();
	
}

void VSC::Envelope::getPointsInTimeRange(const TimeRange& range) const {
	
    Points points;
    
	BOOST_FOREACH(EnvelopePoint:SPtr point, mPoints)
    {
		if (point->getTime() > range.origin && point->getTime() < range.origin + range.size)
        {
			points.push_back(point);
		}
	}
    
    return points;
}

const VSC::Envelope::PointList& VSC::Envelope::getPoints(void) const {
    return mPoints;
}

int VSC::Envelope::numberOfPoints(void) const {
	return (int)mPoints.size();
}

#pragma mark - Point Displacement 

// this method is necessary in order to validate that all displacements are possible when displacePoints is performed
bool VSC::Envelope::canDisplacePoint(EnvelopePoint::SPtr point, Float deltaTime, Float deltaValue) const {
    
    /*
     *  Check allowed time and value limits 
     */
    if (deltaTime > 0 && point->getTime() + deltaTime > mAllowedTimeRange.origin + mAllowedTimeRange.size) {
        std::cout << "Cannot move point " << *point << " by time " << deltaTime  << "allowed range ( ";
        std::cout << mAllowedTimeRange.origin << " " << mAllowedTimeRange.origin + mAllowedTimeRange.size << ")" << std::endl; 
        return false;
    }
    if (deltaTime < 0 && point->getTime() + deltaTime < mAllowedTimeRange.origin)  {
        std::cout << "Cannot move point " << *point << " by time " << deltaTime  << "allowed range ( ";
        std::cout << mAllowedTimeRange.origin << " " << mAllowedTimeRange.origin + mAllowedTimeRange.size << ")" << std::endl; 
        return false;
    }
    if (deltaValue > 0 && point->getValue() + deltaValue > mAllowedValueRange.origin + mAllowedValueRange.size) {
        std::cout << "Cannot move point " << *point << " by value " << deltaValue  << "allowed range ( ";
        std::cout << mAllowedValueRange.origin << " " << mAllowedValueRange.origin + mAllowedValueRange.size << ")" << std::endl; 
        return false;
    }
    if (deltaValue < 0 && point->getValue() + deltaValue < mAllowedValueRange.origin) {
        std::cout << "Cannot move point " << *point << " by value " << deltaValue  << "allowed range ( ";
        std::cout << mAllowedValueRange.origin << " " << mAllowedValueRange.origin + mAllowedValueRange.size << ")" << std::endl;
        return false;
    }
    /*
     *  Check point collision according to resolution method
     */
    if (mPointDisplacementConflictResolution == PointDisplacementConflictResolutionNone) return true;
    if (mPointDisplacementConflictResolution == PointDisplacementConflictResolutionMix) return true;
    
    if (mPointDisplacementConflictResolution == PointDisplacementConflictResolutionClear) {
        
        throw VSCNotImplementedException();
        
    }
    
    if (mPointDisplacementConflictResolution == PointDisplacementConflictResolutionBlock) {
        
        throw VSCNotImplementedException();
        
        /*
        
        PointIterator pointIt = std::find(mPoints.begin(), mPoints.end(), point);
        if (pointIt == mPoints.end()) {
            throw VSCSInvalidArgumentException();
        }
        
        if (pointIt == mPoints.begin()) {
            PointIterator nextIt = pointIt;
            nextIt++;
        }
        
        */
    }
    
    throw VSCInternalInconsistencyException();
    
}

// this iterator must be an iterator into mPoints
bool VSC::Envelope::displacePoint(PointIterator pointIt, Float deltaTime, Float deltaValue) {
	
    EnvelopePoint::SPtr point = *pointIt;
    
#ifdef VSC_DEBUG
    PointIterator selfPointIt = std::find(mPoints.begin(), mPoints.end(), point);
    assert(selfPointIt != mPoints.end());
#endif
    
    if (this->canDisplacePoint(point, deltaTime, deltaValue)) 
    {
        
        if (mPointDisplacementConflictResolution == PointDisplacementConflictResolutionClear) 
        {
            if (deltaTime > 0) {
                //this->removePointsInTimeRange(TimeRange(point->getTime(), deltaTime + mMinimumTimeStep));
            }
            if (deltaTime < 0) {
                //this->removePointsInTimeRange(TimeRange(point->getTime() + deltaTime - mMinimumTimeStep, - deltaTime + mMinimumTimeStep));
            }
        }
        
        point->setTime(point->getTime()+deltaTime);
        point->setValue(point->getValue()+deltaValue);
        
    }
    
    return true;
	
}

bool VSC::Envelope::displacePoint(EnvelopePoint::SPtr point, Float deltaTime, Float deltaValue) {
	PointIterator pointIt = std::find(mPoints.begin(), mPoints.end(), point);
    return this->displacePoint(pointIt, deltaTime, deltaValue);
}

bool VSC::Envelope::displacePoints(PointList& pts, Float deltaTime, Float deltaValue) {
	
    // the list is most likely not the same as mPoints so we can't just use it with displacePoints which requires
    // the iterator to be so.
    
    // first we need to check that all the points CAN be displaced
    
    for (ConstPointIterator pointIt = pts.begin(); pointIt != pts.end(); pointIt++) {
        EnvelopePoint::SPtr point = *pointIt;
        if (this->canDisplacePoint(point, deltaTime, deltaValue) == false) {
            return false;
        }
    }
    
    // then we actually displace all the points and return true ...
    for (PointIterator pointIt = pts.begin(); pointIt != pts.end(); pointIt++) {
        EnvelopePoint::SPtr point = *pointIt;
        if (this->displacePoint(point, deltaTime, deltaValue) == false) {
            // we should not be getting back false here as we have previously checked that
            // all the points can be displaced
            throw VSCInternalInconsistencyException();
        }
    }
    
    this->sortPointsByTime();
    
    return true;
    
}


#pragma mark - Envelope Duration 

VSC::Float VSC::Envelope::duration(void) const {
	
	assert(isSortedByTime());
	
	/* 
	 * If there are no points then return 0 duration 
	 */
	if (numberOfPoints() < 1)
		return 0.0;
	
	/* 
	 * Get last pointand return its time value
	 */
	ConstReversePointIterator rit = mPoints.rbegin();  
	
	return (*rit)->getTime();
	
}

#pragma mark - Value Computation

/* value */

VSC::Float VSC::Envelope::getValueAtTime(Float time) const {
	
	EnvelopePoint::SPtr lp = getFirstPointBeforeTime(time);
	EnvelopePoint::SPtr up = getFirstPointAfterTime(time);
	//EnvelopePoint::SPtr up = lp;
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

void VSC::Envelope::sortPointsByTime(void) {
	mPoints.sort(compareEnvelopePointTimes);
}

#pragma mark - Envelope Extremes

/* extremes */
VSC::Float VSC::Envelope::minTime(void) const {
    assert(isSortedByTime());
    if (mPoints.size() == 0) throw EnvelopeEmptyException();
    return (*(mPoints.begin()))->getTime();
}

VSC::Float VSC::Envelope::maxTime(void) const {
    assert(isSortedByTime());
    if (mPoints.size() == 0) throw EnvelopeEmptyException();
    ConstPointIterator endIter = mPoints.end();
    endIter--;
    return (*endIter)->getTime();
}

VSC::Float VSC::Envelope::minValue(void) const {
    if (mPoints.size() == 0) throw EnvelopeEmptyException();
    Float minValue = std::numeric_limits<Float>::max();
    for (ConstPointIterator it = mPoints.begin(); it != mPoints.end(); it++) {
        if ((*it)->getValue() < minValue) minValue = (*it)->getValue();
    }
    return minValue;
}

Float VSC::Envelope::maxValue(void) const {
    if (mPoints.size() == 0) throw EnvelopeEmptyException();
    Float maxValue = std::numeric_limits<Float>::min();
    for (ConstPointIterator it = mPoints.begin(); it != mPoints.end(); it++) {
        if ((*it)->getValue() > maxValue) maxValue = (*it)->getValue();
    }
    return maxValue;
}

std::ostream& VSC::operator<<(std::ostream& output, Envelope& p) {
	output << "Envelope with points:";
	for (Envelope::ConstPointIterator pntIt = p.getPointBeginIterator(); pntIt != p.getPointEndIterator(); pntIt++) {
		output << "\n	" << *((*pntIt).get());
	}
	return output;
}

void VSC::sortEnvelopePointListByTime(Envelope::PointList& points) {
    points.sort(compareEnvelopePointTimes);
}



