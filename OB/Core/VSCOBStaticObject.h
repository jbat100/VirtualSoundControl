
#ifndef _VSC_OB_STATIC_OBJECT_H_
#define _VSC_OB_STATIC_OBJECT_H_

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
            
        class StaticObject : public Element {
            
        public:
            
            typedef boost::shared_ptr<StaticObject>   SPtr;
            typedef boost::weak_ptr<StaticObject>     WPtr;
            
            struct FactoryDescription {
                
                FactoryDescription();
                
                Ogre::String    name;
                Ogre::Real      bodyRestitution;
                Ogre::Real      bodyFriction;
                Ogre::String    materialName;
                
            };
            
            /**------------------------------------------------------------
             *  Constructor
             */
            
            StaticObject(Scene_WPtr scene,
                         Ogre::StaticGeometry* staticGeometry,
                         OgreBulletDynamics::RigidBody* rigidBody);
            
            /**------------------------------------------------------------
             *  Destroy
             */
            
            virtual void destroy(void);
            
            /**------------------------------------------------------------
             *  Getters
             */
            
            Ogre::StaticGeometry*                   getGeometry(void) {return mStaticGeometry;}
            
        private:
            
            Ogre::StaticGeometry*                   mStaticGeometry;

        };
        
    }
}

#endif //_VSC_OB_STATIC_OBJECT_H_

