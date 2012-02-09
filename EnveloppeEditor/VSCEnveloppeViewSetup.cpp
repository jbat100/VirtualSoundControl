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

#pragma mark Encveloppe point calculations

int VSCEnveloppeViewSetup::pixelForTime(const VSCSFloat t, const int widthInPixels) const {
    throw VSCNotImplementedException();
	return 0;
}

int VSCEnveloppeViewSetup::pixelForValue(const VSCSFloat t, const int heightInPixels) const {
    throw VSCNotImplementedException();
	return 0;
}

VSCSFloat VSCEnveloppeViewSetup::timeForPixel(const int pixel, const int widthInPixels) const {
    throw VSCNotImplementedException();
	return 0.0;
}

VSCSFloat VSCEnveloppeViewSetup::valueForPixel(const int pixel, int const widthInPixels) const {
    throw VSCNotImplementedException();
	return 0.0;
}

#pragma mark View range setters / getters

/*
 *	View range setters / getters
 */
VSCEnveloppe::TimeRange VSCEnveloppeViewSetup::getTimeRange(void) const {
    return _timeRange;
}

void VSCEnveloppeViewSetup::setTimeRange(const VSCEnveloppe::TimeRange timeRange) {
    _timeRange = timeRange;
}

VSCEnveloppe::ValueRange VSCEnveloppeViewSetup::getValueRange(void) const {
    return _valueRange;
}

void VSCEnveloppeViewSetup::setValueRange(const VSCEnveloppe::ValueRange valueRange) {
    _valueRange = valueRange;
}

/*
 *	Allowed range setters / getters
 */

VSCEnveloppe::TimeRange VSCEnveloppeViewSetup::getAllowedTimeRange(void) const {
    return _allowedTimeRange;
}

void VSCEnveloppeViewSetup::setAllowedTimeRange(const VSCEnveloppe::TimeRange timeRange) {
    _allowedTimeRange = timeRange;
}

VSCEnveloppe::ValueRange VSCEnveloppeViewSetup::getAllowedValueRange(void) const {
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

#pragma mark Enveloppe Display Setups

VSCEnveloppeDisplaySetup& VSCEnveloppeViewSetup::getActiveDisplaySetup() {
    return _activeDisplaySetup;
}

VSCEnveloppeDisplaySetup& VSCEnveloppeViewSetup::getInactiveDisplaySetup() {
    return _inactiveDisplaySetup;
}

