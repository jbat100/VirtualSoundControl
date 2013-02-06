
#ifndef _VSC_IM_MAPPING_IMPLEMENTATIONS_H_
#define _VSC_IM_MAPPING_IMPLEMENTATIONS_H_

#include "VSCOB.h"
#include "VSCIMMapping.h"

#include <Ogre/Ogre.h>

#include <boost/shared_ptr.hpp>

namespace VSC {
    
    namespace IM {
        
        class ReferencePointOwner
        {
        public:
            
            ReferencePointOwner();
            
            static Ogre::Vector3 defaultReferencePoint();
            
            Ogre::Vector3 getReferencePoint();
            void setReferencePoint(Ogre::Vector3 referencePoint);
            
        private:
            
            static Ogre::Vector3 defaultReferencePoint;
            Ogre::Vector3 mReferencePoint;
        };

    }
}

#endif // _VSC_IM_MAPPING_IMPLEMENTATIONS_H_
