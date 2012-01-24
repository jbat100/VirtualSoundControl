//
//  VSCStaticEnveloppePoint.h
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 1/24/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#ifndef VSC_STATIC_ENVELOPPE_POINT_H
#define VSC_STATIC_ENVELOPPE_POINT_H


#include <iostream>
#include <ostream>

#include <boost/shared_ptr.hpp>

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/serialization/nvp.hpp>

#include "VSCEnveloppeCoordinate.h"
#include "VSCSound.h"

class VSCStaticEnveloppePoint : public VSCEnveloppeCoordinate {
	
public:
	
	/* Constructors / Destructors */
    VSCStaticEnveloppePoint(void);
	VSCStaticEnveloppePoint(const VSCStaticEnveloppePoint& p);
	VSCStaticEnveloppePoint(VSCSFloat value, VSCSFloat time);
	VSCStaticEnveloppePoint(void);
    
    void setLeftControlEnveloppeCoordinate(VSCEnveloppeCoordinate controlPoint);
    VSCEnveloppeCoordinate getLeftControlEnveloppeCoordinate(void) const;
    
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
		using boost::serialization::make_nvp;
		ar  & make_nvp("point_coordinate", boost::serialization::base_object<VSCEnveloppeCoordinate>(*this));    
		ar  & make_nvp("left_control_coordinate", _leftControlCoordinate);
		ar  & make_nvp("right_control_coordinate", _rightControlCoordinate);
    }
    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
		using boost::serialization::make_nvp;
		ar  & make_nvp("point_coordinate", boost::serialization::base_object<VSCEnveloppeCoordinate>(*this));    
		ar  & make_nvp("left_control_coordinate", _leftControlCoordinate);
		ar  & make_nvp("right_control_coordinate", _rightControlCoordinate);
    }
	
    BOOST_SERIALIZATION_SPLIT_MEMBER()
	
protected:
    
    VSCEnveloppeCoordinate _leftControlCoordinate;
    VSCEnveloppeCoordinate _rightControlCoordinate;
    
    
};

BOOST_CLASS_VERSION(VSCEnveloppePoint, 1)


#endif
