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

namespace VSC {
    Float pointForTime(const Float t, const Enveloppe::TimeRange& timeRange, const Float width);
    Float pointForValue(const Float v, const Enveloppe::ValueRange& valueRange, const Float height);
    Float timeForPoint(const Float point, const Enveloppe::TimeRange& timeRange, const Float width);
    Float valueForPoint(const Float point, const Enveloppe::ValueRange& valueRange, const Float height);
    Float timeDeltaForPointDelta(Float delta, const Enveloppe::TimeRange& timeRange, const Float width);
    Float valueDeltaForPointDelta(Float delta, const Enveloppe::ValueRange& valueRange, const Float height);
}


//MARK: Points Calculations

VSC::Float VSC::pointForTime(const Float t, const Enveloppe::TimeRange& timeRange, const Float width) {
	Float timePerPixel = timeRange.size / width; 
	return (t - timeRange.origin) / timePerPixel;
}

VSC::Float VSC::pointForValue(const Float v, const Enveloppe::ValueRange& valueRange, const Float height) {
	Float valuePerPixel = valueRange.size / height; 
	return (v - valueRange.origin) / valuePerPixel;
}

VSC::Float VSC::timeForPoint(const Float point, const Enveloppe::TimeRange& timeRange, const Float width) {
	Float normalisedX = point / width;
	return timeRange.origin  + (normalisedX*timeRange.size);
}

VSC::Float VSC::valueForPoint(const Float point, const Enveloppe::ValueRange& valueRange, const Float height) {
	Float normalisedY = (point / height);
	return valueRange.origin  + (normalisedY*valueRange.size);
}

VSC::Float VSC::timeDeltaForPointDelta(Float delta, const Enveloppe::TimeRange& timeRange, const Float width) {
    Float normalisedDelta = (delta / width);
    return normalisedDelta*timeRange.size;
}

VSC::Float VSC::valueDeltaForPointDelta(Float delta, const Enveloppe::ValueRange& valueRange, const Float height) {
    Float normalisedDelta = (delta / height);
	return normalisedDelta*valueRange.size;
}

VSC::EnveloppeEditorGUIConfig::EnveloppeEditorGUIConfig(void) {
    this->setToDefault();
}

void VSC::EnveloppeEditorGUIConfig::setToDefault(void) {
    _timeRange = Enveloppe::TimeRange(0.0,5.0);
    _valueRange = Enveloppe::ValueRange(0.0,1.0);
    _allowedTimeRange = Enveloppe::TimeRange(0.0,5.0);
    _allowedValueRange = Enveloppe::ValueRange(0.0,1.0);
}


#pragma mark Setters / Getters

/*
 *	View range setters / getters
 */

const VSC::Size& VSC::EnveloppeEditorGUIConfig::getEditorSize(void) const {
    return _editorSize;
}

void VSC::EnveloppeEditorGUIConfig::setEditorSize(const Size& size) {
    _editorSize = size;
}


const VSC::Enveloppe::TimeRange& VSC::EnveloppeEditorGUIConfig::getTimeRange(void) const {
    return _timeRange;
}

void VSC::EnveloppeEditorGUIConfig::setTimeRange(const Enveloppe::TimeRange& timeRange) {
    _timeRange = timeRange;
}

const VSC::Enveloppe::ValueRange& VSC::EnveloppeEditorGUIConfig::getValueRange(void) const {
    return _valueRange;
}

void VSC::EnveloppeEditorGUIConfig::setValueRange(const Enveloppe::ValueRange& valueRange) {
    _valueRange = valueRange;
}

/*
 *	Allowed range setters / getters
 */

const VSC::Enveloppe::TimeRange& VSC::EnveloppeEditorGUIConfig::getAllowedTimeRange(void) const {
    return _allowedTimeRange;
}

void VSC::EnveloppeEditorGUIConfig::setAllowedTimeRange(const Enveloppe::TimeRange& timeRange) {
    _allowedTimeRange = timeRange;
}

const VSC::Enveloppe::ValueRange& VSC::EnveloppeEditorGUIConfig::getAllowedValueRange(void) const {
    return _allowedValueRange;
}

void VSC::EnveloppeEditorGUIConfig::setAllowedValueRange(const Enveloppe::ValueRange& valueRange) {
    _allowedValueRange = valueRange;
}

Float VSC::EnveloppeEditorGUIConfig::getPointSelectionRadius(void) const {
    return _pointSelectionRadius;
}

void VSC::EnveloppeEditorGUIConfig::setPointSelectionRadius(const Float radius) {
    _pointSelectionRadius = radius;
}

const VSC::Color& VSC::EnveloppeEditorGUIConfig::getSelectionRectColour(void) const 
{
    return _selectionRectColour;
}

void VSC::EnveloppeEditorGUIConfig::setSelectionRectColour(const Color& colour)
{
    _selectionRectColour = colour;
}

