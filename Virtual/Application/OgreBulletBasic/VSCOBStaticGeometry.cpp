
VSC::OB::Object::Object(Ogre::Entity* entity,
                        OgreBulletDynamics::DynamicsWorld* world,
                        OgreBulletDynamics::RigidBody* rigidBody,
                        OgreBulletCollisions::CollisionShape* collisionShape
                        PrimitiveType primitiveType) :
mEntity(entity),
mRigidBody(rigidBody),
mCollisionShape(collisionShape),
mWorld(world),
mPrimitiveType(primitiveType)
{
    
}

Ogre::Entity* VSC::OB::Object::getEntity(void)
{
    return mEntity;
}

OgreBulletDynamics::RigidBody* VSC::OB::Object::getRigidBody(void)
{
    return mRigidBody;
}

OgreBulletCollisions::CollisionShape* VSC::OB::Object::getCollisionShape(void)
{
    return mCollisionShape;
}

OgreBulletDynamics::DynamicsWorld* VSC::OB::Object::getWorld(void)
{
    return mWorld;
}

PrimitiveType VSC::OB::Object::getPrimitiveType(void)
{
    return mPrimitiveType;
}



