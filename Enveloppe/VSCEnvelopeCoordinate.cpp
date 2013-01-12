/*
 *  VSC::EnvelopeCoordinate.cpp
 *  EnvelopeEditor
 *
 *  Created by Jonathan Thorpe on 25/09/2011.
 *  Copyright 2011 JBAT. All rights reserved.
 *
 */

#include "VSC::EnvelopeCoordinate.h"
#include "VSCSound.h"

#include <boost/assert.hpp>

#pragma mark - Comparison Functions

bool VSC::compareEnvelopeCoordinateValues (VSC::EnvelopeCoordinate* firstPoint, VSC::EnvelopeCoordinate* secondPoint) {
	return firstPoint->getValue() < secondPoint->getValue();
}

bool VSC::compareEnvelopeCoordinateTimes (VSC::EnvelopeCoordinate* firstPoint, VSC::EnvelopeCoordinate* secondPoint) {
	return firstPoint->getTime() < secondPoint->getTime();
}

VSC::EnvelopeCoordinate::EnvelopeCoordinate() {
    setTime(0.0);
    setValue(0.0);
}

VSC::EnvelopeCoordinate::EnvelopeCoordinate(Float value, Float time) {
	setTime(time);
    setValue(value);
}

VSC::EnvelopeCoordinate::EnvelopeCoordinate(const EnvelopeCoordinate& c) {
	_value = c.getValue();
	_time = c.getTime();
}

VSC::EnvelopeCoordinate::~EnvelopeCoordinate(void) {
	std::cout << "Destroying envelope coordinate!" << std::endl;
}

void VSC::EnvelopeCoordinate::setValue(Float value) {
	_value = value;
}

VSC::Float VSC::EnvelopeCoordinate::getValue(void) const {
	return _value;
}

void VSC::EnvelopeCoordinate::setTime(Float time) {
    BOOST_ASSERT(time >= 0.0);
	_time = time;
}

VSC::Float VSC::EnvelopeCoordinate::getTime(void) const {
	return _time;
}

#pragma mark --- Operator <<

std::ostream& VSC::operator<<(std::ostream& output, const VSC::EnvelopeCoordinate& c) {
    output << "VSC::EnvelopeCoordinate (time: " <<  c._time << "s, value:" << c._value << ")";
    return output;  
}
