//
//  VSCStaticEnveloppePoint.cpp
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 1/24/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#include "VSCStaticEnveloppePoint.h"

#include <cstddef>
#include <assert.h>


VSCStaticEnveloppePoint::VSCEnveloppePoint(void) {
    _leftControlCoordinate = VSCEnveloppeCoordinate();
    _rightControlCoordinate = VSCEnveloppeCoordinate();
}

VSCStaticEnveloppePoint::VSCStaticEnveloppePoint(VSCSFloat value, VSCSFloat time) : VSCEnveloppeCoordinate (value, time) {
    _leftControlCoordinate = VSCEnveloppeCoordinate();
    _rightControlCoordinate = VSCEnveloppeCoordinate();
}

VSCStaticEnveloppePoint::VSCStaticEnveloppePoint(const VSCStaticEnveloppePoint& p) : VSCEnveloppeCoordinate(p) {
    _value = p._value;
    _time = p._time;
    _leftControlCoordinate = p._leftControlCoordinate;
    _rightControlCoordinate = p._rightControlCoordinate;
}

VSCStaticEnveloppePoint::~VSCStaticEnveloppePoint(void) {
	std::cout << "Destroying enveloppe point!" << std::endl;
}

void VSCStaticEnveloppePoint::setLeftControlEnveloppeCoordinate(VSCEnveloppeCoordinate controlCoordinate) {
    _leftControlCoordinate = controlCoordinate;
}

VSCStaticEnveloppePoint VSCEnveloppePoint::getLeftControlEnveloppeCoordinate(void) const {
    return _leftControlCoordinate;
}

void VSCStaticEnveloppePoint::setRightControlEnveloppeCoordinate(VSCEnveloppeCoordinate controlCoordinate) {
    _rightControlCoordinate = controlCoordinate;
}

VSCStaticEnveloppePoint VSCEnveloppePoint::getRightControlEnveloppeCoordinate(void) const {
    return _rightControlCoordinate;
}

#pragma mark --- Operator <<

std::ostream& operator<<(std::ostream& output, const VSCStaticEnveloppePoint& p) {
    output << "VSCEnveloppePoint (time: " <<  p._time << "s, value: " << p._value; 
	output << ", left control: " << *(p._leftControlCoordinate.get()); 
	output << ", right control: " << *(p._rightControlCoordinate.get()); 
	output << ")";
    return output;  
}