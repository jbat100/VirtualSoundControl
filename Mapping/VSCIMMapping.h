
#ifndef _VSC_IM_MAPPING_H_
#define _VSC_IM_MAPPING_H_

#include "VSC.h"
#include "VSCIM.h"

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
            
            virtual Float mappedValue(void);
            
            Float getOffset(void) {return mOffset;}
            void setOffset(Float offset) {mOffset = offset;}
            
            Float getScaleFactor(void) {return mScaleFactor;}
            void setScaleFactor(Float factor) {mScaleFactor = factor;}
            
            MappingType getMappingType(void) {return mMappingType;}
            bool setMappingType(MappingType mappingType);
            
            class Implementation
            {
            public:
                typedef boost::shared_ptr<Implementation>   SPtr;
                typedef boost::weak_ptr<Implementation>     WPtr;
                virtual Float mappedValue() = 0;
            };
            class ImplementationConstant;
            
            Implementation::SPtr getImplementation(void);
            
        protected:
            
            Float applyOffsetAndScaleFactor(Float rawValue);
            
            /**
              Called by subclasses in their constructor to define allowed mapping types
             */
            
            void allowMappingType(const MappingType mappingType);
            
            /**
             Called by setMappingType to allow sublasses to update potential additional setup 
             */
            
            virtual void updateSetups(void);
            
        private:
            
            bool checkMappingType(const MappingType mappingType);
            
            const static bool mTrace = true;
            
            Float   mOffset;
            Float   mScaleFactor;
            
            MappingType             mMappingType;
            
            MappingTypeSet          mAllowedMappingTypeSet;
            
            Implementation::SPtr    mImplementation;
            
        };
        
        typedef std::vector<Mapping::SPtr> Mappings;
        
    }
    
}



#endif // _VSC_IM_MAPPING_H_
