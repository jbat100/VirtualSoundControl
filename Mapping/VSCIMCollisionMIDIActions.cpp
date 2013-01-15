
//
//  Created by Jonathan Thorpe on 4/21/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#include "VSCIMCollisionMIDIActions.h"
#include "VSCMIDIOutputManager.h"
#include "VSCMIDITasks.h"

#include "VSCOBElement.h"
#include "VSCOBCollision.h"

#include <boost/foreach.hpp>

bool VSC::IM::collisionActionIsMIDI(CollisionAction::SPtr collisionAction)
{
    if (boost::dynamic_pointer_cast<CollisionMIDIAction>(collisionAction)) return true;
    
    return false;
}

bool VSC::IM::collisionActionIsMIDIControl(CollisionAction::SPtr collisionAction)
{
    if (boost::dynamic_pointer_cast<CollisionMIDIControlAction>(collisionAction)) return true;
    
    return false;
}


VSC::IM::CollisionMIDIAction::CollisionMIDIAction() :
mChannel(1)
{
    this->setTaskQueue(MIDI::SingletonMIDITaskQueue());
    
    MIDI::OutputManager::SPtr outputManager = MIDI::OutputManager::singletonManager();
    BOOST_ASSERT(outputManager);
    if (outputManager)
    {
        // Get the first openend MIDI output...
        mMIDIOutput = outputManager->getFirstOpenedOutput();
    }
}

VSC::IM::CollisionMIDIControlAction::CollisionMIDIControlAction() : mControlNumber(MIDI::ControlBreath)
{
    
}

VSC::IM::CollisionMIDINoteOnAction::CollisionMIDINoteOnAction()
{
    this->addExpectedMappingTarget(TargetPitch);
    this->addExpectedMappingTarget(TargetVelocityOn);
}

void VSC::IM::CollisionMIDINoteOnAction::createDefaultMappings()
{
    const Targets& targets = this->getExpectedMappingTargets();
    
    BOOST_FOREACH(const Target& target, targets)
    {
        CollisionMapping::SPtr mapping(new CollisionConstantMapping);
        mapping->setOffset(64.0);
        this->setMappingForTarget(target, mapping);
    }
}

VSC::Task::SPtr VSC::IM::CollisionMIDINoteOnAction::createTaskForCollision(OB::Element::SPtr element,
                                                                           OB::Collision::SPtr collision)
{
    bool gotMappings = this->checkExpectedMappingTargets();
    BOOST_ASSERT(gotMappings);
    
    CollisionMapping::SPtr pitchMapping = this->getMappingForTarget(TargetPitch);
    CollisionMapping::SPtr velocityMapping = this->getMappingForTarget(TargetVelocityOn);
    
    BOOST_ASSERT(pitchMapping);
    BOOST_ASSERT(velocityMapping);
    
    if (pitchMapping && velocityMapping)
    {
        MIDI::MIDINoteOnTask::Payload::SPtr payload = MIDI::MIDINoteOnTask::Payload::SPtr(new MIDI::MIDINoteOnTask::Payload);
        
        payload->channel    = this->getChannel();
        payload->pitch      = (unsigned int) pitchMapping->mappedValue(element, collision);
        payload->velocity   = (unsigned int) velocityMapping->mappedValue(element, collision);
        payload->midiOutput = this->getMIDIOutput();
        
        return Task::SPtr(new MIDI::MIDINoteOnTask(payload));
    }
    
    BOOST_ASSERT_MSG(false, "Could not create CollisionMIDINoteOnAction task");
    
    return Task::SPtr();
}

VSC::IM::CollisionMIDINoteOffAction::CollisionMIDINoteOffAction()
{
    this->addExpectedMappingTarget(TargetPitch);
    this->addExpectedMappingTarget(TargetVelocityOff);
}

void VSC::IM::CollisionMIDINoteOffAction::createDefaultMappings()
{
    const Targets& targets = this->getExpectedMappingTargets();
    
    BOOST_FOREACH(const Target& target, targets)
    {
        CollisionMapping::SPtr mapping(new CollisionConstantMapping);
        mapping->setOffset(64.0);
        this->setMappingForTarget(target, mapping);
    }
}

