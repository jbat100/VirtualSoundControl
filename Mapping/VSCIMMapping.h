
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
            
            /**
             Mapping::Setup provides a way for additional information relative to particular
             mapping types to be provided without the need to subclass the mapping itself.
             */
            
            enum SetupType
            {
                SetupTypeNone = 0,
                SetupTypeReferencePoint
            };
            struct Setup;
            struct ReferencePointSetup;
            typedef boost::shared_ptr<Setup>            Setup_SPtr;
            typedef boost::weak_ptr<Setup>              Setup_WPtr;
            typedef std::map<SetupType, Setup_SPtr>     SetupMap;
            
            Setup_SPtr getSetupForSetupType(SetupType setupType) {return mSetupMap[setupType];}
            
            const SetupMap& getSetupMap(void) {return mSetupMap;}
            
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
            
            MappingType     mMappingType;
            
            MappingTypeSet  mAllowedMappingTypeSet;
            
            SetupMap        mSetupMap;
            
        };
        
        typedef std::vector<Mapping::SPtr> Mappings;
        
        /*
         *  Distance setup, for mappings which are dependant on a reference
         */
        
        struct Mapping::Setup
        {
            typedef boost::shared_ptr<Setup>      SPtr;
            typedef boost::weak_ptr<Setup>        WPtr;
        };
        
        /*
         *  Distance setup, for mappings which are dependant on a reference
         */
        
        struct Mapping::ReferencePointSetup : public Mapping::Setup
        {
            ReferencePointSetup();
            static Ogre::Vector3 defaultReferencePoint;
            Ogre::Vector3 referencePoint;
        };
        
    }
    
}



#endif // _VSC_IM_MAPPING_H_
