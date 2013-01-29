
#ifndef _VSC_IM_COLLISION_MAPPING_H_
#define _VSC_IM_COLLISION_MAPPING_H_

#include "VSC.h"
#include "VSCOB.h"
#include "VSCIMTarget.h"

#include <Ogre/Ogre.h>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

namespace VSC {
    
    namespace IM {
    
        /*
         *  GENERAL NOTE ON MAPPINGS
         *
         *  Mappings should have range [-1.0 1.0], which will then be translated appropriately
         *  for different protocols (MIDI, OSC, Internal Audio Synth Parameters ...)
         */
        
        class CollisionMapping {
            
        public:
            
            typedef boost::shared_ptr<CollisionMapping>     SPtr;
            typedef boost::weak_ptr<CollisionMapping>       WPtr;
            
            CollisionMapping() : mOffset(0.0), mScaleFactor(1.0) {}
            virtual ~CollisionMapping() {}
            
            Float mappedValue(OB::Element_SPtr element, OB::Collision_SPtr collision);
            
            Float getOffset(void) {return mOffset;}
            void setOffset(Float offset) {mOffset = offset;}
            
            Float getScaleFactor(void) {return mScaleFactor;}
            void setScaleFactor(Float factor) {mScaleFactor = factor;}
            
        protected:
            
            /*
             *  Internal function allowing subclasses to specialise mapping,
             *  which should not apply offset and scale factor
             */
            
            virtual Float internalMappedValue(OB::Element_SPtr element, OB::Collision_SPtr collision) = 0;
            
        private:
            
            const static bool mTrace = true;
            
            Float   mOffset;
            Float   mScaleFactor;
            
        };
        
        typedef std::vector<CollisionMapping::SPtr> CollisionMappings;
        
        /*
         *  Constant mapping (which can be modulated with offset, then modulators and scale factor)
         */
        
        class CollisionConstantMapping : public CollisionMapping {
            
        protected:
            
            virtual Float internalMappedValue(OB::Element_SPtr element, OB::Collision_SPtr collision) {return 0.0;}
            
        };
        
        /*
         *  Velocity mapping (which can be modulated with offset, then modulators and scale factor)
         *  Relative velocity of the two objects at collision time.
         */
        
        class CollisionVelocityMapping : public CollisionMapping {
            
        protected:
            
            virtual Float internalMappedValue(OB::Element_SPtr element, OB::Collision_SPtr collision);
            
        };
        
        /*
         *  Distance mapping (which can be modulated with offset, then modulators and scale factor)
         *  Distance of the collision point from another specified point.
         */
        
        class CollisionDistanceMapping : public CollisionMapping {
            
        public:
            
            const Ogre::Vector3& getReferencePoint(void) {return mReferencePoint;}
            void setReferencePoint(const Ogre::Vector3 p) {mReferencePoint = p;}
            
        protected:
            
            virtual Float internalMappedValue(OB::Element_SPtr element, OB::Collision_SPtr collision);
            
        private:
            
            Ogre::Vector3   mReferencePoint;
            
        };
        
    }
    
}



#endif // _VSC_IM_COLLISION_MAPPING_H_
