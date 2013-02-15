
#ifndef _VSC_IM_MAPPING_IMPLEMENTATIONS_H_
#define _VSC_IM_MAPPING_IMPLEMENTATIONS_H_

#include "VSCIM.h"
#include "VSCIMMapping.h"

#include <Ogre/Ogre.h>

#include <boost/shared_ptr.hpp>

namespace VSC {
    
    namespace IM {
        
        /**
         *  Base owner classes so that I don't have to write accessors 10 million times
         */
        
        class ReferencePointOwner
        {
        public:
            ReferencePointOwner() : mReferencePoint (defaultReferencePoint) {}
            static Ogre::Vector3 getDefaultReferencePoint() {return defaultReferencePoint;}
            Ogre::Vector3 getReferencePoint() {return mReferencePoint;}
            void setReferencePoint(Ogre::Vector3 referencePoint) {mReferencePoint = referencePoint;}
        private:
            static Ogre::Vector3 defaultReferencePoint;
            Ogre::Vector3 mReferencePoint;
        };
        
        /**
         *  Concrete Implementations
         */
        
        class Mapping::ImplementationConstant : public Mapping::Implementation
        {
            virtual Float mappedValue(Trigger trigger, TriggerPayload::SPtr payload);
        };
        
        class Mapping::ImplementationCollisionVelocity : public Mapping::Implementation
        {
            virtual Float mappedValue(Trigger trigger, TriggerPayload::SPtr payload);
        };
        
        class Mapping::ImplementationCollisionDistance : public Mapping::Implementation,
        public ReferencePointOwner
        {
            virtual Float mappedValue(Trigger trigger, TriggerPayload::SPtr payload);
        };

    }
}

#endif // _VSC_IM_MAPPING_IMPLEMENTATIONS_H_
