
//
//  Created by Jonathan Thorpe on 4/21/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#ifndef _VSC_IM_COLLISION_MAPPING_H_
#define _VSC_IM_COLLISION_MAPPING_H_

#include "VSC.h"
#include "VSCOBScene.h"

#include <Ogre/Ogre.h>

#include <boost/shared_ptr.hpp>

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
            
            typedef boost::shared_ptr<CollisionMapping> SPtr;
            
            CollisionMapping() : mOffset(0.0), mScaleFactor(0.0) {}
            
            Float mappedValue(OB::Scene::Element::SPtr element, OB::Scene::Collision::SPtr collision);
            
            Float getOffset(void) {return mOffset;}
            void setOffset(Float offset) {mOffset = offset;}
            
            Float getScaleFactor(void) {return mScaleFactor;}
            void setScaleFactor(Float factor) {mScaleFactor = factor;}
            
        protected:
            
            /*
             *  Internal function allowing subclasses to specialise mapping,
             *  which should not apply offset and scale factor
             */
            
            virtual Float internalMappedValue(VSC::OB::Scene::Element::SPtr element, OB::Scene::Collision::SPtr collision) = 0;
            
        private:
            
            Float   mOffset;
            Float   mScaleFactor;
            
        };
        
        /*
         *  Constant mapping (which can be modulated with offset, then modulators and scale factor)
         */
        
        class CollisionConstantMapping : public CollisionMapping {
            
        protected:
            
            virtual Float internalMappedValue(VSC::OB::Scene::Element::SPtr element, OB::Scene::Collision::SPtr collision) {return 0.0;}
            
        };
        
        /*
         *  Velocity mapping (which can be modulated with offset, then modulators and scale factor)
         *  Relative velocity of the two objects at collision time.
         */
        
        class CollisionVelocityMapping : public CollisionMapping {
            
        protected:
            
            virtual Float internalMappedValue(VSC::OB::Scene::Element::SPtr element, OB::Scene::Collision::SPtr collision);
            
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
            
            virtual Float internalMappedValue(VSC::OB::Scene::Element::SPtr element, OB::Scene::Collision::SPtr collision);
            
        private:
            
            Ogre::Vector3   mReferencePoint;
            
        };
        
    }
    
}



#endif // _VSC_IM_COLLISION_MAPPING_H_
