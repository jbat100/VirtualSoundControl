//
//  VSCEnveloppeDisplaySetup.cpp
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 2/6/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#include "VSCEnveloppeDisplaySetup.h"

VSCEnveloppeDisplaySetup::VSCEnveloppeDisplaySetup() {
    this->setControlPointRadius(3.0);
    this->setLineWidth(1.0);
    this->setControlPointSelectedColour((VSCColour){0.0, 0.0, 1.0, 1.0});
    this->setControlPointUnselectedColour((VSCColour){0.0, 0.0, 0.0, 1.0});
    this->setLineColour((VSCColour){0.0, 0.0, 0.0, 1.0});
}

VSCEnveloppeDisplaySetup::VSCEnveloppeDisplaySetup(const VSCEnveloppeDisplaySetup& setup) {
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

float VSCEnveloppeDisplaySetup::getControlPointRadius(void) const {
	return _controlPointRadius;
}

void VSCEnveloppeDisplaySetup::setControlPointRadius(float controlPointRadius) {
	_controlPointRadius = controlPointRadius;
}

VSCColour VSCEnveloppeDisplaySetup::getControlPointSelectedColour(void) const {
	return _controlPointSelectedColour;
}

void VSCEnveloppeDisplaySetup::setControlPointSelectedColour(VSCColour colour) {
	_controlPointSelectedColour = colour;
}

VSCColour VSCEnveloppeDisplaySetup::getControlPointUnselectedColour(void) const {
	return _controlPointUnselectedColour;
}

void VSCEnveloppeDisplaySetup::setControlPointUnselectedColour(VSCColour colour) {
	_controlPointUnselectedColour = colour;
}

float VSCEnveloppeDisplaySetup::getLineWidth(void) const {
	return _lineWidth;
}

void VSCEnveloppeDisplaySetup::setLineWidth(float lineWidth) {
	_lineWidth = lineWidth;
}

#pragma mark Enveloppe lines setters / getters

/*
 *	Enveloppe lines setters / getters
 */

VSCColour VSCEnveloppeDisplaySetup::getLineColour(void) const {
	return _lineColour;
}

void VSCEnveloppeDisplaySetup::setLineColour(VSCColour colour) {
	_lineColour = colour;
}
