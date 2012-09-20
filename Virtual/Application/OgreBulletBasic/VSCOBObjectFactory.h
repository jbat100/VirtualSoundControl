
#ifndef _VSC_OGRE_BULLET_OBJECT_FACTORY_H_
#define _VSC_OGRE_BULLET_OBJECT_FACTORY_H_

#include "VSCOBObject.h"

#include "OgreBulletDynamics.h"
#include "OgreBulletCollisions.h"

#include <Ogre/Ogre.h>

#include <boost/shared_ptr.hpp>

namespace VSC {
    
    namespace OB {

        class ObjectFactory {
            
        public:
            
            ObjectFactory(OgreBulletDynamics::DynamicsWorld* world);
            
            Object::SPtr addPrimitive(Object::PrimitiveType primitiveType,
                                      const Ogre::String instanceName,
                                      const Ogre::Vector3 &pos,
                                      const Ogre::Quaternion &q,
                                      const Ogre::Vector3 &size,
                                      const Ogre::Real bodyMass,
                                      const Ogre::Real bodyFriction,
                                      const Ogre::Real bodyRestitution);
            
            
            
            Object::SPtr addStaticTrimesh(const Ogre::String &instanceName,
                                          const Ogre::String &meshName,
                                          const Ogre::Vector3 &pos,
                                          const Ogre::Quaternion &q,
                                          const Ogre::Real bodyRestitution,
                                          const Ogre::Real bodyFriction,
                                          const bool castShadow = true);
            
            Object::SPtr addStaticPlane(const Ogre::Real bodyRestitution,
                                        const Ogre::Real bodyFriction);
            
            
            
        private:
            
            OgreBulletDynamics::DynamicsWorld*  mWorld;
            Ogre::String                        mDefaultMaterialName;
            
            static int                          mNumberOfObjectsCreated;
            
            
        }
        
    }
}

#endif //_VSC_OGRE_BULLET_OBJECT_FACTORY_H_

