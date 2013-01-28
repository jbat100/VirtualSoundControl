
#ifndef _VSC_IM_MAPPING_H_
#define _VSC_IM_MAPPING_H_

#include "VSC.h"
#include "VSCIMTarget.h"

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

namespace VSC {
    
    namespace IM {
        
        class Mapping {
            
        public:
            
            typedef boost::shared_ptr<Mapping>      SPtr;
            typedef boost::weak_ptr<Mapping>        WPtr;
            
            Mapping() : mOffset(0.0), mScaleFactor(1.0) {}
            virtual ~Mapping() {}
            
            virtual Float mappedValue(void);
            
            Float getOffset(void) {return mOffset;}
            void setOffset(Float offset) {mOffset = offset;}
            
            Float getScaleFactor(void) {return mScaleFactor;}
            void setScaleFactor(Float factor) {mScaleFactor = factor;}
            
        protected:
            
            Float applyOffsetAndScaleFactor(Float rawValue);
            
        private:
            
            const static bool mTrace = true;
            
            Float   mOffset;
            Float   mScaleFactor;
            
        };
        
        typedef std::vector<Mapping::SPtr> Mappings;
        
    }
    
}



#endif // _VSC_IM_MAPPING_H_
