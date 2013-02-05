
#ifndef _VSC_IM_COLLISION_MAPPING_H_
#define _VSC_IM_COLLISION_MAPPING_H_

#include "VSC.h"
#include "VSCOB.h"
#include "VSCIMMapping.h"

#include <Ogre/Ogre.h>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

namespace VSC
{
    
    namespace IM
    {
    
        class CollisionMapping : public Mapping
        {
            
        public:
            
            typedef boost::shared_ptr<CollisionMapping>     SPtr;
            typedef boost::weak_ptr<CollisionMapping>       WPtr;
            
            CollisionMapping();
            virtual ~CollisionMapping() {}
            
            virtual Float mappedValue(OB::Collision_SPtr collision, OB::Element_SPtr effector) = 0;
            
        protected:
            
            OB::Element_SPtr getCollisionEffectee(OB::Collision_SPtr collision, OB::Element_SPtr effector);
            
        private:
            
            const static bool mTrace = true;
            
        };
        
        typedef std::vector<CollisionMapping::SPtr> CollisionMappings;
        
    }
    
}



#endif // _VSC_IM_COLLISION_MAPPING_H_
