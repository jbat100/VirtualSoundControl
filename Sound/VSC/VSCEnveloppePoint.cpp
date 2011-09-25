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


VSCEnveloppePoint::VSCEnveloppePoint(void) {
    setLeftControlPoint(VSCEnveloppePointPtr());
    setRightControlPoint(VSCEnveloppePointPtr());
}

VSCEnveloppePoint::VSCEnveloppePoint(double value, double time) : VSCEnveloppeCoordinate (value, time) {
    setLeftControlPoint(VSCEnveloppePointPtr());
    setRightControlPoint(VSCEnveloppePointPtr());
}

VSCEnveloppePoint::VSCEnveloppePoint(const VSCEnveloppePoint& p) : VSCEnveloppeCoordinate(p) {
    
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
    /* MUST NOT DELETE SHARED POINTERS */
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
    output << "VSCEnveloppePoint (time: " <<  p._time << "s, value: " << p._value; 
	output<< ", left control: " << *(_leftControlCoordinate.get()); 
	output<< ", right control: " << *(_rightControlCoordinate.get()) << ")";
    return output;  
}
