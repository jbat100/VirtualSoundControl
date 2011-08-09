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
    
    VSCEnveloppePoint* _leftControlPoint;
    VSCEnveloppePoint* _rightControlPoint;
	
	double _value;
	double _time;
	
public:
	
    VSCEnveloppePoint(void);
	VSCEnveloppePoint(const VSCEnveloppePoint& p);
	
	void setValue(double value);
	double getValue(void) const;
	
	void setTime(double time);
	double getTime(void) const;
    
    void setLeftControlPoint(VSCEnveloppePoint* controlPoint);
    VSCEnveloppePoint* getLeftControlPoint(void) const;
    
    void setRightControlPoint(VSCEnveloppePoint* controlPoint);
    VSCEnveloppePoint* getRightControlPoint(void) const;
    

};

bool compareEnveloppePointValues (VSCEnveloppePoint* firstPoint, VSCEnveloppePoint* secondPoint);
bool compareEnveloppePointTimes (VSCEnveloppePoint* firstPoint, VSCEnveloppePoint* secondPoint);