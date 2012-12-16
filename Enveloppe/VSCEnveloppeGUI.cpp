/*
 *  VSCEnvelopeViewSetup.cpp
 *  EnvelopeEditor
 *
 *  Created by Jonathan Thorpe on 02/08/2011.
 *  Copyright 2011 JBAT. All rights reserved.
 *
 */

#include "VSCEnvelopeGUI.h"
#include "VSCSound.h"
#include "VSCException.h"

#include <cmath>

namespace VSC {
    Float pointForTime(const Float t, const Envelope::TimeRange& timeRange, const Float width);
    Float pointForValue(const Float v, const Envelope::ValueRange& valueRange, const Float height);
    Float timeForPoint(const Float point, const Envelope::TimeRange& timeRange, const Float width);
    Float valueForPoint(const Float point, const Envelope::ValueRange& valueRange, const Float height);
    Float timeDeltaForPointDelta(Float delta, const Envelope::TimeRange& timeRange, const Float width);
    Float valueDeltaForPointDelta(Float delta, const Envelope::ValueRange& valueRange, const Float height);
}


//MARK: Points Calculations

VSC::Float VSC::pointForTime(const Float t, const Envelope::TimeRange& timeRange, const Float width) {
	Float timePerPixel = timeRange.size / width; 
	return (t - timeRange.origin) / timePerPixel;
}

VSC::Float VSC::pointForValue(const Float v, const Envelope::ValueRange& valueRange, const Float height) {
	Float valuePerPixel = valueRange.size / height; 
	return (v - valueRange.origin) / valuePerPixel;
}

VSC::Float VSC::timeForPoint(const Float point, const Envelope::TimeRange& timeRange, const Float width) {
	Float normalisedX = point / width;
	return timeRange.origin  + (normalisedX*timeRange.size);
}

VSC::Float VSC::valueForPoint(const Float point, const Envelope::ValueRange& valueRange, const Float height) {
	Float normalisedY = (point / height);
	return valueRange.origin  + (normalisedY*valueRange.size);
}

VSC::Float VSC::timeDeltaForPointDelta(Float delta, const Envelope::TimeRange& timeRange, const Float width) {
    Float normalisedDelta = (delta / width);
    return normalisedDelta*timeRange.size;
}

VSC::Float VSC::valueDeltaForPointDelta(Float delta, const Envelope::ValueRange& valueRange, const Float height) {
    Float normalisedDelta = (delta / height);
	return normalisedDelta*valueRange.size;
}

VSC::EnvelopeEditorGUIConfig::EnvelopeEditorGUIConfig(void) {
    this->setToDefault();
}

void VSC::EnvelopeEditorGUIConfig::setToDefault(void) {
    _timeRange = Envelope::TimeRange(0.0,5.0);
    _valueRange = Envelope::ValueRange(0.0,1.0);
    _allowedTimeRange = Envelope::TimeRange(0.0,5.0);
    _allowedValueRange = Envelope::ValueRange(0.0,1.0);
}


#pragma mark Setters / Getters

/*
 *	View range setters / getters
 */

const VSC::Size& VSC::EnvelopeEditorGUIConfig::getEditorSize(void) const {
    return _editorSize;
}

void VSC::EnvelopeEditorGUIConfig::setEditorSize(const Size& size) {
    _editorSize = size;
}


const VSC::Envelope::TimeRange& VSC::EnvelopeEditorGUIConfig::getTimeRange(void) const {
    return _timeRange;
}

void VSC::EnvelopeEditorGUIConfig::setTimeRange(const Envelope::TimeRange& timeRange) {
    _timeRange = timeRange;
}

const VSC::Envelope::ValueRange& VSC::EnvelopeEditorGUIConfig::getValueRange(void) const {
    return _valueRange;
}

void VSC::EnvelopeEditorGUIConfig::setValueRange(const Envelope::ValueRange& valueRange) {
    _valueRange = valueRange;
}

/*
 *	Allowed range setters / getters
 */

const VSC::Envelope::TimeRange& VSC::EnvelopeEditorGUIConfig::getAllowedTimeRange(void) const {
    return _allowedTimeRange;
}

void VSC::EnvelopeEditorGUIConfig::setAllowedTimeRange(const Envelope::TimeRange& timeRange) {
    _allowedTimeRange = timeRange;
}

const VSC::Envelope::ValueRange& VSC::EnvelopeEditorGUIConfig::getAllowedValueRange(void) const {
    return _allowedValueRange;
}

void VSC::EnvelopeEditorGUIConfig::setAllowedValueRange(const Envelope::ValueRange& valueRange) {
    _allowedValueRange = valueRange;
}

Float VSC::EnvelopeEditorGUIConfig::getPointSelectionRadius(void) const {
    return _pointSelectionRadius;
}

void VSC::EnvelopeEditorGUIConfig::setPointSelectionRadius(const Float radius) {
    _pointSelectionRadius = radius;
}

const VSC::Color& VSC::EnvelopeEditorGUIConfig::getSelectionRectColour(void) const 
{
    return _selectionRectColour;
}

