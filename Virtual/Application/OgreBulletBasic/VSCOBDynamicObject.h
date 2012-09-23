
#ifndef _VSC_OGRE_BULLET_DYNAMIC_OBJECT_H_
#define _VSC_OGRE_BULLET_DYNAMIC_OBJECT_H_

#include "VSCOBScene.h"

#include "OgreBulletDynamics.h"
#include "OgreBulletCollisions.h"

#include <Ogre/Ogre.h>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

namespace VSC {
    
    namespace OB {
        
        /*
         *  More of a container than a class...
         */
            
        class DynamicObject : public Scene::Element {
            
        public:
            
            typedef boost::shared_ptr<DynamicObject>    SPtr;
            typedef boost::weak_ptr<DynamicObject>      WPtr;
            
            /*
             *  FactoryDespription is designed to simplify the passing of arguments
             *  
             */
            
            struct FactoryDespription
            {
                FactoryDespription();
                
                Ogre::String        name;
                Ogre::Vector3       position;
                Ogre::Quaternion    orientation;
                Ogre::Vector3       size;
                Ogre::Real          bodyMass;
                Ogre::Real          bodyFriction;
                Ogre::Real          bodyRestitution;
                Ogre::String        materialName;
            }
            
            /**------------------------------------------------------------
             *  Constructor
             */
            
            Object(Scene::WPtr scene,
                   Ogre::Entity* entity,
                   OgreBulletDynamics::RigidBody* rigidBody,
                   OgreBulletCollisions::CollisionShape* collisionShape);
            
            /**------------------------------------------------------------
             *  Destroy
             */
            
            virtual void destroy(void);
            
            /**------------------------------------------------------------
             *  Getters
             */
            
            Ogre::Entity*                           getEntity(void);
            OgreBulletDynamics::RigidBody*          getRigidBody(void);
            OgreBulletCollisions::CollisionShape*   getCollisionShape(void);
            
        private:
            
            Ogre::Entity*                           mEntity;
            OgreBulletDynamics::RigidBody*          mRigidBody;
            OgreBulletCollisions::CollisionShape*   mCollisionShape;
        }
        
    }
}

#endif //_VSC_OGRE_BULLET_DYNAMIC_OBJECT_H_

