/*
 *  VSCEnveloppeViewSetup.cpp
 *  EnveloppeEditor
 *
 *  Created by Jonathan Thorpe on 02/08/2011.
 *  Copyright 2011 JBAT. All rights reserved.
 *
 */

#include "VSCEnveloppeViewSetup.h"

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
	
}

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



