/*
 *  VSCEnveloppePoint.cpp
 *  EnveloppeEditor
 *
 *  Created by Jonathan Thorpe on 29/07/2011.
 *  Copyright 2011 JBAT. All rights reserved.
 *
 */

#include "VSCEnveloppePoint.h"

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