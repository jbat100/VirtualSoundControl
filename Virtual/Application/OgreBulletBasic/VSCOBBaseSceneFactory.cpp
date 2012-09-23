
VSC::OB::DynamicObject::SPtr VSC::OB::ObjectFactory::addPrimitive(Object::PrimitiveType primitiveType,
                                                                  const DynamicObject::FactoryDespription& description);
{
    
    /*
     *  Depending on primitive type:
     *
     *  Create an Ogre entity, and attach it to a node. Why no mention of size ?
     *  Create a corresponding OgreBulletCollisions::BoxCollisionShape which just represents the shape
     */
    
    Ogre::Entity *entity = 0;
    OgreBulletCollisions::CollisionShape *collisionShape = 0;
    Ogre::String entityName = description.name + StringConverter::toString(mNumberOfObjectsCreated);
    Ogre::String rigidBodyName = "";
    
    switch (primitiveType) {
            
        case Object::PrimitiveCube:
            entity =  mSceneMgr->createEntity(entityName, "Bulletbox.mesh");
            // "Crate.mesh", "Crate1.mesh", "Crate2.mesh"
            collisionShape = new OgreBulletCollisions::BoxCollisionShape(size);
            rigidBodyName = "RigidCube";
            break;
            
        case Object::PrimitiveSphere:
            entity = mSceneMgr->createEntity(entityName, "ellipsoid.mesh");
            collisionShape = new OgreBulletCollisions::SphereCollisionShape(radius);
            rigidBodyName = "RigidSphere";
            break;
            
        case Object::PrimitiveCylinder:
            entity = mSceneMgr->createEntity(entityName, "cylinder.mesh");
            // "capsule.mesh" "Barrel.mesh" "Barrel1.mesh" "Barrel2.mesh"
            collisionShape = new OgreBulletCollisions::CylinderCollisionShape(size, Ogre::Vector3::UNIT_X);
            rigidBodyName = "RigidCylinder";
            break;
            
        case Object::PrimitiveCone:
            entity = mSceneMgr->createEntity(entityName, "Mesh.mesh");
            //"Cone.mesh");
            collisionShape = new OgreBulletCollisions::ConeCollisionShape(size.x, size.y, Ogre::Vector3::UNIT_Y);
            rigidBodyName = "RigidCylinder";
            break;
            
        default:
            // TODO throw exception
            return 0;
            break;
    }
    
    entity->setQueryFlags(VSC::OB::QueryMaskGeometry);
	entity->setCastShadows(true);
    entity->setMaterialName(description.materialName);
    
    SceneNode *node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    node->attachObject(entity);
    
    /*
     *  Create an OgreBulletDynamics::RigidBody which handles the Ogre::SceneNode (along with the Ogre::Entity), 
     *  the OgreBulletCollisions::BoxCollisionShape,
     *  friction, mass, position and orientation (quaternion).
     */
    
    Ogre::String fullRigidBodyName = rigidBodyName + StringConverter::toString(mNumberOfObjectsCreated);
    
    OgreBulletDynamics::DynamicsWorld* world = this->getScene().lock()->getWorld();
    
    OgreBulletDynamics::RigidBody *rigidBody = new OgreBulletDynamics::RigidBody(fullRigidBodyName, world);
    
    rigidBody->setShape(node, collisionShape, description.bodyRestitution,
                        description.bodyFriction, description.bodyMass, description.position, description.orientation);
    
    mNumberOfObjectsCreated++;
    
    VSC::OB::DynamicObject::SPtr object(new VSC::OB::DynamicObject(this->getScene(), entity, rigidBody, collisionShape));

    return object;
}


VSC::OB::Object::SPtr VSC::OB::ObjectFactory::addStaticTrimesh(const Ogre::String &instanceName,
                                                                const Ogre::String &meshName,
                                                                const Ogre::Vector3 &pos,
                                                                const Ogre::Quaternion &q,
                                                                const Ogre::Real bodyRestitution,
                                                                const Ogre::Real bodyFriction)
{
    /*
     *  Create an Ogre::Entity with the specified mesh, and an associated scene node.
     */
    
    Ogre::String entityName = instanceName + StringConverter::toString(mNumberOfObjectsCreated);
    Ogre::Entity *sceneEntity = mSceneMgr->createEntity(instanceName + StringConverter::toString(mNumberOfObjectsCreated), meshName);
    sceneEntity->setCastShadows(true);
    SceneNode *node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    node->attachObject(sceneEntity);

    /*
     *  Create a OgreBulletCollisions::TriangleMeshCollisionShape from the entity using the 
     *  OgreBulletCollisions::StaticMeshToShapeConverter util.
     */
    
    OgreBulletCollisions::StaticMeshToShapeConverter *trimeshConverter = new StaticMeshToShapeConverter(sceneEntity);
    OgreBulletCollisions::TriangleMeshCollisionShape *sceneTriMeshShape = trimeshConverter->createTrimesh();
    delete trimeshConverter;
    
    Ogre::String rigidBodyName = "Rigid" + instanceName  + StringConverter::toString(mNumberOfObjectsCreated)
    OgreBulletDynamics::RigidBody *sceneRigid = new OgreBulletDynamics::RigidBody(rigidBodyName, mWorld);
    sceneRigid->setStaticShape(node, sceneTriMeshShape, bodyRestitution, bodyFriction, pos);
    
    mNumberOfObjectsCreated++;
    
    Object::SPtr object(new Object(sceneEntity, mWorld, sceneRigid, sceneTriMeshShape));

    return sceneRigid;
}


OgreBulletDynamics::RigidBody *VSC::OB::Scene::addStaticPlane( const Ogre::Real bodyRestitution, 
                                              const Ogre::Real bodyFriction)
{
    // Use a load of meshes to represent the floor
    int i = 0;
    
    StaticGeometry* s = mSceneMgr->createStaticGeometry("StaticFloor");
    s->setRegionDimensions(Ogre::Vector3(160.0, 100.0, 160.0));
    
    // Set the region origin so the center is at 0 world
    s->setOrigin(Ogre::Vector3::ZERO);
    
    for (Real z = -80.0; z <= 80.0; z += 20.0)
    {
        for (Real x = -80.0; x <= 80.0; x += 20.0)
        {
            String name = String("Plane_") + StringConverter::toString(i++);

            Entity* entity = mSceneMgr->createEntity(name, "plane.mesh");
			entity->setMaterialName("BulletPlane");
            entity->setQueryFlags (STATIC_GEOMETRY_QUERY_MASK);
            //entity->setUserObject(_plane);
            entity->setCastShadows(false);
            s->addEntity(entity, Ogre::Vector3(x,0,z));
        }
    }
    
    s->build();
    
    CollisionShape *shape = new StaticPlaneCollisionShape (Ogre::Vector3(0,1,0), 0);

    OgreBulletDynamics::RigidBody *defaultPlaneBody = new OgreBulletDynamics::RigidBody("Plane" + StringConverter::toString(mNumEntitiesInstanced), mWorld);
    defaultPlaneBody->setStaticShape(shape, bodyRestitution, bodyFriction);

    mBodies.push_back(defaultPlaneBody);
    mShapes.push_back(shape);

    mNumEntitiesInstanced++;

    return defaultPlaneBody;
}

