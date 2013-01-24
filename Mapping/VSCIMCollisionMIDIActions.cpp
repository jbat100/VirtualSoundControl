
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


VSC::Tasks VSC::IM::CollisionMIDINoteOnAction::generateTasksForCollision(OB::Element::SPtr element, OB::Collision::SPtr collision)
{
    bool gotMappings = this->checkExpectedMappingTargets();
    BOOST_ASSERT(gotMappings);
    
    CollisionMapping::SPtr pitchMapping = this->getMappingForTarget(TargetPitch);
    CollisionMapping::SPtr velocityMapping = this->getMappingForTarget(TargetVelocityOn);
    
    BOOST_ASSERT(pitchMapping);
    BOOST_ASSERT(velocityMapping);
    
    Tasks tasks;
    
    if (pitchMapping && velocityMapping)
    {
        MIDI::MIDISendMessageTask::Payload::SPtr payload(new MIDI::MIDISendMessageTask::Payload);
        BOOST_ASSERT(payload->messageDescription);
        payload->messageDescription->type = MIDI::MessageTypeNoteOn;
        payload->messageDescription->parameterMap[MIDI::MessageParameterKeyChannel] = (unsigned char)this->getChannel();
        payload->messageDescription->parameterMap[MIDI::MessageParameterKeyPitch] = (unsigned char) pitchMapping->mappedValue(element, collision);
        payload->messageDescription->parameterMap[MIDI::MessageParameterKeyVelocity] = (unsigned char) velocityMapping->mappedValue(element, collision);
        payload->midiOutput = this->getMIDIOutput();
        MIDI::MIDISendMessageTask::SPtr task(new MIDI::MIDISendMessageTask(boost::static_pointer_cast<Task::Payload>(payload)));
        tasks.push_back(task);
    }
    else
    {
        BOOST_ASSERT_MSG(false, "Could not create CollisionMIDINoteOnAction task");
    }
    
    return tasks;
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

VSC::Tasks VSC::IM::CollisionMIDINoteOffAction::generateTasksForCollision(OB::Element::SPtr element, OB::Collision::SPtr collision)
{
    bool gotMappings = this->checkExpectedMappingTargets();
    BOOST_ASSERT(gotMappings);
    
    CollisionMapping::SPtr pitchMapping = this->getMappingForTarget(TargetPitch);
    CollisionMapping::SPtr velocityMapping = this->getMappingForTarget(TargetVelocityOff);
    
    BOOST_ASSERT(pitchMapping);
    BOOST_ASSERT(velocityMapping);
    
    Tasks tasks;
    
    if (pitchMapping && velocityMapping)
    {
        MIDI::MIDISendMessageTask::Payload::SPtr payload(new MIDI::MIDISendMessageTask::Payload);
        BOOST_ASSERT(payload->messageDescription);
        payload->messageDescription->type = MIDI::MessageTypeNoteOff;
        payload->messageDescription->parameterMap[MIDI::MessageParameterKeyChannel] = (unsigned char)this->getChannel();
        payload->messageDescription->parameterMap[MIDI::MessageParameterKeyPitch] = (unsigned char) pitchMapping->mappedValue(element, collision);
        payload->messageDescription->parameterMap[MIDI::MessageParameterKeyVelocity] = (unsigned char) velocityMapping->mappedValue(element, collision);
        payload->midiOutput = this->getMIDIOutput();
        MIDI::MIDISendMessageTask::SPtr task(new MIDI::MIDISendMessageTask(boost::static_pointer_cast<Task::Payload>(payload)));
        tasks.push_back(task);
    }
    else
    {
        BOOST_ASSERT_MSG(false, "Could not create CollisionMIDINoteOnAction task");
    }
    
    return tasks;
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

VSC::Tasks VSC::IM::CollisionMIDINoteOnAndOffAction::generateTasksForCollision(OB::Element::SPtr element, OB::Collision::SPtr collision)
{
    bool gotMappings = this->checkExpectedMappingTargets();
    BOOST_ASSERT(gotMappings);
    
    CollisionMapping::SPtr pitchMapping = this->getMappingForTarget(TargetPitch);
    CollisionMapping::SPtr velocityOnMapping = this->getMappingForTarget(TargetVelocityOn);
    CollisionMapping::SPtr durationMapping = this->getMappingForTarget(TargetDuration);
    CollisionMapping::SPtr velocityOffMapping = this->getMappingForTarget(TargetVelocityOff);
    
    Tasks tasks;
    
    if (pitchMapping && velocityOnMapping && durationMapping && velocityOffMapping)
    {
        MIDI::MIDISendMessageTask::Payload::SPtr onPayload(new MIDI::MIDISendMessageTask::Payload);
        BOOST_ASSERT(onPayload->messageDescription);
        onPayload->messageDescription->type = MIDI::MessageTypeNoteOn;
        onPayload->messageDescription->parameterMap[MIDI::MessageParameterKeyChannel] = (unsigned char)this->getChannel();
        onPayload->messageDescription->parameterMap[MIDI::MessageParameterKeyPitch] = (unsigned char) pitchMapping->mappedValue(element, collision);
        payload->messageDescription->parameterMap[MIDI::MessageParameterKeyVelocity] = (unsigned char) velocityOnMapping->mappedValue(element, collision);
        onPayload->midiOutput = this->getMIDIOutput();
        MIDI::MIDISendMessageTask::SPtr onTask(new MIDI::MIDISendMessageTask(boost::static_pointer_cast<Task::Payload>(onPayload)));
        tasks.push_back(onTask);
        
        Float duration = durationMapping->mappedValue(element, collision);
        long milliseconds = (long) std::floor(duration * 1000.0);
        TimeDuration timeDuration = boost::posix_time::millisec(milliseconds);
        
        MIDI::MIDISendMessageTask::Payload::SPtr offPayload(new MIDI::MIDISendMessageTask::Payload);
        BOOST_ASSERT(offPayload->messageDescription);
        offPayload->timeOffset = timeDuration;
        offPayload->messageDescription->type = MIDI::MessageTypeNoteOff;
        offPayload->messageDescription->parameterMap[MIDI::MessageParameterKeyChannel] = (unsigned char)this->getChannel();
        offPayload->messageDescription->parameterMap[MIDI::MessageParameterKeyPitch] = (unsigned char) pitchMapping->mappedValue(element, collision);
        offPayload->messageDescription->parameterMap[MIDI::MessageParameterKeyVelocity] = (unsigned char) velocityOffMapping->mappedValue(element, collision);
        offPayload->midiOutput = this->getMIDIOutput();
        MIDI::MIDISendMessageTask::SPtr offTask(new MIDI::MIDISendMessageTask(boost::static_pointer_cast<Task::Payload>(offPayload)));
        tasks.push_back(offTask);
    }
    else
    {
        BOOST_ASSERT_MSG(false, "Could not create CollisionMIDINoteOnAction task");
    }
    
    return tasks;
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

VSC::Tasks VSC::IM::CollisionMIDIControlChangeAction::generateTasksForCollision(OB::Element::SPtr element, OB::Collision::SPtr collision)
{
    bool gotMappings = this->checkExpectedMappingTargets();
    BOOST_ASSERT(gotMappings);
    
    CollisionMapping::SPtr valueMapping = this->getMappingForTarget(TargetControlValue);
    
    Tasks tasks;
    
    if (valueMapping)
    {
        MIDI::MIDISendMessageTask::Payload::SPtr payload(new MIDI::MIDISendMessageTask::Payload);
        BOOST_ASSERT(payload->messageDescription);
        payload->messageDescription->type = MIDI::MessageTypeNoteOff;
        payload->messageDescription->parameterMap[MIDI::MessageParameterKeyChannel] = (unsigned char)this->getChannel();
        payload->messageDescription->parameterMap[MIDI::MessageParameterKeyValue] = (unsigned char) valueMapping->mappedValue(element, collision);
        payload->midiOutput = this->getMIDIOutput();
        MIDI::MIDISendMessageTask::SPtr task(new MIDI::MIDISendMessageTask(boost::static_pointer_cast<Task::Payload>(payload)));
        tasks.push_back(task);
    }
    else
    {
        BOOST_ASSERT_MSG(false, "Could not create CollisionMIDINoteOnAction task");
    }
    
    return tasks;
}

