/*
 *  VSC::EnvelopePoint.cpp
 *  EnvelopeEditor
 *
 *  Created by Jonathan Thorpe on 29/07/2011.
 *  Copyright 2011 JBAT. All rights reserved.
 *
 */

#include "VSCEnvelopePoint.h"
#include "VSCSound.h"

#include <cstddef>
#include <assert.h>


VSC::EnvelopePoint::EnvelopePoint(void) :
_curveType(CurveTypeLinear),
_curveInflexion(0.0)
{

}

VSC::EnvelopePoint::EnvelopePoint(Float value, Float time) :
EnvelopeCoordinate (value, time),
_curveType(CurveTypeLinear),
_curveInflexion(0.0)
{
    
}

VSC::EnvelopePoint::EnvelopePoint(const EnvelopePoint& p) : EnvelopeCoordinate(p)
{
    _curveType = p.getCurveType();
    _curveInflexion = p.getCurveInflexion();
}

VSC::EnvelopePoint::~EnvelopePoint(void)
{
	std::cout << "Destroying envelope point!" << std::endl;
}

#pragma mark --- Operator <<

std::ostream& VSC::operator<<(std::ostream& output, const EnvelopePoint& p)
{
    output << "EnvelopePoint (time: " <<  p._time << "s, value: " << p._value << ")";
    return output;  
}

bool VSC::compareEnvelopePointTimes(const VSC::EnvelopePoint::SPtr& point1, const VSC::EnvelopePoint::SPtr& point2)
{
    if (point1->getTime() < point2->getTime())
    {
        return true;
    }
    return false;
}
