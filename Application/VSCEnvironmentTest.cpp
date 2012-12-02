
#include "VSCEnvironmentTest.h"
#include "VSCEnvironment.h"

#include "VSCOB.h"
#include "VSCOBScene.h"
#include "VSCOBBasicSceneElementFactory.h"

#include "VSCIMCollisionMapper.h"
#include "VSCIMEvent.h"
#include "VSCIMDelay.h"
#include "VSCIMCollisionEventChain.h"
#include "VSCIMCollisionMIDIActions.h"

void VSC::EnvironmentTest::setupTestForEnvironment(Environment::SPtr environment)
{
    this->internalSetupForEnvironment(environment);
}


void VSC::EnvironmentTest::internalSetupForEnvironment(Environment::SPtr environment)
{
    BOOST_ASSERT(environment);
    if (!environment) return;
    
    OB::Scene::SPtr scene = environment->getOBScene();
    BOOST_ASSERT(scene);
    if (!scene) return;
    
    /*
     *  Create 4 elements...
     */
    
    OB::Scene::ElementFactory::SPtr factory = scene->getElementFactory();
    OB::BasicSceneElementFactory::SPtr elementFactory = boost::dynamic_pointer_cast<OB::BasicSceneElementFactory>(factory);
    OB::DynamicObject::SPtr element1 = OB::DynamicObject::SPtr();
    OB::DynamicObject::SPtr element2 = OB::DynamicObject::SPtr();
    OB::DynamicObject::SPtr element3 = OB::DynamicObject::SPtr();
    OB::DynamicObject::SPtr element4 = OB::DynamicObject::SPtr();
    
    BOOST_ASSERT(elementFactory);
    if (elementFactory)
    {
        OB::DynamicObject::FactoryDescription description1;
        description1.name = "Test Cube 1";
        description1.size = Ogre::Vector3(1.0, 1.0, 1.0);
        description1.position = Ogre::Vector3(-5.0, 10.0, -5.0);
        element1 = elementFactory->addPrimitive(OB::PrimitiveCube, description1);
        BOOST_ASSERT(element1);
        
        OB::DynamicObject::FactoryDescription description2;
        description2.name = "Test Cube 2";
        description2.position = Ogre::Vector3(-5.0, 10.0, 5.0);
        description2.size = Ogre::Vector3(1.0, 1.0, 1.0);
        element2 = elementFactory->addPrimitive(OB::PrimitiveCube, description2);
        BOOST_ASSERT(element2);
        
        OB::DynamicObject::FactoryDescription description3;
        description3.name = "Test Cube 3";
        description3.position = Ogre::Vector3(5.0, 10.0, -5.0);
        description3.size = Ogre::Vector3(1.0, 1.0, 1.0);
        element3 = elementFactory->addPrimitive(OB::PrimitiveCube, description3);
        BOOST_ASSERT(element3);
        
        OB::DynamicObject::FactoryDescription description4;
        description4.name = "Test Cube 4";
        description4.position = Ogre::Vector3(5.0, 10.0, 5.0);
        description4.size = Ogre::Vector3(1.0, 1.0, 1.0);
        element4 = elementFactory->addPrimitive(OB::PrimitiveCube, description4);
        BOOST_ASSERT(element4);
    }
    
    /*
     *  Get collision mapper and 
     */
    
    IM::CollisionMapper::SPtr collisionMapper = environment->getIMCollisionMapper();
    BOOST_ASSERT(collisionMapper);
    if (collisionMapper)
    {
        IM::CollisionEventChain::SPtr chain = collisionMapper->getEventChainForCollisionStarted(element1);
        
        IM::Event::SPtr event;
        IM::CollisionMIDINoteOnAction::SPtr noteOnAction;
        IM::CollisionMIDINoteOnAction::SPtr noteOffAction;
        
        noteOnAction = IM::CollisionMIDINoteOnAction::SPtr(new IM::CollisionMIDINoteOnAction);
        noteOnAction->createDefaultMappings();
        
        BOOST_ASSERT(noteOnAction);
        BOOST_ASSERT(noteOnAction->getMappingForTarget(IM::TargetPitch));
        if (noteOnAction->getMappingForTarget(IM::TargetPitch))
            noteOnAction->getMappingForTarget(IM::TargetPitch)->setOffset(64.0);
        
        noteOnAction->getMappingForTarget(IM::TargetVelocityOn)->setOffset(64.0);
        chain->appendEvent(boost::static_pointer_cast<IM::Event>(noteOnAction));
        
        event = IM::Event::SPtr(new IM::Delay(1.0));
        chain->appendEvent(event);
        
        noteOffAction = IM::CollisionMIDINoteOffAction::SPtr(new IM::CollisionMIDINoteOffAction);
        noteOffAction->createDefaultMappings();
        
        noteOffAction->getMappingForTarget(IM::TargetPitch)->setOffset(64.0);
        noteOffAction->getMappingForTarget(IM::TargetVelocityOff)->setOffset(64.0);
        chain->appendEvent(boost::static_pointer_cast<IM::Event>(noteOffAction));
    }
    
}

