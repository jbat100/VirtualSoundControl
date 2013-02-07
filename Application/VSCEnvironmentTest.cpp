
#include "VSCEnvironmentTest.h"
#include "VSCEnvironment.h"

#include "VSCOB.h"
#include "VSCOBScene.h"
#include "VSCOBElement.h"
#include "VSCOBElementFactory.h"
#include "VSCOBDynamicObject.h"

#include "VSCCollisionMapper.h"
#include "VSCIMEvent.h"
#include "VSCIMDelay.h"
#include "VSCIMEventChain.h"
#include "VSCIMActionImplementations.h"

using namespace VSC;
using namespace VSC::IM;
using namespace VSC::OB;

void EnvironmentTest::setupTestForEnvironment(Environment::SPtr environment)
{
    this->internalSetupForEnvironment(environment);
}


void EnvironmentTest::internalSetupForEnvironment(Environment::SPtr environment)
{
    BOOST_ASSERT(environment);
    if (!environment) return;
    
    Scene::SPtr scene = environment->getScene();
    BOOST_ASSERT(scene);
    if (!scene) return;
    
    // reset scene if it wasn't pristine...
    //scene->destroyAllElements();
    
    /*
     *  Create 4 elements...
     */
    
    ElementFactory::SPtr elementFactory = scene->getElementFactory();
    DynamicObject::SPtr element1 = DynamicObject::SPtr();
    DynamicObject::SPtr element2 = DynamicObject::SPtr();
    DynamicObject::SPtr element3 = DynamicObject::SPtr();
    DynamicObject::SPtr element4 = DynamicObject::SPtr();
    
    BOOST_ASSERT(elementFactory);
    if (elementFactory)
    {
        DynamicObject::FactoryDescription description1;
        description1.name = "Test Cube 1";
        description1.size = Ogre::Vector3(1.0, 1.0, 1.0);
        description1.position = Ogre::Vector3(0.0, 1.0, 0.0);
        element1 = elementFactory->addPrimitive(PrimitiveCube, description1);
        //element1->setImmobilized(true);
        BOOST_ASSERT(element1);
        
        DynamicObject::FactoryDescription description2;
        description2.name = "Test Cube 2";
        description2.position = Ogre::Vector3(0.0, 20.0, 0.0);
        description2.size = Ogre::Vector3(1.0, 1.0, 1.0);
        element2 = elementFactory->addPrimitive(PrimitiveCube, description2);
        BOOST_ASSERT(element2);
        
        /*
        DynamicObject::FactoryDescription description3;
        description3.name = "Test Cube 3";
        description3.position = Ogre::Vector3(5.0, 10.0, -5.0);
        description3.size = Ogre::Vector3(1.0, 1.0, 1.0);
        element3 = elementFactory->addPrimitive(PrimitiveCube, description3);
        BOOST_ASSERT(element3);
        
        DynamicObject::FactoryDescription description4;
        description4.name = "Test Cube 4";
        description4.position = Ogre::Vector3(5.0, 10.0, 5.0);
        description4.size = Ogre::Vector3(1.0, 1.0, 1.0);
        element4 = elementFactory->addPrimitive(PrimitiveCube, description4);
        BOOST_ASSERT(element4);
         */
    }
    
    /*
     *  Get collision mapper and create a new chain for element 1
     */
    
    CollisionMapper::SPtr collisionMapper = environment->getCollisionMapper();
    BOOST_ASSERT(collisionMapper);
    if (collisionMapper)
    {        
        EventChain::SPtr chain = environment->createEventChain();
        
        MIDINoteOnAction::SPtr noteOnAction = MIDINoteOnAction::SPtr(new MIDINoteOnAction);
        noteOnAction->createDefaultMappings();
        BOOST_ASSERT(noteOnAction);
        BOOST_ASSERT(noteOnAction->getMappingForTarget(TargetPitch));
        if (noteOnAction->getMappingForTarget(TargetPitch))
            noteOnAction->getMappingForTarget(TargetPitch)->setOffset(64.0);
        noteOnAction->getMappingForTarget(TargetVelocityOn)->setOffset(64.0);
        chain->appendEvent(boost::static_pointer_cast<Event>(noteOnAction));
        
        Delay::SPtr delay = Delay::SPtr(new Delay());
        delay->createDefaultMappings();
        chain->appendEvent(boost::static_pointer_cast<Event>(delay));
        
        MIDINoteOffAction::SPtr noteOffAction = MIDINoteOffAction::SPtr(new MIDINoteOffAction);
        noteOffAction->createDefaultMappings();
        noteOffAction->getMappingForTarget(TargetPitch)->setOffset(64.0);
        noteOffAction->getMappingForTarget(TargetVelocityOff)->setOffset(64.0);
        chain->appendEvent(boost::static_pointer_cast<Event>(noteOffAction));
        
        collisionMapper->addEventChainForCollisionStarted(chain, element1);
    }
    
}

