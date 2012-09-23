
VSC::OB::DynamicObject::FactoryDespription::FactoryDespription() // set defaults
{}

VSC::OB::DynamicObject::DynamicObject(Scene::WPtr scene,
                        Ogre::Entity* entity,
                        OgreBulletDynamics::RigidBody* rigidBody,
                        OgreBulletCollisions::CollisionShape* collisionShape) :
Scene::Element(scene),
mEntity(entity),
mRigidBody(rigidBody),
mCollisionShape(collisionShape),
{
    
}

Ogre::Entity* VSC::OB::DynamicObject::getEntity(void)
{
    return mEntity;
}

OgreBulletDynamics::RigidBody* VSC::OB::DynamicObject::getRigidBody(void)
{
    return mRigidBody;
}

OgreBulletCollisions::CollisionShape* VSC::OB::DynamicObject::getCollisionShape(void)
{
    return mCollisionShape;
}




