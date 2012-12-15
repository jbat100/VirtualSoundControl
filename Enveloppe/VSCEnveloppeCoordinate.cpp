/*
 *  VSC::EnveloppeCoordinate.cpp
 *  EnveloppeEditor
 *
 *  Created by Jonathan Thorpe on 25/09/2011.
 *  Copyright 2011 JBAT. All rights reserved.
 *
 */

#include "VSC::EnveloppeCoordinate.h"
#include "VSCSound.h"

#include <boost/assert.hpp>

#pragma mark - Comparison Functions

bool VSC::compareEnveloppeCoordinateValues (VSC::EnveloppeCoordinate* firstPoint, VSC::EnveloppeCoordinate* secondPoint) {
	return firstPoint->getValue() < secondPoint->getValue();
}

bool VSC::compareEnveloppeCoordinateTimes (VSC::EnveloppeCoordinate* firstPoint, VSC::EnveloppeCoordinate* secondPoint) {
	return firstPoint->getTime() < secondPoint->getTime();
}

VSC::EnveloppeCoordinate::EnveloppeCoordinate() {
    setTime(0.0);
    setValue(0.0);
}

VSC::EnveloppeCoordinate::EnveloppeCoordinate(Float value, Float time) {
	setTime(time);
    setValue(value);
}

VSC::EnveloppeCoordinate::EnveloppeCoordinate(const EnveloppeCoordinate& c) {
	_value = c.getValue();
	_time = c.getTime();
}

VSC::EnveloppeCoordinate::~EnveloppeCoordinate(void) {
	std::cout << "Destroying enveloppe coordinate!" << std::endl;
}

void VSC::EnveloppeCoordinate::setValue(Float value) {
	_value = value;
}

VSC::Float VSC::EnveloppeCoordinate::getValue(void) const {
	return _value;
}

void VSC::EnveloppeCoordinate::setTime(Float time) {
    BOOST_ASSERT(time >= 0.0);
	_time = time;
}

VSC::Float VSC::EnveloppeCoordinate::getTime(void) const {
	return _time;
}

#pragma mark --- Operator <<

std::ostream& VSC::operator<<(std::ostream& output, const VSC::EnveloppeCoordinate& c) {
    output << "VSC::EnveloppeCoordinate (time: " <<  c._time << "s, value:" << c._value << ")";
    return output;  
}
