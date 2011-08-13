/*
 *  VSCEnveloppeViewSetup.cpp
 *  EnveloppeEditor
 *
 *  Created by Jonathan Thorpe on 02/08/2011.
 *  Copyright 2011 JBAT. All rights reserved.
 *
 */

#include "VSCEnveloppeViewSetup.h"

#pragma mark View range setters / getters

/*
 *	View range setters / getters
 */

float VSCEnveloppeViewSetup::getMinTime(void) const {
	return _minTime;
}

void VSCEnveloppeViewSetup::setMinTime(float  minTime) {
	_minTime = minTime;
}


float VSCEnveloppeViewSetup::getMaxTime(void) const {
	return _maxTime;
}

void VSCEnveloppeViewSetup::setMaxTime(float  maxTime) {
	_maxTime = maxTime;
}

float VSCEnveloppeViewSetup::getMinValue(void) const {
	return _minValue;
}

void VSCEnveloppeViewSetup::setMinValue(float  minValue) {
	_minValue = minValue;
}


float VSCEnveloppeViewSetup::getMaxValue(void) const {
	return _maxValue;
}

void VSCEnveloppeViewSetup::setMaxValue(float  maxValue) {
	_maxValue = maxValue;
}

#pragma mark Display type setters / getters

/*
 *	Display type setters / getters
 */

VSCEnveloppeDisplayType VSCEnveloppeViewSetup::getDisplayType(void) const {
	return _displayType;
}

void VSCEnveloppeViewSetup::setDisplayType(VSCEnveloppeDisplayType displayType) {
	_displayType = displayType;
}

#pragma mark Control Points setters / getters

/*
 *	Control Points setters / getters
 */

float VSCEnveloppeViewSetup::getControlPointRadius(void) const {
	return _controlPointRadius;
}

void VSCEnveloppeViewSetup::setControlPointRadius(float controlPointRadius) {
	_controlPointRadius = controlPointRadius;
}

VSCColour VSCEnveloppeViewSetup::getControlPointSelectedColour(void) const {
	return _controlPointSelectedColour;
}

void VSCEnveloppeViewSetup::setControlPointSelectedColour(VSCColour colour) {
	_controlPointSelectedColour = colour;
}

VSCColour VSCEnveloppeViewSetup::getControlPointUnselectedColour(void) const {
	return _controlPointUnselectedColour;
}

void VSCEnveloppeViewSetup::setControlPointUnselectedColour(VSCColour colour) {
	_controlPointUnselectedColour = colour;
}

float VSCEnveloppeViewSetup::getLineWidth(void) const {
	return _lineWidth;
}

void VSCEnveloppeViewSetup::setLineWidth(float lineWidth) {
	_lineWidth = lineWidth;
}

#pragma mark Enveloppe lines setters / getters

/*
 *	Enveloppe lines setters / getters
 */

VSCColour VSCEnveloppeViewSetup::getLineColour(void) const {
	return _lineColour;
}

void VSCEnveloppeViewSetup::setLineColour(VSCColour colour) {
	_lineColour = colour;
}


