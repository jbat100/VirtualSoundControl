/*
 *  VSCEnveloppeViewSetup.h
 *  EnveloppeEditor
 *
 *  Created by Jonathan Thorpe on 02/08/2011.
 *  Copyright 2011 JBAT. All rights reserved.
 *
 */

#ifndef _VSC_ENVELOPPE_VIEW_SETUP_H_
#define _VSC_ENVELOPPE_VIEW_SETUP_H_

#include "VSCColour.h"
#include <boost/shared_ptr.hpp>

#define VSCEnveloppeViewSetupPtr    boost::shared_ptr<VSCEnveloppeViewSetup>

typedef enum _VSCEnveloppeDisplayType {
	VSCEnveloppeDisplayTypeDefault = 0,
	VSCEnveloppeDisplayTypeLinear,
	VSCEnveloppeDisplayTypeDB
} VSCEnveloppeDisplayType;


class VSCEnveloppeViewSetup {
	
protected:
	
	/*
	 *	View range
	 */
	float _minTime;
	float _maxTime;
	float _minValue;
	float _maxValue;
	
	/*
	 *	Display type
	 */
	VSCEnveloppeDisplayType _displayType;
	
	/*
	 *	Control Points
	 */
	float _controlPointRadius;
	VSCColour _controlPointSelectedColour;
	VSCColour _controlPointUnselectedColour;
	
	/*
	 *	Enveloppe lines
	 */
	float _lineWidth;
	VSCColour _lineColour;
	
	
public:
	
	/*
	 *	View range setters / getters
	 */
	float getMinTime(void) const;
	void setMinTime(float  minTime);
	float getMaxTime(void) const;
	void setMaxTime(float  maxTime);
	float getMinValue(void) const;
	void setMinValue(float  minValue);
	float getMaxValue(void) const;
	void setMaxValue(float  maxValue);
	
	/*
	 *	Display type setters / getters
	 */
	VSCEnveloppeDisplayType getDisplayType(void) const;
	void setDisplayType(VSCEnveloppeDisplayType displayType);
	
	/*
	 *	Control Points setters / getters
	 */
	float getControlPointRadius(void) const;
	void setControlPointRadius(float controlPointRadius);
	VSCColour getControlPointSelectedColour(void) const;
	void setControlPointSelectedColour(VSCColour colour);
	VSCColour getControlPointUnselectedColour(void) const;
	void setControlPointUnselectedColour(VSCColour colour);
	
	/*
	 *	Enveloppe lines setters / getters
	 */
	float getLineWidth(void) const;
	void setLineWidth(float lineWidth);
	VSCColour getLineColour(void) const;
	void setLineColour(VSCColour colour);

};

#endif