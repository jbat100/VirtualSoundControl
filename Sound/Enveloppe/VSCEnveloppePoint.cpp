/*
 *  VSCEnveloppePoint.cpp
 *  EnveloppeEditor
 *
 *  Created by Jonathan Thorpe on 29/07/2011.
 *  Copyright 2011 JBAT. All rights reserved.
 *
 */

#include "VSCEnveloppePoint.h"
#include "VSCSound.h"

#include <cstddef>
#include <assert.h>


VSCEnveloppePoint::VSCEnveloppePoint(void) {
    setLeftControlEnveloppeCoordinate(VSCEnveloppeCoordinatePtr());
    setRightControlEnveloppeCoordinate(VSCEnveloppeCoordinatePtr());
}

VSCEnveloppePoint::VSCEnveloppePoint(VSCSFloat value, VSCSFloat time) : VSCEnveloppeCoordinate (value, time) {
    setLeftControlEnveloppeCoordinate(VSCEnveloppeCoordinatePtr());
    setRightControlEnveloppeCoordinate(VSCEnveloppeCoordinatePtr());
}

VSCEnveloppePoint::VSCEnveloppePoint(const VSCEnveloppePoint& p) : VSCEnveloppeCoordinate(p) {
    
    if (p.getLeftControlEnveloppeCoordinate()) {
        VSCEnveloppeCoordinate* leftRawPtr = p.getLeftControlEnveloppeCoordinate().get();
        setLeftControlEnveloppeCoordinate(VSCEnveloppeCoordinatePtr(new VSCEnveloppeCoordinate(*leftRawPtr)));
    }
    else {
        setLeftControlEnveloppeCoordinate(VSCEnveloppeCoordinatePtr());
    }
    
    if (p.getRightControlEnveloppeCoordinate()) {
        VSCEnveloppeCoordinate* rightRawPtr = p.getRightControlEnveloppeCoordinate().get();
        setRightControlEnveloppeCoordinate(VSCEnveloppeCoordinatePtr(new VSCEnveloppeCoordinate(*rightRawPtr)));
    }
    else {
        setRightControlEnveloppeCoordinate(VSCEnveloppeCoordinatePtr());
    }
    
}

VSCEnveloppePoint::~VSCEnveloppePoint(void) {
	std::cout << "Destroying enveloppe point!" << std::endl;
    /* MUST NOT DELETE SHARED POINTERS */
}

void VSCEnveloppePoint::setLeftControlEnveloppeCoordinate(VSCEnveloppeCoordinatePtr controlCoordinate) {
    _leftControlCoordinate = controlCoordinate;
}

VSCEnveloppeCoordinatePtr VSCEnveloppePoint::getLeftControlEnveloppeCoordinate(void) const {
    return _leftControlCoordinate;
}

void VSCEnveloppePoint::setRightControlEnveloppeCoordinate(VSCEnveloppeCoordinatePtr controlCoordinate) {
    _rightControlCoordinate = controlCoordinate;
}

VSCEnveloppeCoordinatePtr VSCEnveloppePoint::getRightControlEnveloppeCoordinate(void) const {
    return _rightControlCoordinate;
}

#pragma mark --- Operator <<

std::ostream& operator<<(std::ostream& output, const VSCEnveloppePoint& p) {
    output << "VSCEnveloppePoint (time: " <<  p._time << "s, value: " << p._value; 
	if (p._leftControlCoordinate)
		output << ", left control: " << *(p._leftControlCoordinate.get()); 
	if (p._rightControlCoordinate)
		output << ", right control: " << *(p._rightControlCoordinate.get()); 
	output << ")";
    return output;  
}

bool compareEnveloppePointTimes(const VSCEnveloppePoint& point1, const VSCEnveloppePoint& point2) {
    if (point1.getTime() < point2.getTime()) {
        return true;
    }
    return false;
}
