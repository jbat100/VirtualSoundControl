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

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/split_member.hpp>

#include "VSCEnveloppeCoordinate.h"

#define VSCEnveloppePointPtr    boost::shared_ptr<VSCEnveloppePoint>

#define EnvPntIter				std::list<VSCEnveloppePointPtr>::iterator 
#define RevEnvPntIter			std::list<VSCEnveloppePointPtr>::reverse_iterator 
#define ConstEnvPntIter         std::list<VSCEnveloppePointPtr>::const_iterator 
#define ConstRevEnvPntIter      std::list<VSCEnveloppePointPtr>::const_reverse_iterator 

class VSCEnveloppePoint : public VSCEnveloppeCoordinate {
	
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
	
private:
	
	/*
	 *	Print out and serialization (private)
	 */
	
	friend std::ostream& operator<<(std::ostream& output, const VSCEnveloppePoint& p);
	
	friend class boost::serialization::access;
    template<class Archive>
    void save(Archive & ar, const unsigned int version) const
    {
        ar  & boost::serialization::base_object<VSCEnveloppeCoordinate>(*this);
        ar  & _leftControlCoordinate;
        ar  & _rightControlCoordinate;
    }
    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
		ar  & boost::serialization::base_object<VSCEnveloppeCoordinate>(*this);
        ar  & _leftControlCoordinate;
        ar  & _rightControlCoordinate;
    }
	
    BOOST_SERIALIZATION_SPLIT_MEMBER()
	
protected:
    
    VSCEnveloppeCoordinatePtr _leftControlCoordinate;
    VSCEnveloppeCoordinatePtr _rightControlCoordinate;
    

};

BOOST_CLASS_VERSION(VSCEnveloppePoint, 1)

#endif