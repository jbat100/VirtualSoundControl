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
	
	setMinTime(0.0);
	setMaxTime(5.0);
	setMinValue(0.0);
	setMaxValue(1.0);
	
	setDisplayType(VSCEnveloppeDisplayTypeDefault);
	
	setControlPointRadius(3.0);
	setLineWidth(1.0);
	
	setControlPointSelectedColour((VSCColour){0.0, 0.0, 1.0, 1.0});
	setControlPointUnselectedColour((VSCColour){0.0, 0.0, 0.0, 1.0});
	setLineColour((VSCColour){0.0, 0.0, 0.0, 1.0});
	
	setTargetNumberOfHorizontalGridLines(5);
	setTargetNumberOfVerticalGridLines(10);
	
	setVerticalGridLineColor((VSCColour){0.8, 0.8, 0.8, 1.0});
	setHorizontalGridLineColor((VSCColour){0.8, 0.8, 0.8, 1.0});
	setHorizontalZeroGridLineColor((VSCColour){0.5, 0.5, 0.5, 1.0});
	
	setVerticalGridLineWidth(1.0);
	setHorizontalGridLineWidth(1.0);
	setHorizontalZeroGridLineWidth(1.0);
	
}

#pragma mark Encveloppe point calculations

int VSCEnveloppeViewSetup::pixelForTime(VSCSFloat t, int widthInPixels) {
	return 0;
}

int VSCEnveloppeViewSetup::pixelForValue(VSCSFloat t, int heightInPixels) {
	return 0;
}

VSCSFloat VSCEnveloppeViewSetup::timeForPixel(int pixel, int widthInPixels) {
	return 0.0;
}

VSCSFloat VSCEnveloppeViewSetup::valueForPixel(int pixel, int widthInPixels) {
	return 0.0;
}

#pragma mark Grid point calculations

int VSCEnveloppeViewSetup::getTimeGridPoints(std::list<VSCSFloat>& ptns) {
	
	int currentScalerPower = 0;
	VSCSFloat scaledTimeRange = getMaxTime() - getMinTime();
	
	if (scaledTimeRange < getTargetNumberOfVerticalGridLines()) {
		while (scaledTimeRange*5.0 < getTargetNumberOfVerticalGridLines()) {
			scaledTimeRange *= 10.0;
			currentScalerPower += 1;
		}
	}
	
	else if (scaledTimeRange > getTargetNumberOfVerticalGridLines()) {
		while (scaledTimeRange*0.5 > getTargetNumberOfVerticalGridLines()) {
			scaledTimeRange *= 0.1;
			currentScalerPower -= 1;
		}
	}
	
	VSCSFloat currentScaler = std::pow(10.0, (VSCSFloat)currentScalerPower);
	
	VSCSFloat scaledMinTime = getMinTime() * currentScaler;
	VSCSFloat scaledMaxTime = getMaxTime() * currentScaler;
	
	VSCSFloat roundMinTime = std::floor(scaledMinTime);
	VSCSFloat roundMaxTime = std::ceil(scaledMaxTime);
	
	for (VSCSFloat t = roundMinTime; t < roundMaxTime; t = t+1.0) {
		ptns.push_back(t/currentScaler);
	}
	
	return currentScalerPower;
	
}

int VSCEnveloppeViewSetup::getValueGridPoints(std::list<VSCSFloat>& ptns) {
	
	int currentScalerPower = 0;
	VSCSFloat scaledValueRange = getMaxValue() - getMinValue();
	
	if (scaledValueRange < getTargetNumberOfHorizontalGridLines()) {
		while (scaledValueRange*5.0 < getTargetNumberOfHorizontalGridLines()) {
			currentScalerPower += 1;
			scaledValueRange *= 10.0;
		}
	}
	
	else if (scaledValueRange*0.5 > getTargetNumberOfHorizontalGridLines()) {
		while (scaledValueRange > getTargetNumberOfHorizontalGridLines()) {
			currentScalerPower -= 1;
			scaledValueRange *= 0.1;
		}
	}
	
	VSCSFloat currentScaler = std::pow(10.0, (VSCSFloat)currentScalerPower);
	
	VSCSFloat scaledMinValue = getMinValue() * currentScaler;
	VSCSFloat scaledMaxValue = getMaxValue() * currentScaler;
	
	VSCSFloat roundMinValue = std::floor(scaledMinValue);
	VSCSFloat roundMaxValue = std::ceil(scaledMaxValue);
	
	for (VSCSFloat v = roundMinValue; v < roundMaxValue; v = v+1.0) {
		ptns.push_back(v/currentScaler);
	}
	
	return currentScalerPower;
	
}

