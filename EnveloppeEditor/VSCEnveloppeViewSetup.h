/*
 *  VSCEnveloppeViewSetup.h
 *  EnveloppeEditor
 *
 *  Created by Jonathan Thorpe on 02/08/2011.
 *  Copyright 2011 JBAT. All rights reserved.
 *
 */

typedef enum _VSCEnveloppeDisplayType {
	EnveloppeDisplayTypeDefault = 0,
	EnveloppeDisplayTypeLinear,
	EnveloppeDisplayTypeDB
} VSCEnveloppeDisplayType;


class VSCEnveloppeViewSetup {
	
protected:
	
	double _minTime;
	double _maxTime;
	
	double _minValue;
	double _maxValue;
	
	VSCEnveloppeDisplayType _displayType;
	
	double _controlPointRadius;
	double _lineWidth;
	
	
public:
	
	double getMinTime(void);
	void setMinTime(double  minTime);
	
	double getMaxTime(void);
	void setMaxTime(double  maxTime);
	
	double getMinValue(void);
	void setMinValue(double  minValue);
	
	double getMaxValue(void);
	void setMaxValue(double  maxValue);
	
	VSCEnveloppeDisplayType getDisplayType(void);
	void setDisplayType(VSCEnveloppeDisplayType displayType);
	
	double getControlPointRadius(void);
	void setControlPointRadius(double controlPointRadius);
	
	double getLineWidth(void);
	void setLineWidth(double lineWidth);

}