/*
 *  VSC::EnveloppePoint.h
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
#include <boost/serialization/nvp.hpp>

#include "VSC::EnveloppeCoordinate.h"
#include "VSC.h"
#include "VSCSound.h"

namespace VSC {
    

    class EnveloppePoint : public EnveloppeCoordinate {
        
    public:
        
        typedef boost::shared_ptr<EnveloppePoint> SPtr;
        
        /* Constructors / Destructors */
        EnveloppePoint(void);
        EnveloppePoint(const EnveloppePoint& p);
        EnveloppePoint(const EnveloppeCoordinate& p) : EnveloppeCoordinate(p) {};
        EnveloppePoint(Float value, Float time);
        ~EnveloppePoint(void);
        
        void setLeftControlEnveloppeCoordinate(EnveloppeCoordinate::SPtr controlPoint);
        EnveloppeCoordinate::SPtr getLeftControlEnveloppeCoordinate(void) const;
        
        void setRightControlEnveloppeCoordinate(EnveloppeCoordinate::SPtr controlPoint);
        EnveloppeCoordinate::SPtr getRightControlEnveloppeCoordinate(void) const;
        
    private:
        
        EnveloppeCoordinate::SPtr _leftControlCoordinate;
        EnveloppeCoordinate::SPtr _rightControlCoordinate;
        
        /*
         *	Print out and serialization (private)
         */
        
        friend std::ostream& operator<<(std::ostream& output, const EnveloppePoint& p);
        
        friend class boost::serialization::access;
        template<class Archive>
        void save(Archive & ar, const unsigned int version) const
        {
            using boost::serialization::make_nvp;
            ar  & make_nvp("point_coordinate", boost::serialization::base_object<EnveloppeCoordinate>(*this));    
            ar  & make_nvp("left_control_coordinate", _leftControlCoordinate);
            ar  & make_nvp("right_control_coordinate", _rightControlCoordinate);
        }
        template<class Archive>
        void load(Archive & ar, const unsigned int version)
        {
            using boost::serialization::make_nvp;
            ar  & make_nvp("point_coordinate", boost::serialization::base_object<EnveloppeCoordinate>(*this));    
            ar  & make_nvp("left_control_coordinate", _leftControlCoordinate);
            ar  & make_nvp("right_control_coordinate", _rightControlCoordinate);
        }
        
        BOOST_SERIALIZATION_SPLIT_MEMBER()
        
    };

    BOOST_CLASS_VERSION(EnveloppePoint, 1)

    bool compareEnveloppePointTimes(const EnveloppePoint::SPtr& point1, const EnveloppePoint::SPtr& point2);
    
}

#endif