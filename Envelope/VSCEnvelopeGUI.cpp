/*
 *  VSCOSXEnvelopeViewSetup.cpp
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

namespace VSC
{
    Float pointForTime(const Float t, const Envelope::TimeRange& timeRange, const Float width);
    Float pointForValue(const Float v, const Envelope::ValueRange& valueRange, const Float height);
    Float timeForPoint(const Float point, const Envelope::TimeRange& timeRange, const Float width);
    Float valueForPoint(const Float point, const Envelope::ValueRange& valueRange, const Float height);
    Float timeDeltaForPointDelta(Float delta, const Envelope::TimeRange& timeRange, const Float width);
    Float valueDeltaForPointDelta(Float delta, const Envelope::ValueRange& valueRange, const Float height);
}

using namespace VSC;


//MARK: EnvelopePoints Calculations

Float pointForTime(const Float t, const Envelope::TimeRange& timeRange, const Float width)
{
	Float timePerPixel = timeRange.size / width; 
	return (t - timeRange.origin) / timePerPixel;
}

Float pointForValue(const Float v, const Envelope::ValueRange& valueRange, const Float height)
{
	Float valuePerPixel = valueRange.size / height; 
	return (v - valueRange.origin) / valuePerPixel;
}

Float timeForPoint(const Float point, const Envelope::TimeRange& timeRange, const Float width)
{
	Float normalisedX = point / width;
	return timeRange.origin  + (normalisedX*timeRange.size);
}

Float valueForPoint(const Float point, const Envelope::ValueRange& valueRange, const Float height)
{
	Float normalisedY = (point / height);
	return valueRange.origin  + (normalisedY*valueRange.size);
}

Float timeDeltaForPointDelta(Float delta, const Envelope::TimeRange& timeRange, const Float width)
{
    Float normalisedDelta = (delta / width);
    return normalisedDelta*timeRange.size;
}

Float valueDeltaForPointDelta(Float delta, const Envelope::ValueRange& valueRange, const Float height)
{
    Float normalisedDelta = (delta / height);
	return normalisedDelta*valueRange.size;
}

EnvelopeEditorGUIConfig::EnvelopeEditorGUIConfig(void)
{
    this->setToDefault();
}

void EnvelopeEditorGUIConfig::setToDefault(void)
{
    _timeRange = Envelope::TimeRange(0.0,5.0);
    _valueRange = Envelope::ValueRange(0.0,1.0);
    mAllowedTimeRange = Envelope::TimeRange(0.0,5.0);
    mAllowedValueRange = Envelope::ValueRange(0.0,1.0);
}


#pragma mark Setters / Getters

/*
 *	View range setters / getters
 */

const Size& EnvelopeEditorGUIConfig::getEditorSize(void) const
{
    return _editorSize;
}

void EnvelopeEditorGUIConfig::setEditorSize(const Size& size)
{
    _editorSize = size;
}


const Envelope::TimeRange& EnvelopeEditorGUIConfig::getTimeRange(void) const
{
    return _timeRange;
}

void EnvelopeEditorGUIConfig::setTimeRange(const Envelope::TimeRange& timeRange)
{
    _timeRange = timeRange;
}

const Envelope::ValueRange& EnvelopeEditorGUIConfig::getValueRange(void) const
{
    return _valueRange;
}

void EnvelopeEditorGUIConfig::setValueRange(const Envelope::ValueRange& valueRange)
{
    _valueRange = valueRange;
}

/*
 *	Allowed range setters / getters
 */

const Envelope::TimeRange& EnvelopeEditorGUIConfig::getAllowedTimeRange(void) const
{
    return mAllowedTimeRange;
}

void EnvelopeEditorGUIConfig::setAllowedTimeRange(const Envelope::TimeRange& timeRange)
{
    mAllowedTimeRange = timeRange;
}

const Envelope::ValueRange& EnvelopeEditorGUIConfig::getAllowedValueRange(void) const
{
    return mAllowedValueRange;
}

void EnvelopeEditorGUIConfig::setAllowedValueRange(const Envelope::ValueRange& valueRange)
{
    mAllowedValueRange = valueRange;
}

Float EnvelopeEditorGUIConfig::getPointSelectionRadius(void) const
{
    return _pointSelectionRadius;
}

void EnvelopeEditorGUIConfig::setPointSelectionRadius(const Float radius)
{
    _pointSelectionRadius = radius;
}

const Color& EnvelopeEditorGUIConfig::getSelectionRectColour(void) const 
{
    return _selectionRectColour;
}

void EnvelopeEditorGUIConfig::setSelectionRectColour(const Color& colour)
{
    _selectionRectColour = colour;
}

Float EnvelopeEditorGUIConfig::getSelectionRectLineWidth(void) const
{
    return _selectionRectLineWidth;
}

void EnvelopeEditorGUIConfig::setSelectionRectLineWidth(const Float width)
{
    _selectionRectLineWidth = width;
}


