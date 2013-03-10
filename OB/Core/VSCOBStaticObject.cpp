
#include "VSCOBStaticObject.h"
#include "VSCOB.h"
#include "VSCOBElement.h"
#include "VSCOBScene.h"

VSC::OB::StaticObject::FactoryDescription::FactoryDescription() : // set defaults
name("DefaultStatic"),
bodyRestitution(0.1f),
bodyFriction(0.8f),
//materialName("BulletPlane")      // "BulletPlane"
materialName("VSC/FloorStones1")
{
    
}

VSC::OB::StaticObject::StaticObject(Scene::WPtr scene,
                                    Ogre::StaticGeometry* staticGeometry,
                                    OgreBulletDynamics::RigidBody* rigidBody) :
Element(scene, rigidBody),
mStaticGeometry(staticGeometry)
{
    
}

void VSC::OB::StaticObject::destroy(void)
{
    Element::destroy();  // call base class destroy() !!!
    
    Scene::SPtr scene = this->getScene();
    
    if (scene)
    {
        scene->getSceneManager()->destroyStaticGeometry(mStaticGeometry);
        mStaticGeometry = 0;
    }
}

