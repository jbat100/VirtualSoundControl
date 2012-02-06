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

#include <cmath>

#pragma mark View range setters / getters

VSCEnveloppeViewSetup::VSCEnveloppeViewSetup(void) {
	setToDefault();
}

void VSCEnveloppeViewSetup::setToDefault(void) {
	this->setDisplayType(VSCEnveloppeDisplayTypeDefault);
}

#pragma mark Encveloppe point calculations

int VSCEnveloppeViewSetup::pixelForTime(VSCSFloat t, int widthInPixels) const {
	return 0;
}

int VSCEnveloppeViewSetup::pixelForValue(VSCSFloat t, int heightInPixels) const {
	return 0;
}

VSCSFloat VSCEnveloppeViewSetup::timeForPixel(int pixel, int widthInPixels) const {
	return 0.0;
}

VSCSFloat VSCEnveloppeViewSetup::valueForPixel(int pixel, int widthInPixels) const {
	return 0.0;
}

#pragma mark View range setters / getters

/*
 *	View range setters / getters
 */
VSCEnveloppe::TimeRange VSCEnveloppeViewSetup::getTimeRange(void) const {
    return _timeRange;
}

void VSCEnveloppeViewSetup::setTimeRange(VSCEnveloppe::TimeRange timeRange) {
    _timeRange = timeRange;
}

VSCEnveloppe::ValueRange VSCEnveloppeViewSetup::getValueRange(void) const {
    return _valueRange;
}

void VSCEnveloppeViewSetup::setValueRange(VSCEnveloppe::ValueRange valueRange) {
    _valueRange = valueRange;
}

/*
 *	Allowed range setters / getters
 */

VSCEnveloppe::TimeRange VSCEnveloppeViewSetup::getAllowedTimeRange(void) const {
    return _allowedTimeRange;
}

void VSCEnveloppeViewSetup::setAllowedTimeRange(VSCEnveloppe::TimeRange timeRange) {
    _allowedTimeRange = timeRange;
}

VSCEnveloppe::ValueRange VSCEnveloppeViewSetup::getAllowedValueRange(void) const {
    return _allowedValueRange;
}

void VSCEnveloppeViewSetup::setAllowedValueRange(VSCEnveloppe::ValueRange valueRange) {
    _allowedValueRange = valueRange;
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


