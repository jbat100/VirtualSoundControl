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

VSCSFloat pointForTime(const VSCSFloat t, const VSCEnveloppe::TimeRange timeRange, const VSCSFloat width) {
    
}

VSCSFloat pointForValue(const VSCSFloat v, const VSCEnveloppe::ValueRange valueRange, const VSCSFloat height) {
    
}

VSCSFloat timeForPoint(const VSCSFloat point, const VSCEnveloppe::TimeRange timeRange, const VSCSFloat width) {
    
}

VSCSFloat valueForPoint(const VSCSFloat point, const VSCEnveloppe::ValueRange valueRange, const VSCSFloat height) {
    
}

#pragma mark View range setters / getters

VSCEnveloppeEditorGUIConfig::VSCEnveloppeEditorGUIConfig(void) {
	setToDefault();
}

void VSCEnveloppeEditorGUIConfig::setToDefault(void) {
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
const VSCEnveloppe::TimeRange& VSCEnveloppeEditorGUIConfig::getTimeRange(void) const {
    return _timeRange;
}

void VSCEnveloppeEditorGUIConfig::setTimeRange(const VSCEnveloppe::TimeRange& timeRange) {
    _timeRange = timeRange;
}

const VSCEnveloppe::ValueRange& VSCEnveloppeEditorGUIConfig::getValueRange(void) const {
    return _valueRange;
}

void VSCEnveloppeEditorGUIConfig::setValueRange(const VSCEnveloppe::ValueRange& valueRange) {
    _valueRange = valueRange;
}

/*
 *	Allowed range setters / getters
 */

const VSCEnveloppe::TimeRange& VSCEnveloppeEditorGUIConfig::getAllowedTimeRange(void) const {
    return _allowedTimeRange;
}

void VSCEnveloppeEditorGUIConfig::setAllowedTimeRange(const VSCEnveloppe::TimeRange& timeRange) {
    _allowedTimeRange = timeRange;
}

const VSCEnveloppe::ValueRange& VSCEnveloppeEditorGUIConfig::getAllowedValueRange(void) const {
    return _allowedValueRange;
}

void VSCEnveloppeEditorGUIConfig::setAllowedValueRange(const VSCEnveloppe::ValueRange& valueRange) {
    _allowedValueRange = valueRange;
}

#pragma mark Display type setters / getters

/*
 *	Display type setters / getters
 */

VSCEnveloppeDisplayType VSCEnveloppeEditorGUIConfig::getDisplayType(void) const {
	return _displayType;
}

void VSCEnveloppeEditorGUIConfig::setDisplayType(const VSCEnveloppeDisplayType displayType) {
	_displayType = displayType;
}

#pragma mark -

#pragma mark Enveloppe Display Setups

VSCEnveloppeGUIConfig& VSCEnveloppeEditorGUIConfig::getActiveDisplaySetup() {
    return _activeDisplaySetup;
}

VSCEnveloppeGUIConfig& VSCEnveloppeEditorGUIConfig::getInactiveDisplaySetup() {
    return _inactiveDisplaySetup;
}


VSCEnveloppeGUIConfig::VSCEnveloppeGUIConfig() {
    this->setControlPointRadius(3.0);
    this->setLineWidth(1.0);
    this->setControlPointSelectedColour((VSCColour){0.0, 0.0, 1.0, 1.0});
    this->setControlPointUnselectedColour((VSCColour){0.0, 0.0, 0.0, 1.0});
    this->setLineColour((VSCColour){0.0, 0.0, 0.0, 1.0});
}

VSCEnveloppeGUIConfig::VSCEnveloppeGUIConfig(const VSCEnveloppeGUIConfig& setup) {
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

float VSCEnveloppeGUIConfig::getControlPointRadius(void) const {
	return _controlPointRadius;
}

void VSCEnveloppeGUIConfig::setControlPointRadius(float controlPointRadius) {
	_controlPointRadius = controlPointRadius;
}

VSCColour VSCEnveloppeGUIConfig::getControlPointSelectedColour(void) const {
	return _controlPointSelectedColour;
}

void VSCEnveloppeGUIConfig::setControlPointSelectedColour(const VSCColour& colour) {
	_controlPointSelectedColour = colour;
}

VSCColour VSCEnveloppeGUIConfig::getControlPointUnselectedColour(void) const {
	return _controlPointUnselectedColour;
}

void VSCEnveloppeGUIConfig::setControlPointUnselectedColour(const VSCColour& colour) {
	_controlPointUnselectedColour = colour;
}

float VSCEnveloppeGUIConfig::getLineWidth(void) const {
	return _lineWidth;
}

void VSCEnveloppeGUIConfig::setLineWidth(float lineWidth) {
	_lineWidth = lineWidth;
}

#pragma mark Enveloppe lines setters / getters

/*
 *	Enveloppe lines setters / getters
 */

VSCColour VSCEnveloppeGUIConfig::getLineSelectedColour(void) const {
	return _lineSelectedColour;
}

void VSCEnveloppeGUIConfig::setLineSelectedColour(const VSCColour& colour) {
	_lineSelectedColour = colour;
}

VSCColour getLineUnselectedColour(void) const {
    return _lineUnselectedColour;
}

void setLineUnselectedColour(const VSCColour& colour) {
    _lineUnselectedColour = colour;
}
