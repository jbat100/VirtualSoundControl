
#include "VSCOBDynamicObject.h"

#include <boost/assert.hpp>

VSC::OB::DynamicObject::FactoryDespription::FactoryDespription() : // set defaults
name("DefaultDynamic"),                                
position(Ogre::Vector3::ZERO)                       
orientation(Ogre::Quaternion(0.0,0.0,0.0,1.0)),     
size(Ogre::Vector3(1.0,1.0,1.0)),                   
bodyMass(1.0),                                      
bodyFriction(0.6),                                  
bodyRestitution(0.6),                               
materialName("Bullet/box"),                         
castsShadow(true),                                  
{

}

VSC::OB::DynamicObject::DynamicObject(Scene::WPtr scene,
                        Ogre::Entity* entity,
                        OgreBulletDynamics::RigidBody* rigidBody) :
Scene::Element(scene, rigidBody),
mEntity(entity)
{
    
}

virtual void destroy(void)
{
    Scene::Element::destroy(); // call base destroy() !!!
    
    SceneNode *node = static_cast<SceneNode*>(mEntity->getParentNode());
    node->detachObject(mEntity);
    this->getScene().lock()->getSceneManager()->destroyEntity(mEntity);
    BOOST_ASSERT_MSG (node->getParent(), "Expected parent node");
    static_cast <SceneNode*>(node->getParent())->removeAndDestroyChild(node->getName());
    mEntity = 0;
}

Ogre::Entity* VSC::OB::DynamicObject::getEntity(void)
{
    return mEntity;
}




