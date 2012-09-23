
#ifndef _VSC_OGRE_BULLET_BASE_SCENE_FACTORY_H_
#define _VSC_OGRE_BULLET_BASE_SCENE_FACTORY_H_

#include "VSCOBScene.h"
#include "VSCOBDynamicObject.h"

#include "OgreBulletDynamics.h"
#include "OgreBulletCollisions.h"

#include <Ogre/Ogre.h>

#include <boost/shared_ptr.hpp>

namespace VSC {
    
    namespace OB {

        class BaseElementFactory : public Scene::ElementFactory {
            
        public:
            
            BaseElementFactory(Scene::WPtr scene);
            
            DynamicObject::SPtr addPrimitive(VSC::OB::PrimitiveType primitiveType,
                                             const DynamicObject::FactoryDespription& description);
            
            
            /*
            Object::SPtr addStaticTrimesh(const Ogre::String &instanceName,
                                          const Ogre::String &meshName,
                                          const Ogre::Vector3 &pos,
                                          const Ogre::Quaternion &q,
                                          const Ogre::Real bodyRestitution,
                                          const Ogre::Real bodyFriction,
                                          const bool castShadow = true);
             */
            
            Object::SPtr addStaticPlane(const Ogre::Real bodyRestitution,
                                        const Ogre::Real bodyFriction);
            
            
            
        private:
            
            static int                          mNumberOfObjectsCreated;
            
            
        }
        
    }
}

#endif //_VSC_OGRE_BULLET_BASE_SCENE_FACTORY_H_