int VSCEnveloppeViewSetup::getTimeGridPointsAndPixels(std::list< std::pair<VSCSFloat,int> >& pps, int pixelSize) {
	
	std::list<VSCSFloat> points;
	getTimeGridPoints(points);
	
	return 0;
	
}

int VSCEnveloppeViewSetup::getValueGridPointsAndPixels(std::list< std::pair<VSCSFloat,int> >& pps, int pixelSize) {
	
	std::list<VSCSFloat> points;
	getValueGridPoints(points);
	
	return 0;
}

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

/*
 *	Grid setters / getters
 */

bool VSCEnveloppeViewSetup::gridIsShowing(void) const {
	return _showGrid;
}

void VSCEnveloppeViewSetup::showGrid(bool showGrid) {
	_showGrid = showGrid;
}

float VSCEnveloppeViewSetup::getTargetVerticalGridSpacing(void) const {
	return _targetVerticalGridSpacing;
}

void VSCEnveloppeViewSetup::setTargetVerticalGridSpacing(float targetVerticalGridSpacing) {
	_targetVerticalGridSpacing = targetVerticalGridSpacing;
}

float VSCEnveloppeViewSetup::getTargetHorizontalGridSpacing(void) const {
	return _targetHorizontalGridSpacing;
}

void VSCEnveloppeViewSetup::setTargetHorizontalGridSpacing(float targetHorizontalGridSpacing) {
	_targetHorizontalGridSpacing = targetHorizontalGridSpacing;
}

float VSCEnveloppeViewSetup::getTargetNumberOfVerticalGridLines(void) const {
	return _targetNumberOfVerticalGridLines;
}

void VSCEnveloppeViewSetup::setTargetNumberOfVerticalGridLines(float targetNumberOfVerticalGridLines) {
	_targetNumberOfVerticalGridLines = targetNumberOfVerticalGridLines;
}

float VSCEnveloppeViewSetup::getTargetNumberOfHorizontalGridLines(void) const {
	return _targetNumberOfHorizontalGridLines;
}

void VSCEnveloppeViewSetup::setTargetNumberOfHorizontalGridLines(float targetNumberOfHorizontalGridLines) {
	_targetNumberOfHorizontalGridLines = targetNumberOfHorizontalGridLines;
}


float VSCEnveloppeViewSetup::getVerticalGridLineWidth(void) const {
	return _verticalGridLineWidth;
}

void VSCEnveloppeViewSetup::setVerticalGridLineWidth(float verticalGridLineWidth) {
	_verticalGridLineWidth = verticalGridLineWidth;
}

float VSCEnveloppeViewSetup::getHorizontalGridLineWidth(void) const {
	return _horizontalGridLineWidth;
}

void VSCEnveloppeViewSetup::setHorizontalGridLineWidth(float horizontalGridLineWidth) {
	_horizontalGridLineWidth = horizontalGridLineWidth;
}

float VSCEnveloppeViewSetup::getHorizontalZeroGridLineWidth(void) const {
	return _horizontalZeroGridLineWidth;
}

void VSCEnveloppeViewSetup::setHorizontalZeroGridLineWidth(float horizontalZeroGridLineWidth) {
	_horizontalZeroGridLineWidth = horizontalZeroGridLineWidth;
}


VSCColour VSCEnveloppeViewSetup::getHorizontalGridLineColor(void) const {
	return _horizontalGridLineColor;
}

void VSCEnveloppeViewSetup::setHorizontalGridLineColor(VSCColour horizontalGridLineColor) {
	_horizontalGridLineColor = horizontalGridLineColor;
}

VSCColour VSCEnveloppeViewSetup::getHorizontalZeroGridLineColor(void) const {
	return _horizontalZeroGridLineColor;
}

void VSCEnveloppeViewSetup::setHorizontalZeroGridLineColor(VSCColour horizontalZeroGridLineColor) {
	_horizontalZeroGridLineColor = horizontalZeroGridLineColor;
}

VSCColour VSCEnveloppeViewSetup::getVerticalGridLineColor(void) const {
	return _verticalGridLineColor;
}

void VSCEnveloppeViewSetup::setVerticalGridLineColor(VSCColour verticalGridLineColor) {
	_verticalGridLineColor = verticalGridLineColor;
}



