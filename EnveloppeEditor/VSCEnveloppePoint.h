/*
 *  VSCEnveloppePoint.h
 *  EnveloppeEditor
 *
 *  Created by Jonathan Thorpe on 29/07/2011.
 *  Copyright 2011 JBAT. All rights reserved.
 *
 */


class VSCEnveloppePoint {
	
protected:
	
	double _value;
	double _time;
	
public:
	
	VSCEnveloppePoint(const VSCEnveloppePoint& p);
	
	void setValue(double value);
	double getValue(void);
	
	void setTime(double time);
	double getTime(void);

}

bool compareEnveloppePointValues (VSCEnveloppePoint* firstPoint, VSCEnveloppePoint* secondPoint);
bool compareEnveloppePointTimes (VSCEnveloppePoint* firstPoint, VSCEnveloppePoint* secondPoint);