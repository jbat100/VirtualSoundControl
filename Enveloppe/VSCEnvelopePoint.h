
#ifndef _VSC_ENVELOPE_POINT_H_
#define _VSC_ENVELOPE_POINT_H_

#include <iostream>
#include <ostream>

#include <boost/shared_ptr.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/serialization/nvp.hpp>

#include "VSCEnvelopeCoordinate.h"
#include "VSC.h"
#include "VSCSound.h"

namespace VSC {
    

    class EnvelopePoint : public EnvelopeCoordinate {
        
    public:
        
        typedef boost::shared_ptr<EnvelopePoint> SPtr;
        
        /* Constructors / Destructors */
        EnvelopePoint(void);
        EnvelopePoint(const EnvelopePoint& p);
        EnvelopePoint(const EnvelopeCoordinate& p) : EnvelopeCoordinate(p) {};
        EnvelopePoint(Float value, Float time);
        ~EnvelopePoint(void);
        
        void setLeftControlEnvelopeCoordinate(EnvelopeCoordinate::SPtr controlPoint);
        EnvelopeCoordinate::SPtr getLeftControlEnvelopeCoordinate(void) const;
        
        void setRightControlEnvelopeCoordinate(EnvelopeCoordinate::SPtr controlPoint);
        EnvelopeCoordinate::SPtr getRightControlEnvelopeCoordinate(void) const;
        
    private:
        
        EnvelopeCoordinate::SPtr _leftControlCoordinate;
        EnvelopeCoordinate::SPtr _rightControlCoordinate;
        
        /*
         *	Print out and serialization (private)
         */
        
        friend std::ostream& operator<<(std::ostream& output, const EnvelopePoint& p);
        
        friend class boost::serialization::access;
        template<class Archive>
        void save(Archive & ar, const unsigned int version) const
        {
            using boost::serialization::make_nvp;
            ar  & make_nvp("point_coordinate", boost::serialization::base_object<EnvelopeCoordinate>(*this));    
            ar  & make_nvp("left_control_coordinate", _leftControlCoordinate);
            ar  & make_nvp("right_control_coordinate", _rightControlCoordinate);
        }
        template<class Archive>
        void load(Archive & ar, const unsigned int version)
        {
            using boost::serialization::make_nvp;
            ar  & make_nvp("point_coordinate", boost::serialization::base_object<EnvelopeCoordinate>(*this));    
            ar  & make_nvp("left_control_coordinate", _leftControlCoordinate);
            ar  & make_nvp("right_control_coordinate", _rightControlCoordinate);
        }
        
        BOOST_SERIALIZATION_SPLIT_MEMBER()
        
    };

    BOOST_CLASS_VERSION(EnvelopePoint, 1)

    bool compareEnvelopePointTimes(const EnvelopePoint::SPtr& point1, const EnvelopePoint::SPtr& point2);
    
}

#endif