Float VSC::EnveloppeEditorGUIConfig::getSelectionRectLineWidth(void) const
{
    return _selectionRectLineWidth;
}

void VSC::EnveloppeEditorGUIConfig::setSelectionRectLineWidth(const Float width)
{
    _selectionRectLineWidth = width;
}


#pragma mark Point Calculations

Float VSC::EnveloppeEditorGUIConfig::pointForTime(const Float t) {
    return ::pointForTime(t, _timeRange, _editorSize.width);
}

Float VSC::EnveloppeEditorGUIConfig::pointForValue(const Float v) {
    return ::pointForValue(v, _valueRange, _editorSize.height);
}

Float VSC::EnveloppeEditorGUIConfig::timeForPoint(const Float point) {
    return ::timeForPoint(point, _timeRange, _editorSize.width);
}

Float VSC::EnveloppeEditorGUIConfig::valueForPoint(const Float point) {
    return ::valueForPoint(point, _valueRange, _editorSize.height);
}

Float VSC::EnveloppeEditorGUIConfig::timeDeltaForPointDelta(const Float delta) {
    return ::timeDeltaForPointDelta(delta, _timeRange, _editorSize.width);
}

Float VSC::EnveloppeEditorGUIConfig::valueDeltaForPointDelta(const Float delta) {
    return ::valueDeltaForPointDelta(delta, _valueRange, _editorSize.height);
}

VSC::Point VSC::EnveloppeEditorGUIConfig::pointForEnveloppeCoordinate(const EnveloppeCoordinate::SPtr& p) {
    Point point;
    point.x = this->pointForTime(p->getTime());
    point.y = this->pointForValue(p->getValue());
    return point;
}

VSC::EnveloppeCoordinate::SPtr VSC::EnveloppeEditorGUIConfig::createEnveloppeCoordinateForPoint(const Point& p) {
    EnveloppeCoordinate::SPtr coord = EnveloppeCoordinate::SPtr(new EnveloppeCoordinate());
    this->setEnveloppeCoordinateToPoint(coord, p);
    return coord;
}

void VSC::EnveloppeEditorGUIConfig::setEnveloppeCoordinateToPoint(EnveloppeCoordinate::SPtr coord, const Point& p) {
    coord->setTime(this->timeForPoint(p.x));
    coord->setValue(this->valueForPoint(p.y));
}


#pragma mark -

#pragma mark Enveloppe Display Setups


VSC::EnveloppeGUIConfig::EnveloppeGUIConfig() {
    this->setControlPointRadius(3.0);
    this->setLineWidth(1.0);
    this->setControlPointSelectedColour((Color){0.0, 0.0, 1.0, 1.0});
    this->setControlPointUnselectedColour((Color){0.0, 0.0, 0.0, 1.0});
    this->setLineSelectedColour((Color){0.0, 0.0, 0.0, 1.0});
    this->setLineUnselectedColour((Color){0.0, 0.0, 1.0, 1.0});
}

VSC::EnveloppeGUIConfig::EnveloppeGUIConfig(const EnveloppeGUIConfig& setup) {
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

float VSC::EnveloppeGUIConfig::getControlPointRadius(void) const {
	return _controlPointRadius;
}

void VSC::EnveloppeGUIConfig::setControlPointRadius(float controlPointRadius) {
	_controlPointRadius = controlPointRadius;
}

const VSC::Color& VSC::EnveloppeGUIConfig::getControlPointSelectedColour(void) const {
	return _controlPointSelectedColour;
}

void VSC::EnveloppeGUIConfig::setControlPointSelectedColour(const Color& colour) {
	_controlPointSelectedColour = colour;
}

const VSC::Color& VSC::EnveloppeGUIConfig::getControlPointUnselectedColour(void) const {
	return _controlPointUnselectedColour;
}

void VSC::EnveloppeGUIConfig::setControlPointUnselectedColour(const VSC::Color& colour) {
	_controlPointUnselectedColour = colour;
}

float VSC::EnveloppeGUIConfig::getLineWidth(void) const {
	return _lineWidth;
}

void VSC::EnveloppeGUIConfig::setLineWidth(float lineWidth) {
	_lineWidth = lineWidth;
}

#pragma mark Enveloppe lines setters / getters

/*
 *	Enveloppe lines setters / getters
 */

const VSC::Color& VSC::EnveloppeGUIConfig::getLineSelectedColour(void) const {
	return _lineSelectedColour;
}

void VSC::EnveloppeGUIConfig::setLineSelectedColour(const Color& colour) {
	_lineSelectedColour = colour;
}

const VSC::Color& VSC::EnveloppeGUIConfig::getLineUnselectedColour(void) const {
    return _lineUnselectedColour;
}

void VSC::EnveloppeGUIConfig::setLineUnselectedColour(const Color& colour) {
    _lineUnselectedColour = colour;
}