void VSC::EnvelopeEditorGUIConfig::setSelectionRectColour(const Color& colour)
{
    _selectionRectColour = colour;
}

Float VSC::EnvelopeEditorGUIConfig::getSelectionRectLineWidth(void) const
{
    return _selectionRectLineWidth;
}

void VSC::EnvelopeEditorGUIConfig::setSelectionRectLineWidth(const Float width)
{
    _selectionRectLineWidth = width;
}


#pragma mark Point Calculations

Float VSC::EnvelopeEditorGUIConfig::pointForTime(const Float t) {
    return ::pointForTime(t, _timeRange, _editorSize.width);
}

Float VSC::EnvelopeEditorGUIConfig::pointForValue(const Float v) {
    return ::pointForValue(v, _valueRange, _editorSize.height);
}

Float VSC::EnvelopeEditorGUIConfig::timeForPoint(const Float point) {
    return ::timeForPoint(point, _timeRange, _editorSize.width);
}

Float VSC::EnvelopeEditorGUIConfig::valueForPoint(const Float point) {
    return ::valueForPoint(point, _valueRange, _editorSize.height);
}

Float VSC::EnvelopeEditorGUIConfig::timeDeltaForPointDelta(const Float delta) {
    return ::timeDeltaForPointDelta(delta, _timeRange, _editorSize.width);
}

Float VSC::EnvelopeEditorGUIConfig::valueDeltaForPointDelta(const Float delta) {
    return ::valueDeltaForPointDelta(delta, _valueRange, _editorSize.height);
}

VSC::Point VSC::EnvelopeEditorGUIConfig::pointForEnvelopeCoordinate(const EnvelopeCoordinate::SPtr& p) {
    Point point;
    point.x = this->pointForTime(p->getTime());
    point.y = this->pointForValue(p->getValue());
    return point;
}

VSC::EnvelopeCoordinate::SPtr VSC::EnvelopeEditorGUIConfig::createEnvelopeCoordinateForPoint(const Point& p) {
    EnvelopeCoordinate::SPtr coord = EnvelopeCoordinate::SPtr(new EnvelopeCoordinate());
    this->setEnvelopeCoordinateToPoint(coord, p);
    return coord;
}

void VSC::EnvelopeEditorGUIConfig::setEnvelopeCoordinateToPoint(EnvelopeCoordinate::SPtr coord, const Point& p) {
    coord->setTime(this->timeForPoint(p.x));
    coord->setValue(this->valueForPoint(p.y));
}


#pragma mark -

#pragma mark Envelope Display Setups


VSC::EnvelopeGUIConfig::EnvelopeGUIConfig() {
    this->setControlPointRadius(3.0);
    this->setLineWidth(1.0);
    this->setControlPointSelectedColour((Color){0.0, 0.0, 1.0, 1.0});
    this->setControlPointUnselectedColour((Color){0.0, 0.0, 0.0, 1.0});
    this->setLineSelectedColour((Color){0.0, 0.0, 0.0, 1.0});
    this->setLineUnselectedColour((Color){0.0, 0.0, 1.0, 1.0});
}

VSC::EnvelopeGUIConfig::EnvelopeGUIConfig(const EnvelopeGUIConfig& setup) {
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

float VSC::EnvelopeGUIConfig::getControlPointRadius(void) const {
	return _controlPointRadius;
}

void VSC::EnvelopeGUIConfig::setControlPointRadius(float controlPointRadius) {
	_controlPointRadius = controlPointRadius;
}

const VSC::Color& VSC::EnvelopeGUIConfig::getControlPointSelectedColour(void) const {
	return _controlPointSelectedColour;
}

void VSC::EnvelopeGUIConfig::setControlPointSelectedColour(const Color& colour) {
	_controlPointSelectedColour = colour;
}

const VSC::Color& VSC::EnvelopeGUIConfig::getControlPointUnselectedColour(void) const {
	return _controlPointUnselectedColour;
}

void VSC::EnvelopeGUIConfig::setControlPointUnselectedColour(const VSC::Color& colour) {
	_controlPointUnselectedColour = colour;
}

float VSC::EnvelopeGUIConfig::getLineWidth(void) const {
	return _lineWidth;
}

void VSC::EnvelopeGUIConfig::setLineWidth(float lineWidth) {
	_lineWidth = lineWidth;
}

#pragma mark Envelope lines setters / getters

/*
 *	Envelope lines setters / getters
 */

const VSC::Color& VSC::EnvelopeGUIConfig::getLineSelectedColour(void) const {
	return _lineSelectedColour;
}

void VSC::EnvelopeGUIConfig::setLineSelectedColour(const Color& colour) {
	_lineSelectedColour = colour;
}

const VSC::Color& VSC::EnvelopeGUIConfig::getLineUnselectedColour(void) const {
    return _lineUnselectedColour;
}

void VSC::EnvelopeGUIConfig::setLineUnselectedColour(const Color& colour) {
    _lineUnselectedColour = colour;
}
