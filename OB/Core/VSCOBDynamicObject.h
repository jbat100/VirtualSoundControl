
#ifndef _VSC_OB_DYNAMIC_OBJECT_H_
#define _VSC_OB_DYNAMIC_OBJECT_H_

#include "VSCOB.h"
#include "VSCOBElement.h"

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
            
        class DynamicObject : public Element {
            
        public:
            
            typedef boost::shared_ptr<DynamicObject>    SPtr;
            typedef boost::weak_ptr<DynamicObject>      WPtr;
            
            /*
             *  FactoryDescription is designed to simplify the passing of arguments
             *  
             */
            
            struct FactoryDescription
            {
                FactoryDescription(); // see cpp for defaults
                
                Ogre::String        name;               // "DefaultName"
                Ogre::Vector3       position;           // Vector3::ZERO
                Ogre::Quaternion    orientation;        // Quaternion(0,0,0,1)
                Ogre::Vector3       size;               // 1 1 1
                Ogre::Real          bodyMass;           // 1.0f;
                Ogre::Real          bodyFriction;       // 0.6f;
                Ogre::Real          bodyRestitution;    // 0.6f;
                Ogre::String        materialName;       // "Bullet/box"
                bool                castsShadow;        // true
            };
            
            /**------------------------------------------------------------
             *  Constructor
             */
            
            DynamicObject(Scene_WPtr scene,
                          Ogre::Entity* entity,
                          OgreBulletDynamics::RigidBody* rigidBody);
            
            /**------------------------------------------------------------
             *  Destroy
             */
            
            virtual void destroy(void);
            
            /**------------------------------------------------------------
             *  Getters
             */
            
            Ogre::Entity*                           getEntity(void) {return mEntity;}
            
        private:
            
            Ogre::Entity*                           mEntity;
        };
        
    }
}

#endif //_VSC_OB_DYNAMIC_OBJECT_H_

