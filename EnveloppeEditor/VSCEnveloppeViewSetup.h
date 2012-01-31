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
#include "VSCSound.h"
#include <utility>
#include <list>
#include <boost/shared_ptr.hpp>

#define VSCEnveloppeViewSetupPtr    boost::shared_ptr<VSCEnveloppeViewSetup>

typedef enum _VSCEnveloppeDisplayType {
	VSCEnveloppeDisplayTypeDefault = 0,
	VSCEnveloppeDisplayTypeLinear,
	VSCEnveloppeDisplayTypeDB
} VSCEnveloppeDisplayType;


class VSCEnveloppeViewSetup {
	
public:
	
	VSCEnveloppeViewSetup(void);
	
	void setToDefault(void);
	
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
    
    /*
	 *	Enveloppe point calculations
	 */
	
	int pixelForTime(VSCSFloat t, int widthInPixels);
	int pixelForValue(VSCSFloat t, int heightInPixels);
	VSCSFloat timeForPixel(int pixel, int widthInPixels);
	VSCSFloat valueForPixel(int pixel, int widthInPixels);
	
	/*-------------------------------------------------------------------
     *  Grid stuff not implemented 
	 *	Grid setters / getters
	 */
	
	bool gridIsShowing(void) const;
	void showGrid(bool showGrid);
	
	float getTargetVerticalGridSpacing(void) const;
	void setTargetVerticalGridSpacing(float targetVerticalGridSpacing);
	float getTargetHorizontalGridSpacing(void) const;
	void setTargetHorizontalGridSpacing(float targetHorizontalGridSpacing);
	
	float getTargetNumberOfVerticalGridLines(void) const;
	void setTargetNumberOfVerticalGridLines(float targetNumberOfVerticalGridLines);
	float getTargetNumberOfHorizontalGridLines(void) const;
	void setTargetNumberOfHorizontalGridLines(float targetNumberOfHorizontalGridLines);
	
	float getVerticalGridLineWidth(void) const;
	void setVerticalGridLineWidth(float verticalGridLineWidth);
	float getHorizontalGridLineWidth(void) const;
	void setHorizontalGridLineWidth(float horizontalGridLineWidth);
	float getHorizontalZeroGridLineWidth(void) const;
	void setHorizontalZeroGridLineWidth(float horizontalZeroGridLineWidth);
	
	VSCColour getHorizontalGridLineColor(void) const;
	void setHorizontalGridLineColor(VSCColour horizontalGridLineColor);
	VSCColour getHorizontalZeroGridLineColor(void) const;
	void setHorizontalZeroGridLineColor(VSCColour horizontalZeroGridLineColor);
	VSCColour getVerticalGridLineColor(void) const;
	void setVerticalGridLineColor(VSCColour verticalGridLineColor);
	
	/*
	 *	Grid point calculations
	 */
	
	int getTimeGridPoints(std::list<VSCSFloat>& ptns);
	int getValueGridPoints(std::list<VSCSFloat>& ptns);
	int getTimeGridPointsAndPixels(std::list< std::pair<VSCSFloat,int> >& pps, int widthInPixels);
	int getValueGridPointsAndPixels(std::list< std::pair<VSCSFloat,int> >& pps, int heightInPixels);
	
	
private:
	
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
	
	/*
	 *	Grid
	 */
	bool _showGrid;
	unsigned int _targetNumberOfVerticalGridLines;
	unsigned int _targetNumberOfHorizontalGridLines;
	float _targetVerticalGridSpacing;
	float _targetHorizontalGridSpacing;
	float _verticalGridLineWidth;
	float _horizontalGridLineWidth;
	float _horizontalZeroGridLineWidth;
	VSCColour _horizontalGridLineColor;
	VSCColour _horizontalZeroGridLineColor;
	VSCColour _verticalGridLineColor;
	

};

#endif