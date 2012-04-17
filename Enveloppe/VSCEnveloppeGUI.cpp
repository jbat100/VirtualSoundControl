/*
 *  VSCEnveloppeViewSetup.cpp
 *  EnveloppeEditor
 *
 *  Created by Jonathan Thorpe on 02/08/2011.
 *  Copyright 2011 JBAT. All rights reserved.
 *
 */

#include "VSCEnveloppeGUI.h"
#include "VSCSound.h"
#include "VSCException.h"

#include <cmath>

VSCSFloat pointForTime(const VSCSFloat t, const VSCEnveloppe::TimeRange& timeRange, const VSCSFloat width) {
    return 0.0;
}

VSCSFloat pointForValue(const VSCSFloat v, const VSCEnveloppe::ValueRange& valueRange, const VSCSFloat height) {
    return 0.0;
}

VSCSFloat timeForPoint(const VSCSFloat point, const VSCEnveloppe::TimeRange& timeRange, const VSCSFloat width) {
    return 0.0;
}

VSCSFloat valueForPoint(const VSCSFloat point, const VSCEnveloppe::ValueRange& valueRange, const VSCSFloat height) {
    return 0.0;
}


VSCEnveloppeEditorGUIConfig::VSCEnveloppeEditorGUIConfig(void) {
	setToDefault();
}

void VSCEnveloppeEditorGUIConfig::setToDefault(void) {
    this->setTimeRange(VSCEnveloppe::TimeRange(0.0,5.0));
    this->setValueRange(VSCEnveloppe::ValueRange(0.0,1.0));
    this->setAllowedTimeRange(VSCEnveloppe::TimeRange(0.0,5.0));
    this->setAllowedValueRange(VSCEnveloppe::ValueRange(0.0, 1.0));
}


#pragma mark Setters / Getters

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

const VSCSFloat VSCEnveloppeEditorGUIConfig::getPointSelectionRadius(void) const {
    
}

void VSCEnveloppeEditorGUIConfig::setPointSelectionRadius(const VSCSFloat radius) {
    
}

#pragma mark Point Calculations

VSCSFloat VSCEnveloppeEditorGUIConfig::pointForTime(const VSCSFloat t) {
    return ::pointForTime(t, _timeRange, _editorSize.width);
}

VSCSFloat VSCEnveloppeEditorGUIConfig::pointForValue(const VSCSFloat v) {
    return ::pointForValue(v, _valueRange, _editorSize.height);
}

VSCSFloat VSCEnveloppeEditorGUIConfig::timeForPoint(const VSCSFloat point) {
    return ::timeForPoint(point, _timeRange, _editorSize.width);
}

VSCSFloat VSCEnveloppeEditorGUIConfig::valueForPoint(const VSCSFloat point) {
    return ::valueForPoint(point, _valueRange, _editorSize.height);
}

VSC::Point VSCEnveloppeEditorGUIConfig::pointForEnveloppeCoordinate(const VSCEnveloppeCoordinatePtr& p) {
    VSC::Point point;
    point.x = this->pointForTime(p->getTime());
    point.y = this->pointForValue(p->getValue());
    return point;
}

VSCEnveloppeCoordinatePtr VSCEnveloppeEditorGUIConfig::enveloppeCoordinateForPoint(const VSC::Point& p) {
    VSCEnveloppeCoordinatePtr coord = VSCEnveloppeCoordinatePtr(new VSCEnveloppeCoordinate());
    coord->setTime(this->timeForPoint(p.x));
    coord->setValue(this->valueForPoint(p.y));
    return coord;
}


#pragma mark -

#pragma mark Enveloppe Display Setups


VSCEnveloppeGUIConfig::VSCEnveloppeGUIConfig() {
    this->setControlPointRadius(3.0);
    this->setLineWidth(1.0);
    this->setControlPointSelectedColour((VSCColour){0.0, 0.0, 1.0, 1.0});
    this->setControlPointUnselectedColour((VSCColour){0.0, 0.0, 0.0, 1.0});
    this->setLineSelectedColour((VSCColour){0.0, 0.0, 0.0, 1.0});
    this->setLineUnselectedColour((VSCColour){0.0, 0.0, 1.0, 1.0});
}

VSCEnveloppeGUIConfig::VSCEnveloppeGUIConfig(const VSCEnveloppeGUIConfig& setup) {
    _controlPointRadius = setup._controlPointRadius;
    _lineWidth = setup._lineWidth;
    _lineSelectedColour = setup._lineSelectedColour;
    _lineUnselectedColour = setup._lineUnselectedColour;
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

const VSCColour& VSCEnveloppeGUIConfig::getControlPointSelectedColour(void) const {
	return _controlPointSelectedColour;
}

void VSCEnveloppeGUIConfig::setControlPointSelectedColour(const VSCColour& colour) {
	_controlPointSelectedColour = colour;
}

const VSCColour& VSCEnveloppeGUIConfig::getControlPointUnselectedColour(void) const {
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

const VSCColour& VSCEnveloppeGUIConfig::getLineSelectedColour(void) const {
	return _lineSelectedColour;
}

void VSCEnveloppeGUIConfig::setLineSelectedColour(const VSCColour& colour) {
	_lineSelectedColour = colour;
}

const VSCColour& VSCEnveloppeGUIConfig::getLineUnselectedColour(void) const {
    return _lineUnselectedColour;
}

void VSCEnveloppeGUIConfig::setLineUnselectedColour(const VSCColour& colour) {
    _lineUnselectedColour = colour;
}
