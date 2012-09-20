
#ifndef _VSC_OGRE_BULLET_OBJECT_H_
#define _VSC_OGRE_BULLET_OBJECT_H_

#include "OgreBulletDynamics.h"
#include "OgreBulletCollisions.h"

#include <Ogre/Ogre.h>

#include <boost/shared_ptr.hpp>


namespace VSC {
    
    namespace OB {
        
        /*
         *  More of a container than a class...
         */
            
        class Object {
            
        public:
            
            typedef boost::shared_ptr<Object> SPtr;
            
            enum PrimitiveType {
                PrimitiveNone = 0,
                PrimitiveCube,
                PrimitiveSphere,
                PrimitiveCylinder,
                PrimitiveCone
            };
            
            /*
             *  Constructor
             */
            
            Object(Ogre::Entity* entity,
                   OgreBulletDynamics::DynamicsWorld* world,
                   OgreBulletDynamics::RigidBody* rigidBody,
                   OgreBulletCollisions::CollisionShape* collisionShape,
                   PrimitiveType primitiveType = PrimitiveNone);
            
            /*
             *  Getters
             */
            
            Ogre::Entity*                           getEntity(void);
            OgreBulletDynamics::RigidBody*          getRigidBody(void);
            OgreBulletCollisions::CollisionShape*   getCollisionShape(void);
            OgreBulletDynamics::DynamicsWorld*      getWorld(void);
            PrimitiveType                           getPrimitiveType(void);
            
        private:
            
            Ogre::Entity*                           mEntity;
            OgreBulletDynamics::RigidBody*          mRigidBody;
            OgreBulletCollisions::CollisionShape*   mCollisionShape;
            OgreBulletDynamics::DynamicsWorld*      mWorld;
            
            PrimitiveType                           mPrimitiveType;
        }
        
    }
}

#endif //_VSC_OGRE_BULLET_OBJECT_H_

