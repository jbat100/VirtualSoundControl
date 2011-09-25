/*
 *  VSCEnveloppeCoordinate.h
 *  EnveloppeEditor
 *
 *  Created by Jonathan Thorpe on 25/09/2011.
 *  Copyright 2011 JBAT. All rights reserved.
 *
 */

#ifndef _VSC_ENVELOPPE_COORDINATE_H_
#define _VSC_ENVELOPPE_COORDINATE_H_

#include <iostream>
#include <ostream>
#include <boost/shared_ptr.hpp>

#define VSCEnveloppeCoordinatePtr    boost::shared_ptr<VSCEnveloppeCoordinate>

class VSCEnveloppeCoordinate {
	
	friend std::ostream& operator<<(std::ostream& output, const VSCEnveloppeCoordinate& c);
	
protected:
	
	double _value;
	double _time;
	
public:
	
	/* Constructors / Destructors */
    VSCEnveloppeCoordinate(void);
	VSCEnveloppeCoordinate(const VSCEnveloppeCoordinate& c);
	VSCEnveloppeCoordinate(double value, double time);
	~VSCEnveloppeCoordinate(void);
	
	void setValue(double value);
	double getValue(void) const;
	
	void setTime(double time);
	double getTime(void) const;
	
};

bool compareEnveloppeCoordinateValues (VSCEnveloppeCoordinate* firstPoint, VSCEnveloppeCoordinate* secondPoint);
bool compareEnveloppeCoordinateTimes (VSCEnveloppeCoordinate* firstPoint, VSCEnveloppeCoordinate* secondPoint);

#endif