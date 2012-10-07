
#include "VSCOBBasicSceneElementFactory.h"

#include "OgreBulletCollisions.h"
#include "OgreBulletDynamics.h"

#include "OgreBulletDynamicsRigidBody.h"

/*
 *  OgreBullet Shapes
 */
#include "OgreBulletCollisionsShape.h"
#include "Shapes/OgreBulletCollisionsBoxShape.h"
#include "Shapes/OgreBulletCollisionsSphereShape.h"
#include "Shapes/OgreBulletCollisionsConeShape.h"
#include "Shapes/OgreBulletCollisionsCylinderShape.h"
#include "Shapes/OgreBulletCollisionsTriangleShape.h"
#include "Shapes/OgreBulletCollisionsStaticPlaneShape.h"
#include "Shapes/OgreBulletCollisionsCompoundShape.h"
#include "Shapes/OgreBulletCollisionsMultiSphereShape.h"
#include "Shapes/OgreBulletCollisionsConvexHullShape.h"
#include "Shapes/OgreBulletCollisionsMinkowskiSumShape.h"
#include "Shapes/OgreBulletCollisionsTrimeshShape.h"

/**
 *
 */
#include "Utils/OgreBulletCollisionsMeshToShapeConverter.h"
#include "OgreBulletCollisionsRay.h"
#include "Debug/OgreBulletCollisionsDebugLines.h"

int VSC::OB::BasicSceneElementFactory::mNumberOfObjectsCreated = 0;

// -------------------------------------------------------------------------
void VSC::OB::BasicSceneElementFactory::addGround()
{
    StaticObject::FactoryDescription description;
    
    description.name = "Ground";
    
    StaticObject::SPtr obj = this->addStaticPlane(description);
    
    obj->setSilentCollisions(true);
}

VSC::OB::DynamicObject::SPtr VSC::OB::BasicSceneElementFactory::addPrimitive(PrimitiveType primitiveType,
                                                                             const DynamicObject::FactoryDescription& description)
{
    
    /*
     *  Depending on primitive type:
     *
     *  Create an Ogre entity, and attach it to a node. Why no mention of size ?
     *  Create a corresponding OgreBulletCollisions::BoxCollisionShape which just represents the shape
     */
    
    VSC::OB::Scene::SPtr scene = this->getScene().lock();
    
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
            return DynamicObject::SPtr();
            break;
    }
    
    entity->setQueryFlags(VSC::OB::QueryMaskGeometry);
	entity->setCastShadows(description.castsShadow);
    entity->setMaterialName(description.materialName);
    
    Ogre::SceneNode *node = scene->getSceneManager()->getRootSceneNode()->createChildSceneNode();
    node->attachObject(entity);
    
    /*
     *  Create an OgreBulletDynamics::RigidBody which handles the Ogre::SceneNode (along with the Ogre::Entity), 
     *  the OgreBulletCollisions::BoxCollisionShape,
     *  friction, mass, position and orientation (quaternion).
     */
    
    Ogre::String fullRigidBodyName = rigidBodyName + Ogre::StringConverter::toString(mNumberOfObjectsCreated);
    OgreBulletDynamics::DynamicsWorld* world = scene->getDynamicsWorld();
    
    OgreBulletDynamics::RigidBody *rigidBody = new OgreBulletDynamics::RigidBody(fullRigidBodyName, world);
    
    rigidBody->setShape(node, collisionShape, description.bodyRestitution,
                        description.bodyFriction, description.bodyMass, description.position, description.orientation);
    
    
    DynamicObject::SPtr object(new VSC::OB::DynamicObject(this->getScene(), entity, rigidBody));
    Scene::Element::SPtr element = boost::static_pointer_cast<Scene::Element>(object);
    
    scene->registerElement(element, description.name, mNumberOfObjectsCreated);
    
    mNumberOfObjectsCreated++;

    return object;
}


