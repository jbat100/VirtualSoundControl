
#ifndef _VSC_OGRE_BULLET_BASE_SCENE_FACTORY_H_
#define _VSC_OGRE_BULLET_BASE_SCENE_FACTORY_H_

#include "VSCOBScene.h"
#include "VSCOBDynamicObject.h"
#include "VSCOBStaticGeometry.h"

#include "OgreBulletDynamics.h"
#include "OgreBulletCollisions.h"

#include <Ogre/Ogre.h>

#include <boost/shared_ptr.hpp>

#include <map>

namespace VSC {
    
    namespace OB {

        class BaseElementFactory : public Scene::ElementFactory {
            
        public:
            
            typedef boost::shared_ptr<BaseElementFactory> SPtr;
            
            BaseElementFactory(Scene::WPtr scene);
            
            DynamicObject::WPtr addPrimitive(VSC::OB::PrimitiveType primitiveType,
                                             const DynamicObject::FactoryDespription& description);
            
            
            
            DynamicObject::WPtr addTrimesh(const Ogre::String& meshName,
                                           const DynamicObject::FactoryDespription& description);
             
            
            StaticGeometry::WPtr addStaticPlane(const StaticGeometry::FactoryDespription& description);
            
            
            
        private:
            
            static int mNumberOfObjectsCreated;
            
            
        }
        
    }
}

#endif //_VSC_OGRE_BULLET_BASE_SCENE_FACTORY_H_

