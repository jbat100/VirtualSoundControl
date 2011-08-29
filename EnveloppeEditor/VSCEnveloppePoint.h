/*
 *  VSCEnveloppePoint.h
 *  EnveloppeEditor
 *
 *  Created by Jonathan Thorpe on 29/07/2011.
 *  Copyright 2011 JBAT. All rights reserved.
 *
 */

#ifndef _VSC_ENVELOPPE_POINT_H_
#define _VSC_ENVELOPPE_POINT_H_

#include <iostream>
#include <ostream>
#include <boost/shared_ptr.hpp>

#define VSCEnveloppePointPtr    boost::shared_ptr<VSCEnveloppePoint>

#define EnvPntIter				std::list<VSCEnveloppePointPtr>::iterator 
#define RevEnvPntIter			std::list<VSCEnveloppePointPtr>::reverse_iterator 
#define ConstEnvPntIter         std::list<VSCEnveloppePointPtr>::const_iterator 
#define ConstRevEnvPntIter      std::list<VSCEnveloppePointPtr>::const_reverse_iterator 

class VSCEnveloppePoint {
	
	friend std::ostream& operator<<(std::ostream& output, const VSCEnveloppePoint& p);
	
protected:
    
    VSCEnveloppePoint* _leftControlPoint;
    VSCEnveloppePoint* _rightControlPoint;
	
	double _value;
	double _time;
	
public:
	
	/* Constructors / Destructors */
    VSCEnveloppePoint(void);
	VSCEnveloppePoint(const VSCEnveloppePoint& p);
	VSCEnveloppePoint(double value, double time);
	~VSCEnveloppePoint(void);
	
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

#endif