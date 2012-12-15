/*
 *  VSC::EnveloppeCoordinate.h
 *  EnveloppeEditor
 *
 *  Created by Jonathan Thorpe on 25/09/2011.
 *  Copyright 2011 JBAT. All rights reserved.
 *
 */

#ifndef _VSC_ENVELOPPE_COORDINATE_H_
#define _VSC_ENVELOPPE_COORDINATE_H_

#include "VSC.h"
#include "VSCSound.h"

#include <iostream>
#include <ostream>
#include <boost/shared_ptr.hpp>

#include <boost/serialization/version.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/shared_ptr.hpp>

namespace VSC {
   

    class EnveloppeCoordinate {
        
    public:
        
        typedef boost::shared_ptr<EnveloppeCoordinate> SPtr;
        
        /* Constructors / Destructors */
        EnveloppeCoordinate();
        EnveloppeCoordinate(const VSC::EnveloppeCoordinate& c);
        EnveloppeCoordinate(Float value, Float time);
        ~EnveloppeCoordinate(void);
        
        void setValue(Float value);
        Float getValue(void) const;
        
        void setTime(Float time);
        Float getTime(void) const;
        
    private:
        
        /*
         *	Print out and serialization (private)
         */
        
        friend std::ostream& operator<<(std::ostream& output, const EnveloppeCoordinate& c);
        
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
        
        Float _value;
        Float _time;
        
    };

    BOOST_CLASS_VERSION(EnveloppeCoordinate, 1)

    bool compareEnveloppeCoordinateValues (EnveloppeCoordinate* firstPoint, EnveloppeCoordinate* secondPoint);
    bool compareEnveloppeCoordinateTimes (EnveloppeCoordinate* firstPoint, EnveloppeCoordinate* secondPoint);

}

#endif