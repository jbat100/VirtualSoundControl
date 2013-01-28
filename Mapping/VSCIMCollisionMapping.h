
#ifndef _VSC_IM_COLLISION_MAPPING_H_
#define _VSC_IM_COLLISION_MAPPING_H_

#include "VSC.h"
#include "VSCOB.h"
#include "VSCIMMapping.h"
#include "VSCIMTarget.h"

#include <Ogre/Ogre.h>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

namespace VSC {
    
    namespace IM {
    
        class CollisionMapping : public Mapping {
            
        public:
            
            typedef boost::shared_ptr<CollisionMapping>     SPtr;
            typedef boost::weak_ptr<CollisionMapping>       WPtr;
            
            CollisionMapping() {}
            virtual ~CollisionMapping() {}
            
            virtual Float mappedValue(OB::Collision_SPtr collision, OB::Element_SPtr effector) = 0;
            
        protected:
            
            OB::Element_SPtr getCollisionEffectee(OB::Collision_SPtr collision, OB::Element_SPtr effector);
            
        private:
            
            const static bool mTrace = true;
            
        };
        
        typedef std::vector<CollisionMapping::SPtr> CollisionMappings;
        
        /*
         *  Constant mapping (which can be modulated with offset, then modulators and scale factor)
         */
        
        class CollisionConstantMapping : public CollisionMapping
        {
            
        public:
            
            virtual Float mappedValue(OB::Collision_SPtr collision, OB::Element_SPtr effector) {return 0.0;}
            
        };
        
        /*
         *  Velocity mapping (which can be modulated with offset, then modulators and scale factor)
         *  Relative velocity of the two objects at collision time.
         */
        
        class CollisionVelocityMapping : public CollisionMapping
        {
            
        public:
            
            virtual Float mappedValue(OB::Collision_SPtr collision, OB::Element_SPtr effector);
            
        };
        
        /*
         *  Distance mapping (which can be modulated with offset, then modulators and scale factor)
         *  Distance of the collision point from another specified point.
         */
        
        class CollisionDistanceMapping : public CollisionMapping {
            
        public:
            
            const Ogre::Vector3& getReferencePoint(void) {return mReferencePoint;}
            void setReferencePoint(const Ogre::Vector3 p) {mReferencePoint = p;}
            
            virtual Float mappedValue(OB::Collision_SPtr collision, OB::Element_SPtr effector);
            
        private:
            
            Ogre::Vector3   mReferencePoint;
            
        };
        
    }
    
}



#endif // _VSC_IM_COLLISION_MAPPING_H_
