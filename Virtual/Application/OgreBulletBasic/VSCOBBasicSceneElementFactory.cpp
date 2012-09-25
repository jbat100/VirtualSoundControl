
#include "VSCOBBasicSceneElementFactory.h"

#include "OgreBulletCollisions.h"
#include "OgreBulletDynamics.h"

// -------------------------------------------------------------------------
void VSC::OB::BasicSceneElementFactory::addGround()
{
    StaticObject::FactoryDescription description;
    
    description.name = "Ground";
    
    this->addStaticPlane(description);
}

VSC::OB::DynamicObject::WPtr VSC::OB::BasicSceneElementFactory::addPrimitive(PrimitiveType primitiveType,
                                                                             const DynamicObject::FactoryDespription& description)
{
    
    /*
     *  Depending on primitive type:
     *
     *  Create an Ogre entity, and attach it to a node. Why no mention of size ?
     *  Create a corresponding OgreBulletCollisions::BoxCollisionShape which just represents the shape
     */
    
    Ogre::Entity *entity = 0;
    OgreBulletCollisions::CollisionShape* collisionShape = 0;
    Ogre::String entityName = description.name + Ogre::String("_") + Ogre::StringConverter::toString(mNumberOfObjectsCreated);
    Ogre::String rigidBodyName = "";
    
    switch (primitiveType) {
            
        case PrimitiveCube:
            entity =  this->getScene().lock()->getSceneManager()->createEntity(entityName, "Bulletbox.mesh");
            // "Crate.mesh", "Crate1.mesh", "Crate2.mesh"
            collisionShape = new OgreBulletCollisions::BoxCollisionShape(description.size);
            rigidBodyName = "RigidCube_" + Ogre::StringConverter::toString(mNumberOfObjectsCreated);
            break;
            
        case PrimitiveSphere:
            entity = this->getScene().lock()->getSceneManager()->createEntity(entityName, "ellipsoid.mesh");
            collisionShape = new OgreBulletCollisions::SphereCollisionShape(description.size.x);
            rigidBodyName = "RigidSphere_" + Ogre::StringConverter::toString(mNumberOfObjectsCreated);
            break;
            
        case PrimitiveCylinder:
            entity = this->getScene().lock()->getSceneManager()->createEntity(entityName, "cylinder.mesh");
            // "capsule.mesh" "Barrel.mesh" "Barrel1.mesh" "Barrel2.mesh"
            collisionShape = new OgreBulletCollisions::CylinderCollisionShape(description.size, Ogre::Vector3::UNIT_X);
            rigidBodyName = "RigidCylinder_" + Ogre::StringConverter::toString(mNumberOfObjectsCreated);
            break;
            
        case PrimitiveCone:
            entity = this->getScene().lock()->getSceneManager()->createEntity(entityName, "Mesh.mesh");
            //"Cone.mesh");
            collisionShape = new OgreBulletCollisions::ConeCollisionShape(description.size.x, description.size.y, Ogre::Vector3::UNIT_Y);
            rigidBodyName = "RigidCylinder_" + Ogre::StringConverter::toString(mNumberOfObjectsCreated);
            break;
            
        default:
            // TODO throw exception
            return DynamicObject::WPtr();
            break;
    }
    
    entity->setQueryFlags(VSC::OB::QueryMaskGeometry);
	entity->setCastShadows(description.castsShadow);
    entity->setMaterialName(description.materialName);
    
    Ogre::SceneNode *node = this->getScene().lock()->getSceneManager()->getRootSceneNode()->createChildSceneNode();
    node->attachObject(entity);
    
    /*
     *  Create an OgreBulletDynamics::RigidBody which handles the Ogre::SceneNode (along with the Ogre::Entity), 
     *  the OgreBulletCollisions::BoxCollisionShape,
     *  friction, mass, position and orientation (quaternion).
     */
    
    Ogre::String fullRigidBodyName = rigidBodyName + Ogre::StringConverter::toString(mNumberOfObjectsCreated);
    OgreBulletDynamics::DynamicsWorld* world = this->getScene().lock()->getDynamicsWorld();
    
    OgreBulletDynamics::RigidBody *rigidBody = new OgreBulletDynamics::RigidBody(fullRigidBodyName, world);
    
    rigidBody->setShape(node, collisionShape, description.bodyRestitution,
                        description.bodyFriction, description.bodyMass, description.position, description.orientation);
    
    mNumberOfObjectsCreated++;
    
    DynamicObject::SPtr object(new VSC::OB::DynamicObject(this->getScene(), entity, rigidBody));
    Scene::Element::SPtr element = boost::static_pointer_cast<Scene::Element>(object);
    this->registerElement(element);

    return DynamicObject::WPtr(object);
}


