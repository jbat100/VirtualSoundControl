/*
 *  VSCEnveloppeViewSetup.cpp
 *  EnveloppeEditor
 *
 *  Created by Jonathan Thorpe on 02/08/2011.
 *  Copyright 2011 JBAT. All rights reserved.
 *
 */

#include "VSCEnveloppeViewSetup.h"
#include <cmath>

#pragma mark View range setters / getters

VSCEnveloppeViewSetup::VSCEnveloppeViewSetup(void) {
	
	setToDefault();
	
}

void VSCEnveloppeViewSetup::setToDefault(void) {
	
	setMinTime(0.0);
	setMaxTime(5.0);
	setMinValue(-1.0);
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

int VSCEnveloppeViewSetup::pixelForTime(double t, int widthInPixels) {
	
}

int VSCEnveloppeViewSetup::pixelForValue(double t, int heightInPixels) {
	
}

double VSCEnveloppeViewSetup::timeForPixel(int pixel, int widthInPixels) {
	
}

double VSCEnveloppeViewSetup::valueForPixel(int pixel, int widthInPixels) {
	
}

#pragma mark Grid point calculations

int VSCEnveloppeViewSetup::getTimeGridPoints(std::list<double>& ptns) {
	
	int currentScalerPower = 0;
	double scaledTimeRange = getMaxTime() - getMinTime();
	
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
	
	double currentScaler = std::pow(10.0, (double)currentScalerPower);
	
	double scaledMinTime = getMinTime() * currentScaler;
	double scaledMaxTime = getMaxTime() * currentScaler;
	
	double roundMinTime = std::floor(scaledMinTime);
	double roundMaxTime = std::ceil(scaledMaxTime);
	
	for (double t = roundMinTime; t < roundMaxTime; t = t+1.0) {
		ptns.push_back(t/currentScaler);
	}
	
	return currentScalerPower;
	
}

int VSCEnveloppeViewSetup::getValueGridPoints(std::list<double>& ptns) {
	
	int currentScalerPower = 0;
	double scaledValueRange = getMaxValue() - getMinValue();
	
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
	
	double currentScaler = std::pow(10.0, (double)currentScalerPower);
	
	double scaledMinValue = getMinValue() * currentScaler;
	double scaledMaxValue = getMaxValue() * currentScaler;
	
	double roundMinValue = std::floor(scaledMinValue);
	double roundMaxValue = std::ceil(scaledMaxValue);
	
	for (double v = roundMinValue; v < roundMaxValue; v = v+1.0) {
		ptns.push_back(v/currentScaler);
	}
	
	return currentScalerPower;
	
}

int VSCEnveloppeViewSetup::getTimeGridPointsAndPixels(std::list<std::pair<double,int>>& pps, int pixelSize) {
	
	std::list<double> points;
	getTimeGridPoints(points);
	
}

int VSCEnveloppeViewSetup::getValueGridPointsAndPixels(std::list<std::pair<double,int>>& pps, int pixelSize) {
	
	std::list<double> points;
	getValueGridPoints(points);
	
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
	_targetNumberOfVerticalGridLines = targetNumberOfVerticalGridLines
}

float VSCEnveloppeViewSetup::getTargetNumberOfHorizontalGridLines(void) const {
	return _targetNumberOfHorizontalGridLines
}

void VSCEnveloppeViewSetup::setTargetNumberOfHorizontalGridLines(float targetNumberOfHorizontalGridLines) {
	_targetNumberOfHorizontalGridLines = targetNumberOfHorizontalGridLines
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



