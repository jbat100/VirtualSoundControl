/*
 *  VSCEnveloppeViewSetup.cpp
 *  EnveloppeEditor
 *
 *  Created by Jonathan Thorpe on 02/08/2011.
 *  Copyright 2011 JBAT. All rights reserved.
 *
 */

#include "VSCEnveloppeViewSetup.h"
#include "VSCSound.h"
#include "VSCException.h"

#include <cmath>

VSCSFloat pointForTime(const VSCSFloat t, const VSCEnveloppe::TimeRange timeRange, const VSCSFloat width) const {
    
}

VSCSFloat pointForValue(const VSCSFloat v, const VSCEnveloppe::ValueRange valueRange, const VSCSFloat height) const {
    
}

VSCSFloat timeForPoint(const VSCSFloat point, const VSCEnveloppe::TimeRange timeRange, const VSCSFloat width) const {
    
}

VSCSFloat valueForPoint(const VSCSFloat point, const VSCEnveloppe::ValueRange valueRange, const VSCSFloat height) const {
    
}

#pragma mark View range setters / getters

VSCEnveloppeViewSetup::VSCEnveloppeViewSetup(void) {
	setToDefault();
}

void VSCEnveloppeViewSetup::setToDefault(void) {
	this->setDisplayType(VSCEnveloppeDisplayTypeDefault);
    this->setTimeRange(VSCEnveloppe::TimeRange(0.0,5.0));
    this->setValueRange(VSCEnveloppe::ValueRange(0.0,1.0));
    this->setAllowedTimeRange(VSCEnveloppe::TimeRange(0.0,5.0));
    this->setAllowedValueRange(VSCEnveloppe::ValueRange(0.0, 1.0));
}


#pragma mark View range setters / getters

/*
 *	View range setters / getters
 */
const VSCEnveloppe::TimeRange& VSCEnveloppeViewSetup::getTimeRange(void) const {
    return _timeRange;
}

void VSCEnveloppeViewSetup::setTimeRange(const VSCEnveloppe::TimeRange timeRange) {
    _timeRange = timeRange;
}

const VSCEnveloppe::ValueRange& VSCEnveloppeViewSetup::getValueRange(void) const {
    return _valueRange;
}

void VSCEnveloppeViewSetup::setValueRange(const VSCEnveloppe::ValueRange valueRange) {
    _valueRange = valueRange;
}

/*
 *	Allowed range setters / getters
 */

const VSCEnveloppe::TimeRange& VSCEnveloppeViewSetup::getAllowedTimeRange(void) const {
    return _allowedTimeRange;
}

void VSCEnveloppeViewSetup::setAllowedTimeRange(const VSCEnveloppe::TimeRange timeRange) {
    _allowedTimeRange = timeRange;
}

const VSCEnveloppe::ValueRange& VSCEnveloppeViewSetup::getAllowedValueRange(void) const {
    return _allowedValueRange;
}

void VSCEnveloppeViewSetup::setAllowedValueRange(const VSCEnveloppe::ValueRange valueRange) {
    _allowedValueRange = valueRange;
}

#pragma mark Display type setters / getters

/*
 *	Display type setters / getters
 */

VSCEnveloppeDisplayType VSCEnveloppeViewSetup::getDisplayType(void) const {
	return _displayType;
}

void VSCEnveloppeViewSetup::setDisplayType(const VSCEnveloppeDisplayType displayType) {
	_displayType = displayType;
}

#pragma mark -

#pragma mark Enveloppe Display Setups

VSCEnveloppeDisplaySetup& VSCEnveloppeViewSetup::getActiveDisplaySetup() {
    return _activeDisplaySetup;
}

VSCEnveloppeDisplaySetup& VSCEnveloppeViewSetup::getInactiveDisplaySetup() {
    return _inactiveDisplaySetup;
}


VSCEnveloppeDisplaySetup::VSCEnveloppeDisplaySetup() {
    this->setControlPointRadius(3.0);
    this->setLineWidth(1.0);
    this->setControlPointSelectedColour((VSCColour){0.0, 0.0, 1.0, 1.0});
    this->setControlPointUnselectedColour((VSCColour){0.0, 0.0, 0.0, 1.0});
    this->setLineColour((VSCColour){0.0, 0.0, 0.0, 1.0});
}

VSCEnveloppeDisplaySetup::VSCEnveloppeDisplaySetup(const VSCEnveloppeDisplaySetup& setup) {
    _controlPointRadius = setup._controlPointRadius;
    _lineWidth = setup._lineWidth;
    _lineColour = setup._lineColour;
    _controlPointSelectedColour = setup._controlPointSelectedColour;
    _controlPointUnselectedColour = setup._controlPointUnselectedColour;
}

#pragma mark Control Points setters / getters

/*
 *	Control Points setters / getters
 */

float VSCEnveloppeDisplaySetup::getControlPointRadius(void) const {
	return _controlPointRadius;
}

void VSCEnveloppeDisplaySetup::setControlPointRadius(float controlPointRadius) {
	_controlPointRadius = controlPointRadius;
}

VSCColour VSCEnveloppeDisplaySetup::getControlPointSelectedColour(void) const {
	return _controlPointSelectedColour;
}

void VSCEnveloppeDisplaySetup::setControlPointSelectedColour(VSCColour colour) {
	_controlPointSelectedColour = colour;
}

VSCColour VSCEnveloppeDisplaySetup::getControlPointUnselectedColour(void) const {
	return _controlPointUnselectedColour;
}

void VSCEnveloppeDisplaySetup::setControlPointUnselectedColour(VSCColour colour) {
	_controlPointUnselectedColour = colour;
}

float VSCEnveloppeDisplaySetup::getLineWidth(void) const {
	return _lineWidth;
}

void VSCEnveloppeDisplaySetup::setLineWidth(float lineWidth) {
	_lineWidth = lineWidth;
}

#pragma mark Enveloppe lines setters / getters

/*
 *	Enveloppe lines setters / getters
 */

VSCColour VSCEnveloppeDisplaySetup::getLineColour(void) const {
	return _lineColour;
}

void VSCEnveloppeDisplaySetup::setLineColour(VSCColour colour) {
	_lineColour = colour;
}