VSC::OB::DynamicObject::WPtr VSC::OB::BasicSceneElementFactory::addTrimesh(const Ogre::String& meshName,
                                                                const DynamicObject::FactoryDespription& description)
{
    /*
     *  Create an Ogre::Entity with the specified mesh, and an associated scene node.
     */
    
    Ogre::String entityName = description.name + String("_") + Ogre::StringConverter::toString(mNumberOfObjectsCreated);
    Ogre::Entity *entity = this->getScene().lock()->getSceneManager()->createEntity(entityName, meshName);
    entity->setCastShadows(description.castsShadow);
    Ogre::SceneNode *node = this->getScene().lock()->getSceneManager()->getRootSceneNode()->createChildSceneNode();
    node->attachObject(entity);

    /*
     *  Create a OgreBulletCollisions::TriangleMeshCollisionShape from the entity using the 
     *  OgreBulletCollisions::StaticMeshToShapeConverter util.
     */
    
    OgreBulletCollisions::StaticMeshToShapeConverter *trimeshConverter = new StaticMeshToShapeConverter(entity);
    OgreBulletCollisions::TriangleMeshCollisionShape *sceneTriMeshShape = trimeshConverter->createTrimesh();
    delete trimeshConverter;
    
    OgreBulletDynamics::DynamicsWorld* world = this->getScene().lock()->getWorld();
    Ogre::String rigidBodyName = "RigidTrimesh_" + entityName;
    OgreBulletDynamics::RigidBody *sceneRigid = new OgreBulletDynamics::RigidBody(rigidBodyName, world);
    sceneRigid->setStaticShape(node, sceneTriMeshShape, bodyRestitution, bodyFriction, pos);
    
    mNumberOfObjectsCreated++;
    
    DynamicObject::SPtr object(new Object(this->getScene(), entity, sceneRigid));
    Scene::Element::SPtr element = boost::static_pointer_cast<Scene::Element>(object);
    this->registerElement(element);
    
    return DynamicObject::WPtr(object);
}


VSC::OB::StaticGeometry::WPtr VSC::OB::BasicSceneElementFactory::addStaticPlane(const StaticGeometry::FactoryDespription& description)
{
    // Use a load of meshes to represent the floor
    int i = 0;
    
    Ogre::String name = description.name + String("_") + StringConverter::toString(mNumberOfObjectsCreated);
    Ogre::StaticGeometry* s = this->getScene().lock()->getSceneManager()->createStaticGeometry(name);
    
    s->setRegionDimensions(Ogre::Vector3(160.0, 100.0, 160.0));
    
    // Set the region origin so the center is at 0 world
    s->setOrigin(Ogre::Vector3::ZERO);
    
    for (Real z = -80.0; z <= 80.0; z += 20.0)
    {
        for (Real x = -80.0; x <= 80.0; x += 20.0)
        {
            Ogre::String entityName = name + String("_ogreplane_") + StringConverter::toString(i++);
            Ogre::Entity* entity = this->getScene().lock()->getSceneManager()->createEntity(name, "plane.mesh");
			entity->setMaterialName(description.materialName);
            entity->setQueryFlags(VSC::OB::QueryMaskStaticGeometry);
            entity->setCastShadows(false);
            s->addEntity(entity, Ogre::Vector3(x,0,z));
            
            /*
             *  Who takes charge of destroying the entities, the scene manager all by himself?
             *  Looking at the SceneManager class, it would seem so
             *  http://www.ogre3d.org/docs/api/html/classOgre_1_1SceneManager.html#a54d553be3f6098ce229e6db53ec15227
             *  All the VSC::OB::StaticGeometry has to do is to destroy the Ogre::StaticGeometry via the 
             *  Ogre::SceneManager
             */
            
        }
    }
    
    s->build();
    
    Ogre::String rigidBodyName = name + "RigigPlane_" + StringConverter::toString(mNumberOfObjectsCreated)
    OgreBulletCollisions::CollisionShape *shape = new OgreBulletCollisions::StaticPlaneCollisionShape (Ogre::Vector3(0,1,0), 0);
    OgreBulletDynamics::RigidBody *rigidPlaneBody = new OgreBulletDynamics::RigidBody(rigidBodyName, mWorld);
    rigidPlaneBody->setStaticShape(shape, bodyRestitution, bodyFriction);

    mNumberOfObjectsCreated++;
    
    StaticGeometry::SPtr geom(new Object(this->getScene(), s, rigidPlaneBody));
    Scene::Element::SPtr element = boost::static_pointer_cast<Scene::Element>(geom);
    this->registerElement(element);

    return StaticGeometry::WPtr(geom);;
}

