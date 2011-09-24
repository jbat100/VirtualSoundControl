/*
 *  VSCEnveloppePoint.cpp
 *  EnveloppeEditor
 *
 *  Created by Jonathan Thorpe on 29/07/2011.
 *  Copyright 2011 JBAT. All rights reserved.
 *
 */

#include "VSCEnveloppePoint.h"
#include <cstddef>
#include <assert.h>

bool compareEnveloppePointValues (VSCEnveloppePointPtr firstPoint, VSCEnveloppePointPtr secondPoint) {
	return firstPoint->getValue() < secondPoint->getValue();
}

bool compareEnveloppePointTimes (VSCEnveloppePointPtr firstPoint, VSCEnveloppePointPtr secondPoint) {
	return firstPoint->getTime() < secondPoint->getTime();
}

VSCEnveloppePoint::VSCEnveloppePoint(void) {
    setTime(0.0);
    setValue(0.0);
    setLeftControlPoint(VSCEnveloppePointPtr());
    setRightControlPoint(VSCEnveloppePointPtr());
}

VSCEnveloppePoint::VSCEnveloppePoint(double value, double time) {
	setTime(time);
    setValue(value);
    setLeftControlPoint(VSCEnveloppePointPtr());
    setRightControlPoint(VSCEnveloppePointPtr());
}

VSCEnveloppePoint::VSCEnveloppePoint(const VSCEnveloppePoint& p) {
    
	_value = p.getValue();
	_time = p.getTime();
    
    if (p.getLeftControlPoint()) {
        VSCEnveloppePoint* leftRawPtr = p.getLeftControlPoint().get();
        setLeftControlPoint(VSCEnveloppePointPtr(new VSCEnveloppePoint(*leftRawPtr)));
    }
    else {
        setLeftControlPoint(VSCEnveloppePointPtr());
    }
    
    if (p.getRightControlPoint()) {
        VSCEnveloppePoint* rightRawPtr = p.getRightControlPoint().get();
        setRightControlPoint(VSCEnveloppePointPtr(new VSCEnveloppePoint(*rightRawPtr)));
    }
    else {
        setRightControlPoint(VSCEnveloppePointPtr());
    }
    
}

VSCEnveloppePoint::~VSCEnveloppePoint(void) {
	std::cout << "Destroying enveloppe point!" << std::endl;
	
    /* NO NEED TO DELETE SHARED POINTERS */
    /*
	if (_leftControlPoint)
		delete _leftControlPoint;
	
	if (_rightControlPoint)
		delete _rightControlPoint;
	*/
}

void VSCEnveloppePoint::setValue(double value) {
	_value = value;
}

double VSCEnveloppePoint::getValue(void) const {
	return _value;
}

void VSCEnveloppePoint::setTime(double time) {
    assert(time >= 0.0);
	_time = time;
}

double VSCEnveloppePoint::getTime(void) const {
	return _time;
}

void VSCEnveloppePoint::setLeftControlPoint(VSCEnveloppePointPtr controlPoint) {
    _leftControlPoint = controlPoint;
}

VSCEnveloppePointPtr VSCEnveloppePoint::getLeftControlPoint(void) const {
    return _leftControlPoint;
}

void VSCEnveloppePoint::setRightControlPoint(VSCEnveloppePointPtr controlPoint) {
    _rightControlPoint = controlPoint;
}

VSCEnveloppePointPtr VSCEnveloppePoint::getRightControlPoint(void) const {
    return _rightControlPoint;
}

#pragma mark --- Operator <<

std::ostream& operator<<(std::ostream& output, const VSCEnveloppePoint& p) {
    output << "VSCEnveloppePoint (time: " <<  p._time << "s, value:" << p._value <<")";
    return output;  
}
