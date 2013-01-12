
#include "VSCOBStaticObject.h"

VSC::OB::StaticObject::FactoryDescription::FactoryDescription() : // set defaults
name("DefaultStatic"),
bodyRestitution(0.1f),
bodyFriction(0.8f),
materialName("BulletPlane")      // "BulletPlane"
{
    
}

VSC::OB::StaticObject::StaticObject(Scene::WPtr scene,
                                    Ogre::StaticGeometry* staticGeometry,
                                    OgreBulletDynamics::RigidBody* rigidBody) :
Scene::Element(scene, rigidBody),
mStaticGeometry(staticGeometry)
{
    
}

void VSC::OB::StaticObject::destroy(void)
{
    Scene::Element::destroy();  // call base class destroy() !!!
    
    Scene::SPtr scene = this->getScene();
    
    if (scene)
    {
        scene->getSceneManager()->destroyStaticGeometry(mStaticGeometry);
        mStaticGeometry = 0;
    }
}