VSC::OB::DynamicObject::SPtr VSC::OB::BasicSceneElementFactory::addTrimesh(const Ogre::String& meshName,
                                                                           const DynamicObject::FactoryDescription& description)
{
    VSC::OB::Scene::SPtr scene = this->getScene().lock();
    
    /*
     *  Create an Ogre::Entity with the specified mesh, and an associated scene node.
     */
    
    Ogre::String entityName = description.name + "_" + Ogre::StringConverter::toString(mNumberOfObjectsCreated);
    Ogre::Entity *entity = scene->getSceneManager()->createEntity(entityName, meshName);
    entity->setCastShadows(description.castsShadow);
    Ogre::SceneNode *node = scene->getSceneManager()->getRootSceneNode()->createChildSceneNode();
    node->attachObject(entity);

    /*
     *  Create a OgreBulletCollisions::TriangleMeshCollisionShape from the entity using the 
     *  OgreBulletCollisions::StaticMeshToShapeConverter util.
     */
    
    OgreBulletCollisions::StaticMeshToShapeConverter *trimeshConverter = new OgreBulletCollisions::StaticMeshToShapeConverter(entity);
    OgreBulletCollisions::TriangleMeshCollisionShape *sceneTriMeshShape = trimeshConverter->createTrimesh();
    delete trimeshConverter;
    
    OgreBulletDynamics::DynamicsWorld* world = scene->getDynamicsWorld();
    Ogre::String rigidBodyName = "RigidTrimesh_" + entityName;
    OgreBulletDynamics::RigidBody *sceneRigid = new OgreBulletDynamics::RigidBody(rigidBodyName, world);
    sceneRigid->setStaticShape(node, sceneTriMeshShape, description.bodyRestitution, description.bodyFriction, description.position);
    
    DynamicObject::SPtr object(new DynamicObject(this->getScene(), entity, sceneRigid));
    Scene::Element::SPtr element = boost::static_pointer_cast<Scene::Element>(object);
    
    scene->registerElement(element, description.name, mNumberOfObjectsCreated);
    
    mNumberOfObjectsCreated++;
    
    return object;
}


VSC::OB::StaticObject::SPtr VSC::OB::BasicSceneElementFactory::addStaticPlane(const StaticObject::FactoryDescription& description)
{
    // Use a load of meshes to represent the floor
    int i = 0;
    
    VSC::OB::Scene::SPtr scene = this->getScene().lock();
    
    BOOST_ASSERT_MSG(scene->getSceneManager(), "Expected scene manager");
    
    Ogre::String name = description.name + "_" + Ogre::StringConverter::toString(mNumberOfObjectsCreated);
    Ogre::StaticGeometry* s = scene->getSceneManager()->createStaticGeometry(name);
    
    s->setRegionDimensions(Ogre::Vector3(160.0, 100.0, 160.0));
    
    // Set the region origin so the center is at 0 world
    s->setOrigin(Ogre::Vector3::ZERO);
    
    for (Ogre::Real z = -80.0; z <= 80.0; z += 20.0)
    {
        for (Ogre::Real x = -80.0; x <= 80.0; x += 20.0)
        {
            Ogre::String entityName = name + "_ogreplane_" + Ogre::StringConverter::toString(i++);
            try {
                Ogre::Entity* entity = scene->getSceneManager()->createEntity(entityName, "plane.mesh");
                entity->setMaterialName(description.materialName);
                entity->setQueryFlags(VSC::OB::QueryMaskStaticGeometry);
                entity->setCastShadows(false);
                s->addEntity(entity, Ogre::Vector3(x,0,z));
            }
            catch (std::exception &e) {
                std::cout << "Got std::exception > " << e.what() << std::endl;
            }
            catch (...) {
                std::cout << "Got unknown exception" << std::endl;
            }
            
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
    
    Ogre::String rigidBodyName = name + "RigigPlane_" + Ogre::StringConverter::toString(mNumberOfObjectsCreated);
    OgreBulletCollisions::CollisionShape *shape = new OgreBulletCollisions::StaticPlaneCollisionShape (Ogre::Vector3(0,1,0), 0);
    OgreBulletDynamics::RigidBody *rigidPlaneBody = new OgreBulletDynamics::RigidBody(rigidBodyName, scene->getDynamicsWorld());
    rigidPlaneBody->setStaticShape(shape, description.bodyRestitution, description.bodyFriction);
    
    StaticObject::SPtr geom(new StaticObject(this->getScene(), s, rigidPlaneBody));
    Scene::Element::SPtr element = boost::static_pointer_cast<Scene::Element>(geom);
    
    scene->registerElement(element, description.name, mNumberOfObjectsCreated);
    
    mNumberOfObjectsCreated++;

    return geom;
}

