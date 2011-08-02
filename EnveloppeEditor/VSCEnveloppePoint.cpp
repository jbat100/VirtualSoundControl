/*
 *  VSCEnveloppePoint.cpp
 *  EnveloppeEditor
 *
 *  Created by Jonathan Thorpe on 29/07/2011.
 *  Copyright 2011 JBAT. All rights reserved.
 *
 */

#include "VSCEnveloppePoint.h"

bool compareEnveloppePointValues (VSCEnveloppePoint* firstPoint, VSCEnveloppePoint* secondPoint) {
	return firstPoint->getValue() < secondPoint->getValue();
}

bool compareEnveloppePointTimes (VSCEnveloppePoint* firstPoint, VSCEnveloppePoint* secondPoint) {
	return firstPoint->getTime() < secondPoint->getTime();
}

VSCEnveloppePoint::VSCEnveloppePoint(const VSCEnveloppePoint& p) {
	
	_value = p.getValue();
	_time = p.getTime();
	
}

void VSCEnveloppePoint::setValue(double value) {
	_value = value;
}

double VSCEnveloppePoint::getValue(void) {
	return _value;
}

void VSCEnveloppePoint::setTime(double time) {
	_time = time;
}

double VSCEnveloppePoint::getTime(void) {
	return _time;
}