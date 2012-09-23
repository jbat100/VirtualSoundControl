
#ifndef _VSC_OGRE_BULLET_SCENE_ELEMENT_H_
#define _VSC_OGRE_BULLET_SCENE_ELEMENT_H_

#include "OgreBulletDynamics.h"
#include "OgreBulletCollisions.h"

#include <Ogre/Ogre.h>

#include <boost/shared_ptr.hpp>


namespace VSC {
    
    namespace OB {
        
        /*
         *  More of a container than a class...
         */
        
        class Scene;
            
        class SceneElement {
            
        public:
            
            typedef boost::shared_ptr<Object> SPtr;
            
            /*
             *  Constructor
             */
            
            SceneElement(Scene* scene);
            virtual ~SceneElement(void); // destructor must call destroy()
            
        protected:
            
            /*
             *  Will perform all necessary cleaning up, removing the object from the scene,
             *  removing from Ogre::SceneManager and OgreBulletDynamics::World
             */
            
            virtual void destroy(void) = 0;
            
        private:
            
            Scene* mScene;
        }
        
    }
}

#endif //_VSC_OGRE_BULLET_SCENE_ELEMENT_H_

