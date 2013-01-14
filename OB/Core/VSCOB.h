
#ifndef _VSC_OGRE_BULLET_H_
#define _VSC_OGRE_BULLET_H_

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include <vector>

namespace VSC {
    
    namespace OB {
        
        /*
         *  Globally used enums.
         */

        enum QueryMask
        {
            QueryMaskNone                   = 0,
            QueryMaskAny					= 1<<0,
            QueryMaskRagdoll				= 1<<1,
            QueryMaskGeometry				= 1<<2,
            QueryMaskVehicle				= 1<<3,
            QueryMaskStaticGeometry         = 1<<4
        };
        
        enum PrimitiveType {
            PrimitiveNone = 0,
            PrimitiveCube,
            PrimitiveSphere,
            PrimitiveCylinder,
            PrimitiveCone
        };
        
        /*
         *  Forward declarations
         */
        
        class Application;
        typedef boost::shared_ptr<Application>          Application_SPtr;
        typedef boost::weak_ptr<Application>            Application_WPtr;
        
        class Scene;
        typedef boost::shared_ptr<Scene>                Scene_SPtr; 
        typedef boost::weak_ptr<Scene>                  Scene_WPtr;
        
        class Element;
        typedef boost::shared_ptr<Element>              Element_SPtr;
        typedef boost::weak_ptr<Element>                Element_WPtr;
        typedef std::vector<Element_SPtr>               Elements;
        
        class ElementFactory;
        typedef boost::shared_ptr<ElementFactory>       ElementFactory_SPtr;
        typedef boost::weak_ptr<ElementFactory>         ElementFactory_WPtr;
        
        class Collision;
        typedef boost::shared_ptr<Collision>            Collision_SPtr;
        typedef boost::weak_ptr<Collision>              Collision_WPtr;
        typedef std::vector<Collision_SPtr>             Collisions;
        
        class CollisionDetector;
        typedef boost::shared_ptr<CollisionDetector>    CollisionDetector_SPtr;
        typedef boost::weak_ptr<CollisionDetector>      CollisionDetector_WPtr;
        
    }
}

#endif //_VSC_OGRE_BULLET_H_

