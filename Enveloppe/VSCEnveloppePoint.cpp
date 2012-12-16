/*
 *  VSC::EnvelopePoint.cpp
 *  EnvelopeEditor
 *
 *  Created by Jonathan Thorpe on 29/07/2011.
 *  Copyright 2011 JBAT. All rights reserved.
 *
 */

#include "VSC::EnvelopePoint.h"
#include "VSCSound.h"

#include <cstddef>
#include <assert.h>


VSC::EnvelopePoint::EnvelopePoint(void) {
    setLeftControlEnvelopeCoordinate(EnvelopeCoordinate::SPtr());
    setRightControlEnvelopeCoordinate(EnvelopeCoordinate::SPtr());
}

VSC::EnvelopePoint::EnvelopePoint(Float value, Float time) : EnvelopeCoordinate (value, time) {
    setLeftControlEnvelopeCoordinate(EnvelopeCoordinate::SPtr());
    setRightControlEnvelopeCoordinate(EnvelopeCoordinate::SPtr());
}

VSC::EnvelopePoint::EnvelopePoint(const EnvelopePoint& p) : EnvelopeCoordinate(p) {
    
    if (p.getLeftControlEnvelopeCoordinate()) {
        EnvelopeCoordinate* leftRawPtr = p.getLeftControlEnvelopeCoordinate().get();
        setLeftControlEnvelopeCoordinate(EnvelopeCoordinate::SPtr(new EnvelopeCoordinate(*leftRawPtr)));
    }
    else {
        setLeftControlEnvelopeCoordinate(EnvelopeCoordinate::SPtr());
    }
    
    if (p.getRightControlEnvelopeCoordinate()) {
        EnvelopeCoordinate* rightRawPtr = p.getRightControlEnvelopeCoordinate().get();
        setRightControlEnvelopeCoordinate(EnvelopeCoordinate::SPtr(new EnvelopeCoordinate(*rightRawPtr)));
    }
    else {
        setRightControlEnvelopeCoordinate(EnvelopeCoordinate::SPtr());
    }
    
}

VSC::EnvelopePoint::~EnvelopePoint(void) {
	std::cout << "Destroying envelope point!" << std::endl;
    /* MUST NOT DELETE SHARED POINTERS */
}

void VSC::EnvelopePoint::setLeftControlEnvelopeCoordinate(EnvelopeCoordinate::SPtr controlCoordinate) {
    _leftControlCoordinate = controlCoordinate;
}

VSC::EnvelopeCoordinate::SPtr EnvelopePoint::getLeftControlEnvelopeCoordinate(void) const {
    return _leftControlCoordinate;
}

void VSC::EnvelopePoint::setRightControlEnvelopeCoordinate(EnvelopeCoordinate::SPtr controlCoordinate) {
    _rightControlCoordinate = controlCoordinate;
}

VSC::EnvelopeCoordinate::SPtr EnvelopePoint::getRightControlEnvelopeCoordinate(void) const {
    return _rightControlCoordinate;
}

#pragma mark --- Operator <<

std::ostream& VSC::operator<<(std::ostream& output, const EnvelopePoint& p) {
    output << "EnvelopePoint (time: " <<  p._time << "s, value: " << p._value; 
	if (p._leftControlCoordinate)
		output << ", left control: " << *(p._leftControlCoordinate.get()); 
	if (p._rightControlCoordinate)
		output << ", right control: " << *(p._rightControlCoordinate.get()); 
	output << ")";
    return output;  
}

bool VSC::compareEnvelopePointTimes(const VSC::EnvelopePoint::SPtr& point1, const VSC::EnvelopePoint::SPtr& point2) {
    if (point1->getTime() < point2->getTime()) {
        return true;
    }
    return false;
}