#pragma mark Point Calculations

Float EnvelopeEditorGUIConfig::pointForTime(const Float t)
{
    return ::pointForTime(t, _timeRange, _editorSize.width);
}

Float EnvelopeEditorGUIConfig::pointForValue(const Float v)
{
    return ::pointForValue(v, _valueRange, _editorSize.height);
}

Float EnvelopeEditorGUIConfig::timeForPoint(const Float point)
{
    return ::timeForPoint(point, _timeRange, _editorSize.width);
}

Float EnvelopeEditorGUIConfig::valueForPoint(const Float point)
{
    return ::valueForPoint(point, _valueRange, _editorSize.height);
}

Float EnvelopeEditorGUIConfig::timeDeltaForPointDelta(const Float delta)
{
    return ::timeDeltaForPointDelta(delta, _timeRange, _editorSize.width);
}

Float EnvelopeEditorGUIConfig::valueDeltaForPointDelta(const Float delta)
{
    return ::valueDeltaForPointDelta(delta, _valueRange, _editorSize.height);
}

Point EnvelopeEditorGUIConfig::pointForEnvelopeCoordinate(const EnvelopeCoordinate_SPtr& p)
{
    Point point;
    point.x = this->pointForTime(p->getTime());
    point.y = this->pointForValue(p->getValue());
    return point;
}

EnvelopeCoordinate_SPtr EnvelopeEditorGUIConfig::createEnvelopeCoordinateForPoint(const Point& p)
{
    EnvelopeCoordinate_SPtr coord = EnvelopeCoordinate_SPtr(new EnvelopeCoordinate());
    this->setEnvelopeCoordinateToPoint(coord, p);
    return coord;
}

void EnvelopeEditorGUIConfig::setEnvelopeCoordinateToPoint(EnvelopeCoordinate_SPtr coord, const Point& p)
{
    coord->setTime(this->timeForPoint(p.x));
    coord->setValue(this->valueForPoint(p.y));
}


#pragma mark -

#pragma mark Envelope Display Setups


EnvelopeGUIConfig::EnvelopeGUIConfig()
{
    this->setControlPointRadius(3.0);
    this->setLineWidth(1.0);
    this->setControlPointSelectedColour((Color){0.0, 0.0, 1.0, 1.0});
    this->setControlPointUnselectedColour((Color){0.0, 0.0, 0.0, 1.0});
    this->setLineSelectedColour((Color){0.0, 0.0, 0.0, 1.0});
    this->setLineUnselectedColour((Color){0.0, 0.0, 1.0, 1.0});
}

EnvelopeGUIConfig::EnvelopeGUIConfig(const EnvelopeGUIConfig& setup)
{
    _controlPointRadius = setup._controlPointRadius;
    _lineWidth = setup._lineWidth;
    _lineSelectedColour = setup._lineSelectedColour;
    _lineUnselectedColour = setup._lineUnselectedColour;
    _controlPointSelectedColour = setup._controlPointSelectedColour;
    _controlPointUnselectedColour = setup._controlPointUnselectedColour;
}

#pragma mark Control EnvelopePoints setters / getters

/*
 *	Control EnvelopePoints setters / getters
 */

Float EnvelopeGUIConfig::getControlPointRadius(void) const
{
	return _controlPointRadius;
}

void EnvelopeGUIConfig::setControlPointRadius(Float controlPointRadius)
{
	_controlPointRadius = controlPointRadius;
}

const Color& EnvelopeGUIConfig::getControlPointSelectedColour(void) const
{
	return _controlPointSelectedColour;
}

void EnvelopeGUIConfig::setControlPointSelectedColour(const Color& colour)
{
	_controlPointSelectedColour = colour;
}

const Color& EnvelopeGUIConfig::getControlPointUnselectedColour(void) const
{
	return _controlPointUnselectedColour;
}

void EnvelopeGUIConfig::setControlPointUnselectedColour(const Color& colour)
{
	_controlPointUnselectedColour = colour;
}

Float EnvelopeGUIConfig::getLineWidth(void) const
{
	return _lineWidth;
}

void EnvelopeGUIConfig::setLineWidth(Float lineWidth)
{
	_lineWidth = lineWidth;
}

#pragma mark Envelope lines setters / getters

/*
 *	Envelope lines setters / getters
 */

const Color& EnvelopeGUIConfig::getLineSelectedColour(void) const
{
	return _lineSelectedColour;
}

void EnvelopeGUIConfig::setLineSelectedColour(const Color& colour)
{
	_lineSelectedColour = colour;
}

const Color& EnvelopeGUIConfig::getLineUnselectedColour(void) const
{
    return _lineUnselectedColour;
}

void EnvelopeGUIConfig::setLineUnselectedColour(const Color& colour)
{
    _lineUnselectedColour = colour;
}
