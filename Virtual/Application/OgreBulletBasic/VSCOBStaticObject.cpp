

VSC::OB::StaticObject::FactoryDespription::FactoryDespription() // set defaults
{
    
}

VSC::OB::StaticObject::StaticObject(Scene::WPtr scene,
                                    Ogre::StaticGeometry* staticGeometry,
                                    OgreBulletDynamics::RigidBody* rigidBody) :
Scene::Element(scene, rigidBody),
mStaticGeometry(staticGeometry)
{
    
}

virtual void destroy(void)
{
    delete mRigidBody;
    mRigidBody = 0;
    
    this->getScene().lock()->getSceneManager()->destroyStaticGeometry(mStaticGeometry);
    mStaticGeometry = 0;
}

