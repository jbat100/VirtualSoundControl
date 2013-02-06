
#ifndef _VSC_IM_MAPPING_H_
#define _VSC_IM_MAPPING_H_

#include "VSC.h"
#include "VSCIM.h"
#include "VSCOB.h"

#include <Ogre/Ogre.h>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include <map>

namespace VSC {
    
    namespace IM {
        
        class Mapping {
            
        public:
            
            typedef boost::shared_ptr<Mapping>      SPtr;
            typedef boost::weak_ptr<Mapping>        WPtr;
            
            Mapping();
            virtual ~Mapping() {}
            
            /**
             *  Plain Value Mapping
             */
            
            Float mappedValue(void);
            
            /**
             *  Collision Value Mapping
             */
            
            Float mappedValue(OB::Collision_SPtr collision, OB::Element_SPtr effector);
            
            MappingType getMappingType(void) {return mMappingType;}
            bool setMappingType(MappingType mappingType);
            
            /**
             *  Scale and Offset
             */
            
            Float getOffset(void) {return mOffset;}
            void setOffset(Float offset) {mOffset = offset;}
            
            Float getScaleFactor(void) {return mScaleFactor;}
            void setScaleFactor(Float factor) {mScaleFactor = factor;}
            
            /**
             *  Implementation
             */
            
            class Implementation
            {
            public:
                typedef boost::shared_ptr<Implementation>   SPtr;
                typedef boost::weak_ptr<Implementation>     WPtr;
                virtual Float mappedValue() {return 0.0;}
                virtual Float mappedValue(OB::Collision_SPtr collision, OB::Element_SPtr effector) {return 0.0;}
            };
            class ImplementationConstant;
            class ImplementationCollisionVelocity;
            class ImplementationCollisionDistance;
            
            Implementation::SPtr getImplementation(void);
            
        protected:
            
            Float applyOffsetAndScaleFactor(Float rawValue);
            
        private:
            
            const static bool mTrace = true;
            
            Float   mOffset;
            Float   mScaleFactor;
            
            MappingType             mMappingType;
            
            Implementation::SPtr    mImplementation;
            
        };
        
        typedef std::vector<Mapping::SPtr> Mappings;
        
    }
    
}



#endif // _VSC_IM_MAPPING_H_
