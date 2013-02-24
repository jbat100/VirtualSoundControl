
#include "VSCOBDynamicObject.h"
#include "VSCOB.h"
#include "VSCOBElement.h"
#include "VSCOB.h"
#include "VSCOBScene.h"

#include <boost/assert.hpp>

VSC::OB::DynamicObject::FactoryDescription::FactoryDescription() : // set defaults
name("DefaultDynamic"),                                
position(Ogre::Vector3::ZERO),
orientation(Ogre::Quaternion(0.0,0.0,0.0,1.0)),     
size(Ogre::Vector3(1.0,1.0,1.0)),                   
bodyMass(1.0),                                      
bodyFriction(0.6),                                  
bodyRestitution(0.6),                               
materialName("Bullet/box"),                         
castsShadow(true)                      
{

}

VSC::OB::DynamicObject::DynamicObject(Scene::WPtr scene,
                        Ogre::Entity* entity,
                        OgreBulletDynamics::RigidBody* rigidBody) :
Element(scene, rigidBody),
mEntity(entity)
{
    
}

void VSC::OB::DynamicObject::destroy(void)
{
    Element::destroy(); // call base destroy() !!!
    
    Ogre::SceneNode *node = dynamic_cast<Ogre::SceneNode*>(mEntity->getParentNode());
    if (node)
    {
        node->detachObject(mEntity);
        BOOST_ASSERT_MSG (node->getParent(), "Expected parent node");
        Ogre::SceneNode *parentNode = dynamic_cast <Ogre::SceneNode*>(node->getParent());
        if (parentNode) parentNode->removeAndDestroyChild(node->getName());
    }
    
    Scene::SPtr scene = this->getScene();
    if (scene)
    {
        scene->getSceneManager()->destroyEntity(mEntity);
    }

    mEntity = 0;
}

/*
Ogre::Entity* VSC::OB::DynamicObject::getEntity(void)
{
    return mEntity;
}
 */




