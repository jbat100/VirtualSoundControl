/*
 *  VSCEnveloppeCoordinate.cpp
 *  EnveloppeEditor
 *
 *  Created by Jonathan Thorpe on 25/09/2011.
 *  Copyright 2011 JBAT. All rights reserved.
 *
 */

#include "VSCEnveloppeCoordinate.h"

#pragma mark - Comparison Functions

bool compareEnveloppeCoordinateValues (VSCEnveloppeCoordinate* firstPoint, VSCEnveloppeCoordinate* secondPoint) {
	return firstPoint->getValue() < secondPoint->getValue();
}

bool compareEnveloppeCoordinateTimes (VSCEnveloppeCoordinate* firstPoint, VSCEnveloppeCoordinate* secondPoint) {
	return firstPoint->getTime() < secondPoint->getTime();
}

VSCEnveloppeCoordinate::VSCEnveloppeCoordinate(void) {
    setTime(0.0);
    setValue(0.0);
}

VSCEnveloppeCoordinate::VSCEnveloppeCoordinate(double value, double time) {
	setTime(time);
    setValue(value);
}

VSCEnveloppeCoordinate::VSCEnveloppeCoordinate(const VSCEnveloppeCoordinate& c) {
	_value = c.getValue();
	_time = c.getTime();
}

VSCEnveloppeCoordinate::~VSCEnveloppeCoordinate(void) {
	std::cout << "Destroying enveloppe point!" << std::endl;
}

void VSCEnveloppeCoordinate::setValue(double value) {
	_value = value;
}

double VSCEnveloppeCoordinate::getValue(void) const {
	return _value;
}

void VSCEnveloppeCoordinate::setTime(double time) {
    assert(time >= 0.0);
	_time = time;
}

double VSCEnveloppeCoordinate::getTime(void) const {
	return _time;
}

#pragma mark --- Operator <<

std::ostream& operator<<(std::ostream& output, const VSCEnveloppeCoordinate& c) {
    output << "VSCEnveloppeCoordinate (time: " <<  c._time << "s, value:" << c._value <<")";
    return output;  
}