VSC::Task::SPtr VSC::IM::CollisionMIDINoteOffAction::createTaskForCollision(OB::Element::SPtr element,
                                                                            OB::Collision::SPtr collision)
{
    bool gotMappings = this->checkExpectedMappingTargets();
    BOOST_ASSERT(gotMappings);
    
    CollisionMapping::SPtr pitchMapping = this->getMappingForTarget(TargetPitch);
    CollisionMapping::SPtr velocityMapping = this->getMappingForTarget(TargetVelocityOff);
    
    BOOST_ASSERT(pitchMapping);
    BOOST_ASSERT(velocityMapping);
    
    if (pitchMapping && velocityMapping)
    {
        MIDI::MIDINoteOffTask::Payload::SPtr payload = MIDI::MIDINoteOffTask::Payload::SPtr(new MIDI::MIDINoteOffTask::Payload);
        
        payload->channel    = this->getChannel();
        payload->pitch      = (unsigned int) pitchMapping->mappedValue(element, collision);
        payload->velocity   = (unsigned int) velocityMapping->mappedValue(element, collision);
        payload->midiOutput = this->getMIDIOutput();
        
        return Task::SPtr(new MIDI::MIDINoteOffTask(payload));
    }
    
    BOOST_ASSERT_MSG(false, "Could not create CollisionMIDINoteOnAction task");
    
    return Task::SPtr();
}

VSC::IM::CollisionMIDINoteOnAndOffAction::CollisionMIDINoteOnAndOffAction()
{
    this->addExpectedMappingTarget(TargetPitch);
    this->addExpectedMappingTarget(TargetVelocityOn);
    this->addExpectedMappingTarget(TargetDuration);
    this->addExpectedMappingTarget(TargetVelocityOff);
}

void VSC::IM::CollisionMIDINoteOnAndOffAction::createDefaultMappings()
{
    const Targets& targets = this->getExpectedMappingTargets();
    
    BOOST_FOREACH(const Target& target, targets)
    {
        CollisionMapping::SPtr mapping(new CollisionConstantMapping);
        mapping->setOffset(64.0);
        this->setMappingForTarget(target, mapping);
    }
}

VSC::Task::SPtr VSC::IM::CollisionMIDINoteOnAndOffAction::createTaskForCollision(OB::Element::SPtr element,
                                                                                 OB::Collision::SPtr collision)
{
    bool gotMappings = this->checkExpectedMappingTargets();
    BOOST_ASSERT(gotMappings);
    
    CollisionMapping::SPtr pitchMapping = this->getMappingForTarget(TargetPitch);
    CollisionMapping::SPtr velocityOnMapping = this->getMappingForTarget(TargetVelocityOn);
    CollisionMapping::SPtr durationMapping = this->getMappingForTarget(TargetDuration);
    CollisionMapping::SPtr velocityOffMapping = this->getMappingForTarget(TargetVelocityOff);
    
    if (pitchMapping && velocityOnMapping && durationMapping && velocityOffMapping)
    {
        MIDI::MIDINoteOnAndOffTask::Payload::SPtr payload = MIDI::MIDINoteOnAndOffTask::Payload::SPtr(new MIDI::MIDINoteOnAndOffTask::Payload);
        
        payload->channel        = this->getChannel();
        payload->pitch          = (unsigned int) pitchMapping->mappedValue(element, collision);
        payload->onVelocity     = (unsigned int) velocityOnMapping->mappedValue(element, collision);
        
        Float duration          = durationMapping->mappedValue(element, collision);
        long milliseconds       = (long) std::floor(duration * 1000.0);
        payload->duration       = boost::posix_time::millisec(milliseconds);
        
        payload->midiOutput     = this->getMIDIOutput();
        
        return Task::SPtr(new MIDI::MIDINoteOnAndOffTask(payload));
    }
    
    return Task::SPtr();
}

VSC::IM::CollisionMIDIControlChangeAction::CollisionMIDIControlChangeAction()
{
    this->addExpectedMappingTarget(TargetControlValue);
}

void VSC::IM::CollisionMIDIControlChangeAction::createDefaultMappings()
{
    const Targets& targets = this->getExpectedMappingTargets();
    
    BOOST_FOREACH(const Target& target, targets)
    {
        CollisionMapping::SPtr mapping(new CollisionConstantMapping);
        mapping->setOffset(64.0);
        this->setMappingForTarget(target, mapping);
    }
}

VSC::Task::SPtr VSC::IM::CollisionMIDIControlChangeAction::createTaskForCollision(OB::Element::SPtr element,
                                                                                  OB::Collision::SPtr collision)
{
    bool gotMappings = this->checkExpectedMappingTargets();
    BOOST_ASSERT(gotMappings);
    
    CollisionMapping::SPtr valueMapping = this->getMappingForTarget(TargetControlValue);
    
    if (valueMapping)
    {
        MIDI::MIDIControlChangeTask::Payload::SPtr payload = MIDI::MIDIControlChangeTask::Payload::SPtr(new MIDI::MIDIControlChangeTask::Payload);
        
        payload->channel = this->getChannel();
        payload->controlNumber = this->getControlNumber();
        payload->value   = (unsigned int) valueMapping->mappedValue(element, collision);
        payload->midiOutput = this->getMIDIOutput();
        
        return Task::SPtr(new MIDI::MIDIControlChangeTask(payload));
    }
    
    return Task::SPtr();
}

