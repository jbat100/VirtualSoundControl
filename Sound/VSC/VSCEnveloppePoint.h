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

#include "VSCEnveloppeCoordinate.h"

#define VSCEnveloppePointPtr    boost::shared_ptr<VSCEnveloppePoint>

#define EnvPntIter				std::list<VSCEnveloppePointPtr>::iterator 
#define RevEnvPntIter			std::list<VSCEnveloppePointPtr>::reverse_iterator 
#define ConstEnvPntIter         std::list<VSCEnveloppePointPtr>::const_iterator 
#define ConstRevEnvPntIter      std::list<VSCEnveloppePointPtr>::const_reverse_iterator 

class VSCEnveloppePoint : public VSCEnveloppeCoordinate {
	
	friend std::ostream& operator<<(std::ostream& output, const VSCEnveloppePoint& p);
	
protected:
    
    VSCEnveloppeCoordinatePtr _leftControlCoordinate;
    VSCEnveloppeCoordinatePtr _rightControlCoordinate;
	
public:
	
	/* Constructors / Destructors */
    VSCEnveloppePoint(void);
	VSCEnveloppePoint(const VSCEnveloppePoint& p);
	VSCEnveloppePoint(double value, double time);
	~VSCEnveloppePoint(void);
    
    void setLeftControlEnveloppeCoordinate(VSCEnveloppeCoordinatePtr controlPoint);
    VSCEnveloppeCoordinatePtr getLeftControlEnveloppeCoordinate(void) const;
    
    void setRightControlEnveloppeCoordinate(VSCEnveloppeCoordinatePtr controlPoint);
    VSCEnveloppeCoordinatePtr getRightControlEnveloppeCoordinate(void) const;
    

};

#endif