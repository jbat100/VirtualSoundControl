/*
 *  VSC::EnvelopeCoordinate.h
 *  EnvelopeEditor
 *
 *  Created by Jonathan Thorpe on 25/09/2011.
 *  Copyright 2011 JBAT. All rights reserved.
 *
 */

#ifndef _VSC_ENVELOPE_COORDINATE_H_
#define _VSC_ENVELOPE_COORDINATE_H_

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
   

    class EnvelopeCoordinate {
        
    public:
        
        typedef boost::shared_ptr<EnvelopeCoordinate> SPtr;
        
        /* Constructors / Destructors */
        EnvelopeCoordinate();
        EnvelopeCoordinate(const VSC::EnvelopeCoordinate& c);
        EnvelopeCoordinate(Float value, Float time);
        ~EnvelopeCoordinate(void);
        
        void setValue(Float value);
        Float getValue(void) const;
        
        void setTime(Float time);
        Float getTime(void) const;
        
    private:
        
        /*
         *	Print out and serialization (private)
         */
        
        friend std::ostream& operator<<(std::ostream& output, const EnvelopeCoordinate& c);
        
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

    BOOST_CLASS_VERSION(EnvelopeCoordinate, 1)

    bool compareEnvelopeCoordinateValues (EnvelopeCoordinate* firstPoint, EnvelopeCoordinate* secondPoint);
    bool compareEnvelopeCoordinateTimes (EnvelopeCoordinate* firstPoint, EnvelopeCoordinate* secondPoint);

}

#endif