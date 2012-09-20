
#ifndef _VSC_OGRE_BULLET_STATIC_GEOMETRY_H_
#define _VSC_OGRE_BULLET_STATIC_GEOMETRY_H_

#include "OgreBulletDynamics.h"
#include "OgreBulletCollisions.h"

#include <Ogre/Ogre.h>

#include <boost/shared_ptr.hpp>


namespace VSC {
    
    namespace OB {
        
        /*
         *  More of a container than a class...
         */
            
        class StaticGeometry {
            
        public:
            
            typedef boost::shared_ptr<StaticGeometry> SPtr;
            
            /*
             *  Constructor
             */
            
            Object(Ogre::StaticGeometry* staticGeometry,
                   OgreBulletDynamics::DynamicsWorld* world,
                   OgreBulletDynamics::RigidBody* rigidBody,
                   OgreBulletCollisions::CollisionShape* collisionShape);
            
            /*
             *  Getters
             */
            
            Ogre::StaticGeometry*                   getGeometry(void);
            OgreBulletDynamics::RigidBody*          getRigidBody(void);
            OgreBulletCollisions::CollisionShape*   getCollisionShape(void);
            OgreBulletDynamics::DynamicsWorld*      getWorld(void);
            
        private:
            
            Ogre::StaticGeometry*                   mStaticGeometry;
            OgreBulletDynamics::RigidBody*          mRigidBody;
            OgreBulletCollisions::CollisionShape*   mCollisionShape;
            OgreBulletDynamics::DynamicsWorld*      mWorld;
        }
        
    }
}

#endif //_VSC_OGRE_BULLET_STATIC_GEOMETRY_H_

