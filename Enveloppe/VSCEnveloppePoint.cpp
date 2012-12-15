/*
 *  VSC::EnveloppePoint.cpp
 *  EnveloppeEditor
 *
 *  Created by Jonathan Thorpe on 29/07/2011.
 *  Copyright 2011 JBAT. All rights reserved.
 *
 */

#include "VSC::EnveloppePoint.h"
#include "VSCSound.h"

#include <cstddef>
#include <assert.h>


VSC::EnveloppePoint::EnveloppePoint(void) {
    setLeftControlEnveloppeCoordinate(EnveloppeCoordinate::SPtr());
    setRightControlEnveloppeCoordinate(EnveloppeCoordinate::SPtr());
}

VSC::EnveloppePoint::EnveloppePoint(Float value, Float time) : EnveloppeCoordinate (value, time) {
    setLeftControlEnveloppeCoordinate(EnveloppeCoordinate::SPtr());
    setRightControlEnveloppeCoordinate(EnveloppeCoordinate::SPtr());
}

VSC::EnveloppePoint::EnveloppePoint(const EnveloppePoint& p) : EnveloppeCoordinate(p) {
    
    if (p.getLeftControlEnveloppeCoordinate()) {
        EnveloppeCoordinate* leftRawPtr = p.getLeftControlEnveloppeCoordinate().get();
        setLeftControlEnveloppeCoordinate(EnveloppeCoordinate::SPtr(new EnveloppeCoordinate(*leftRawPtr)));
    }
    else {
        setLeftControlEnveloppeCoordinate(EnveloppeCoordinate::SPtr());
    }
    
    if (p.getRightControlEnveloppeCoordinate()) {
        EnveloppeCoordinate* rightRawPtr = p.getRightControlEnveloppeCoordinate().get();
        setRightControlEnveloppeCoordinate(EnveloppeCoordinate::SPtr(new EnveloppeCoordinate(*rightRawPtr)));
    }
    else {
        setRightControlEnveloppeCoordinate(EnveloppeCoordinate::SPtr());
    }
    
}

VSC::EnveloppePoint::~EnveloppePoint(void) {
	std::cout << "Destroying enveloppe point!" << std::endl;
    /* MUST NOT DELETE SHARED POINTERS */
}

void VSC::EnveloppePoint::setLeftControlEnveloppeCoordinate(EnveloppeCoordinate::SPtr controlCoordinate) {
    _leftControlCoordinate = controlCoordinate;
}

VSC::EnveloppeCoordinate::SPtr EnveloppePoint::getLeftControlEnveloppeCoordinate(void) const {
    return _leftControlCoordinate;
}

void VSC::EnveloppePoint::setRightControlEnveloppeCoordinate(EnveloppeCoordinate::SPtr controlCoordinate) {
    _rightControlCoordinate = controlCoordinate;
}

VSC::EnveloppeCoordinate::SPtr EnveloppePoint::getRightControlEnveloppeCoordinate(void) const {
    return _rightControlCoordinate;
}

#pragma mark --- Operator <<

std::ostream& VSC::operator<<(std::ostream& output, const EnveloppePoint& p) {
    output << "EnveloppePoint (time: " <<  p._time << "s, value: " << p._value; 
	if (p._leftControlCoordinate)
		output << ", left control: " << *(p._leftControlCoordinate.get()); 
	if (p._rightControlCoordinate)
		output << ", right control: " << *(p._rightControlCoordinate.get()); 
	output << ")";
    return output;  
}

bool VSC::compareEnveloppePointTimes(const VSC::EnveloppePoint::SPtr& point1, const VSC::EnveloppePoint::SPtr& point2) {
    if (point1->getTime() < point2->getTime()) {
        return true;
    }
    return false;
}
