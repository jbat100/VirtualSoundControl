
#ifndef _VSC_OB_ELEMENT_FACTORY_H_
#define _VSC_OB_ELEMENT_FACTORY_H_

#include "VSCOB.h"
#include "VSCBroadcaster.h"
#include "VSCListener.h"
#include "VSCOBElement.h"
#include "VSCOBDynamicObject.h"
#include "VSCOBStaticObject.h"

#include "OgreBulletDynamics.h"

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/enable_shared_from_this.hpp> 

#include <map>
#include <string>


namespace VSC {
    
    namespace OB {
        
        /*
         *  ElementFactory is used to generate Element and keep a shared_ptr to them. Like Element, it is meant
         *  to be subclassed to be useful.
         */
        
        class ElementFactory
        {
        public:
            
            typedef boost::shared_ptr<ElementFactory> SPtr;
            
            ElementFactory(Scene_WPtr scene) : mScene(scene) {}
            virtual ~ElementFactory() {}
            
            Scene_SPtr getScene(void) {return mScene.lock();}; // no public setter, cannot change the element scene.
            
            /*
             *  Abstract API for adding the stuff to the world. Used directly by VSC::OB::Scene 
             */
            virtual void addGround(void);
            
            DynamicObject::SPtr addPrimitive(PrimitiveType primitiveType, const DynamicObject::FactoryDescription& description);
            
            DynamicObject::SPtr addTrimesh(const Ogre::String& meshName, const DynamicObject::FactoryDescription& description);
            
            StaticObject::SPtr addStaticPlane(const StaticObject::FactoryDescription& description);
            
        protected:
            
            void setElementMass(Element_SPtr element, Float mass);
            
            void materialTest(const Ogre::String& materialName);
            
            void entityMaterialTest(Ogre::Entity* entity);
            
        private:
            
            Scene_WPtr mScene;
            
            static int mNumberOfObjectsCreated;
            
            static const bool mTraceMaterial = true;
            
        };
        
        
    }
}

#endif //_VSC_OB_ELEMENT_FACTORY_H_

