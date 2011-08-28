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
#

bool compareEnveloppePointValues (VSCEnveloppePoint* firstPoint, VSCEnveloppePoint* secondPoint) {
	return firstPoint->getValue() < secondPoint->getValue();
}

bool compareEnveloppePointTimes (VSCEnveloppePoint* firstPoint, VSCEnveloppePoint* secondPoint) {
	return firstPoint->getTime() < secondPoint->getTime();
}

VSCEnveloppePoint::VSCEnveloppePoint(void) {
    setTime(0.0);
    setValue(0.0);
    setLeftControlPoint(NULL);
    setRightControlPoint(NULL);
}

VSCEnveloppePoint::VSCEnveloppePoint(double value, double time) {
	setTime(time);
    setValue(value);
    setLeftControlPoint(NULL);
    setRightControlPoint(NULL);
}

VSCEnveloppePoint::VSCEnveloppePoint(const VSCEnveloppePoint& p) {
    
	_value = p.getValue();
	_time = p.getTime();
    
    if (p.getLeftControlPoint() != NULL) 
        setLeftControlPoint(new VSCEnveloppePoint(*(p.getLeftControlPoint())));
    else 
        setLeftControlPoint(NULL);
    
    if (p.getRightControlPoint() != NULL) 
        setRightControlPoint(new VSCEnveloppePoint(*(p.getRightControlPoint())));
    else 
        setRightControlPoint(NULL);
    
}

VSCEnveloppePoint::~VSCEnveloppePoint(void) {
	std::cout << "Destroying enveloppe point!" << std::endl;
	
	if (_leftControlPoint)
		delete _leftControlPoint;
	
	if (_rightControlPoint)
		delete _rightControlPoint;
	
}

void VSCEnveloppePoint::setValue(double value) {
	_value = value;
}

double VSCEnveloppePoint::getValue(void) const {
	return _value;
}

void VSCEnveloppePoint::setTime(double time) {
	_time = time;
}

double VSCEnveloppePoint::getTime(void) const {
	return _time;
}

void VSCEnveloppePoint::setLeftControlPoint(VSCEnveloppePoint* controlPoint) {
    _leftControlPoint = controlPoint;
}

VSCEnveloppePoint* VSCEnveloppePoint::getLeftControlPoint(void) const {
    return _leftControlPoint;
}

void VSCEnveloppePoint::setRightControlPoint(VSCEnveloppePoint* controlPoint) {
    _rightControlPoint = controlPoint;
}

VSCEnveloppePoint* VSCEnveloppePoint::getRightControlPoint(void) const {
    return _rightControlPoint;
}

#pragma mark --- Operator <<

std::ostream& operator<<(std::ostream& output, const VSCEnveloppePoint& p) {
    output << "VSCEnveloppePoint (time: " <<  p._time << "s, value:" << p._value <<")";
    return output;  
}
