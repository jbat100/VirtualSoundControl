/*
 *  VSCEnveloppeViewSetup.cpp
 *  EnveloppeEditor
 *
 *  Created by Jonathan Thorpe on 02/08/2011.
 *  Copyright 2011 JBAT. All rights reserved.
 *
 */

#include "VSCEnveloppeViewSetup.h"

double VSCEnveloppeViewSetup::getMinTime(void) const {
	return _minTime;
}

void VSCEnveloppeViewSetup::setMinTime(double  minTime) {
	_minTime = minTime;
}


double VSCEnveloppeViewSetup::getMaxTime(void) const {
	return _maxTime;
}

void VSCEnveloppeViewSetup::setMaxTime(double  maxTime) {
	_maxTime = maxTime;
}


double VSCEnveloppeViewSetup::getMinValue(void) const {
	return _minValue;
}

void VSCEnveloppeViewSetup::setMinValue(double  minValue) {
	_minValue = minValue;
}


double VSCEnveloppeViewSetup::getMaxValue(void) const {
	return _maxValue;
}

void VSCEnveloppeViewSetup::setMaxValue(double  maxValue) {
	_maxValue = maxValue;
}


VSCEnveloppeDisplayType VSCEnveloppeViewSetup::getDisplayType(void) const {
	return _displayType;
}

void VSCEnveloppeViewSetup::setDisplayType(VSCEnveloppeDisplayType displayType) {
	_displayType = displayType;
}


double VSCEnveloppeViewSetup::getControlPointRadius(void) const {
	return _controlPointRadius;
}

void VSCEnveloppeViewSetup::setControlPointRadius(double controlPointRadius) {
	_controlPointRadius = controlPointRadius;
}


double VSCEnveloppeViewSetup::getLineWidth(void) const {
	return _lineWidth;
}

void VSCEnveloppeViewSetup::setLineWidth(double lineWidth) {
	_lineWidth = lineWidth;
}


