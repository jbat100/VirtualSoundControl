
#ifndef _VSC_OGRE_BULLET_STATIC_GEOMETRY_H_
#define _VSC_OGRE_BULLET_STATIC_GEOMETRY_H_

#include "VSCOBScene.h"

#include "OgreBulletDynamics.h"
#include "OgreBulletCollisions.h"

#include <Ogre/Ogre.h>

#include <boost/shared_ptr.hpp>


namespace VSC {
    
    namespace OB {
        
        /*
         *  More of a container than a class...
         */
            
        class StaticGeometry : public Scene::Element {
            
        public:
            
            typedef boost::shared_ptr<StaticGeometry>   SPtr;
            typedef boost::weak_ptr<StaticGeometry>     WPtr;
            
            /*
             *  Constructor
             */
            
            Object(Scene::WPtr scene,
                   Ogre::StaticGeometry* staticGeometry,
                   OgreBulletDynamics::RigidBody* rigidBody,
                   OgreBulletCollisions::CollisionShape* collisionShape);
            
            /*
             *  Getters
             */
            
            Ogre::StaticGeometry*                   getGeometry(void);
            OgreBulletDynamics::RigidBody*          getRigidBody(void);
            OgreBulletCollisions::CollisionShape*   getCollisionShape(void);
            
        private:
            
            Ogre::StaticGeometry*                   mStaticGeometry;
            OgreBulletDynamics::RigidBody*          mRigidBody;
            OgreBulletCollisions::CollisionShape*   mCollisionShape;
        }
        
    }
}

#endif //_VSC_OGRE_BULLET_STATIC_GEOMETRY_H_

