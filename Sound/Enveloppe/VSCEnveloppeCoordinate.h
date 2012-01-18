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

#include "VSCSound.h"

#include <iostream>
#include <ostream>
#include <boost/shared_ptr.hpp>

#include <boost/serialization/version.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/shared_ptr.hpp>

#define VSCEnveloppeCoordinatePtr    boost::shared_ptr<VSCEnveloppeCoordinate>

class VSCEnveloppeCoordinate {
	
public:
	
	/* Constructors / Destructors */
    VSCEnveloppeCoordinate();
	VSCEnveloppeCoordinate(const VSCEnveloppeCoordinate& c);
	VSCEnveloppeCoordinate(VSCSFloat value, VSCSFloat time);
	~VSCEnveloppeCoordinate(void);
	
	void setValue(VSCSFloat value);
	VSCSFloat getValue(void) const;
	
	void setTime(VSCSFloat time);
	VSCSFloat getTime(void) const;
	
private:
	
	/*
	 *	Print out and serialization (private)
	 */
	
	friend std::ostream& operator<<(std::ostream& output, const VSCEnveloppeCoordinate& c);
	
	friend class boost::serialization::access;
	
    template<class Archive>
    void save(Archive & ar, const unsigned int version) const
    {
		using boost::serialization::make_nvp;
        // note, version is always the latest when saving
		ar  & make_nvp("value", _value);
        ar  & make_nvp("time", _time);
    }
    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
		using boost::serialization::make_nvp;
		ar  & make_nvp("value", _value);
        ar  & make_nvp("time", _time);
    }
	
    BOOST_SERIALIZATION_SPLIT_MEMBER()
	
protected:
	
	VSCSFloat _value;
	VSCSFloat _time;
	
};

BOOST_CLASS_VERSION(VSCEnveloppeCoordinate, 1)
//BOOST_SERIALIZATION_SHARED_PTR(VSCEnveloppeCoordinate)

bool compareEnveloppeCoordinateValues (VSCEnveloppeCoordinate* firstPoint, VSCEnveloppeCoordinate* secondPoint);
bool compareEnveloppeCoordinateTimes (VSCEnveloppeCoordinate* firstPoint, VSCEnveloppeCoordinate* secondPoint);

#endif