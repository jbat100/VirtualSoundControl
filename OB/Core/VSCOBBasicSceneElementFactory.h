
#ifndef _VSC_OGRE_BULLET_BASIC_SCENE_ELEMENT_FACTORY_H_
#define _VSC_OGRE_BULLET_BASIC_SCENE_ELEMENT_FACTORY_H_

#include "VSCOB.h"
#include "VSCOBScene.h"
#include "VSCOBDynamicObject.h"
#include "VSCOBStaticObject.h"

#include "OgreBulletDynamics.h"
#include "OgreBulletCollisions.h"

#include <Ogre/Ogre.h>

#include <boost/shared_ptr.hpp>

#include <map>

namespace VSC {
    
    namespace OB {

        class BasicSceneElementFactory : public Scene::ElementFactory {
            
        public:
            
            typedef boost::shared_ptr<BasicSceneElementFactory> SPtr;
            
            BasicSceneElementFactory(Scene::WPtr scene) : Scene::ElementFactory(scene) {}
            
            virtual void addGround(void);
            
            DynamicObject::SPtr addPrimitive(VSC::OB::PrimitiveType primitiveType,
                                             const DynamicObject::FactoryDescription& description);
            
            DynamicObject::SPtr addTrimesh(const Ogre::String& meshName,
                                           const DynamicObject::FactoryDescription& description);
             
            
            StaticObject::SPtr addStaticPlane(const StaticObject::FactoryDescription& description);
            
        private:
            
            static int mNumberOfObjectsCreated;
            
            
        };
        
    }
}

#endif //_VSC_OGRE_BULLET_BASIC_SCENE_ELEMENT_FACTORY_H_

