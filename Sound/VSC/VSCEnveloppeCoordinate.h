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

#include <boost/serialization/version.hpp>
#include <boost/serialization/split_member.hpp>

#define VSCEnveloppeCoordinatePtr    boost::shared_ptr<VSCEnveloppeCoordinate>

class VSCEnveloppeCoordinate {
	
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
	
private:
	
	/*
	 *	Print out and serialization (private)
	 */
	
	friend std::ostream& operator<<(std::ostream& output, const VSCEnveloppeCoordinate& c);
	
	friend class boost::serialization::access;
    template<class Archive>
    void save(Archive & ar, const unsigned int version) const
    {
        // note, version is always the latest when saving
        ar  & _value;
        ar  & _time;
    }
    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
        ar  & _value;
        ar  & _time;
    }
	
    BOOST_SERIALIZATION_SPLIT_MEMBER()
	
protected:
	
	double _value;
	double _time;
	
};

BOOST_CLASS_VERSION(VSCEnveloppeCoordinate, 1)

bool compareEnveloppeCoordinateValues (VSCEnveloppeCoordinate* firstPoint, VSCEnveloppeCoordinate* secondPoint);
bool compareEnveloppeCoordinateTimes (VSCEnveloppeCoordinate* firstPoint, VSCEnveloppeCoordinate* secondPoint);

#endif