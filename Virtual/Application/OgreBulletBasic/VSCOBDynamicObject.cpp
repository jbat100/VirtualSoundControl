
#include <boost/assert.hpp>

VSC::OB::DynamicObject::FactoryDespription::FactoryDespription() // set defaults
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
    delete mRigidBody;
    mRigidBody = 0;
    
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




