
//
//  Created by Jonathan Thorpe on 4/21/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#include "VSCIMMIDIActions.h"
#include "VSCMIDIOutputManager.h"
#include "VSCMIDITasks.h"

#include <boost/foreach.hpp>

using namespace VSC;
using namespace VSC::IM;

bool actionIsMIDIAction(Action::SPtr action)
{
    if (boost::dynamic_pointer_cast<MIDIAction>(action)) return true;
    
    return false;
}

bool actionIsMIDIControlAction(Action::SPtr action)
{
    if (boost::dynamic_pointer_cast<MIDIControlAction>(action)) return true;
    
    return false;
}


MIDIAction::MIDIAction() :
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

MIDIControlAction::MIDIControlAction() : mControlNumber(MIDI::ControlBreath)
{
    
}

MIDINoteOnAction::MIDINoteOnAction()
{
    this->addRequiredMappingTarget(TargetPitch);
    this->addRequiredMappingTarget(TargetVelocityOn);
}

void MIDINoteOnAction::createDefaultMappings()
{
    const Targets& targets = this->getRequiredMappingTargets();
    
    BOOST_FOREACH(const Target& target, targets)
    {
        Mapping::SPtr mapping(new Mapping);
        mapping->setOffset(64.0);
        this->setMappingForTarget(mapping, target);
    }
}


const Tasks MIDINoteOnAction::generateTasksWithValueMap(Action::ValueMap& valueMap)
{
    Tasks tasks;
    
    MIDI::MIDISendMessageTask::Payload::SPtr payload(new MIDI::MIDISendMessageTask::Payload);
    BOOST_ASSERT(payload->messageDescription);
    payload->messageDescription->type = MIDI::MessageTypeNoteOn;
    payload->messageDescription->parameterMap[MIDI::MessageParameterKeyChannel] = (unsigned char) this->getChannel();
    payload->messageDescription->parameterMap[MIDI::MessageParameterKeyPitch] = (unsigned char) valueMap[TargetPitch];
    payload->messageDescription->parameterMap[MIDI::MessageParameterKeyVelocity] = (unsigned char) valueMap[TargetVelocityOn];
    payload->midiOutput = this->getMIDIOutput();
    MIDI::MIDISendMessageTask::SPtr task(new MIDI::MIDISendMessageTask(boost::dynamic_pointer_cast<Task::Payload>(payload)));
    tasks.push_back(task);
    
    return tasks;
}

MIDINoteOffAction::MIDINoteOffAction()
{
    this->addRequiredMappingTarget(TargetPitch);
    this->addRequiredMappingTarget(TargetVelocityOff);
}

void MIDINoteOffAction::createDefaultMappings()
{
    const Targets& targets = this->getRequiredMappingTargets();
    
    BOOST_FOREACH(const Target& target, targets)
    {
        Mapping::SPtr mapping(new Mapping);
        mapping->setOffset(64.0);
        this->setMappingForTarget(mapping, target);
    }
}

const Tasks MIDINoteOffAction::generateTasksWithValueMap(Action::ValueMap& valueMap)
{
    Tasks tasks;
    
    MIDI::MIDISendMessageTask::Payload::SPtr payload(new MIDI::MIDISendMessageTask::Payload);
    BOOST_ASSERT(payload->messageDescription);
    payload->messageDescription->type = MIDI::MessageTypeNoteOff;
    payload->messageDescription->parameterMap[MIDI::MessageParameterKeyChannel] = (unsigned char)this->getChannel();
    payload->messageDescription->parameterMap[MIDI::MessageParameterKeyPitch] = (unsigned char) valueMap[TargetPitch];
    payload->messageDescription->parameterMap[MIDI::MessageParameterKeyVelocity] = (unsigned char) valueMap[TargetVelocityOff];
    payload->midiOutput = this->getMIDIOutput();
    MIDI::MIDISendMessageTask::SPtr task(new MIDI::MIDISendMessageTask(boost::dynamic_pointer_cast<Task::Payload>(payload)));
    tasks.push_back(task);
    
    return tasks;
}

MIDINoteOnAndOffAction::MIDINoteOnAndOffAction()
{
    this->addRequiredMappingTarget(TargetPitch);
    this->addRequiredMappingTarget(TargetVelocityOn);
    this->addRequiredMappingTarget(TargetDuration);
    this->addRequiredMappingTarget(TargetVelocityOff);
}

void MIDINoteOnAndOffAction::createDefaultMappings()
{
    const Targets& targets = this->getRequiredMappingTargets();
    
    BOOST_FOREACH(const Target& target, targets)
    {
        Mapping::SPtr mapping(new Mapping);
        mapping->setOffset(64.0);
        this->setMappingForTarget(mapping, target);
    }
}

const Tasks MIDINoteOnAndOffAction::generateTasksWithValueMap(Action::ValueMap& valueMap)
{
    Tasks tasks;
    
    MIDI::MIDISendMessageTask::Payload::SPtr onPayload(new MIDI::MIDISendMessageTask::Payload);
    BOOST_ASSERT(onPayload->messageDescription);
    onPayload->messageDescription->type = MIDI::MessageTypeNoteOn;
    onPayload->messageDescription->parameterMap[MIDI::MessageParameterKeyChannel] = (unsigned char)this->getChannel();
    onPayload->messageDescription->parameterMap[MIDI::MessageParameterKeyPitch] = (unsigned char) valueMap[TargetPitch];
    onPayload->messageDescription->parameterMap[MIDI::MessageParameterKeyVelocity] = (unsigned char) valueMap[TargetVelocityOn];
    onPayload->midiOutput = this->getMIDIOutput();
    MIDI::MIDISendMessageTask::SPtr onTask(new MIDI::MIDISendMessageTask(boost::dynamic_pointer_cast<Task::Payload>(onPayload)));
    tasks.push_back(onTask);
    
    Float duration = valueMap[TargetDuration];
    long milliseconds = (long) std::floor(duration * 1000.0);
    TimeDuration timeDuration = boost::posix_time::millisec(milliseconds);
    
    MIDI::MIDISendMessageTask::Payload::SPtr offPayload(new MIDI::MIDISendMessageTask::Payload);
    BOOST_ASSERT(offPayload->messageDescription);
    offPayload->timeOffset = timeDuration;
    offPayload->messageDescription->type = MIDI::MessageTypeNoteOff;
    offPayload->messageDescription->parameterMap[MIDI::MessageParameterKeyChannel] = (unsigned char)this->getChannel();
    offPayload->messageDescription->parameterMap[MIDI::MessageParameterKeyPitch] = (unsigned char) valueMap[TargetPitch];
    offPayload->messageDescription->parameterMap[MIDI::MessageParameterKeyVelocity] = (unsigned char) valueMap[TargetVelocityOff];
    offPayload->midiOutput = this->getMIDIOutput();
    MIDI::MIDISendMessageTask::SPtr offTask(new MIDI::MIDISendMessageTask(boost::dynamic_pointer_cast<Task::Payload>(offPayload)));
    tasks.push_back(offTask);
    
    return tasks;
}

MIDIControlChangeAction::MIDIControlChangeAction()
{
    this->addRequiredMappingTarget(TargetValue);
}

void MIDIControlChangeAction::createDefaultMappings()
{
    const Targets& targets = this->getRequiredMappingTargets();
    
    BOOST_FOREACH(const Target& target, targets)
    {
        Mapping::SPtr mapping(new Mapping);
        mapping->setOffset(64.0);
        this->setMappingForTarget(mapping, target);
    }
}

const Tasks MIDIControlChangeAction::generateTasksWithValueMap(Action::ValueMap& valueMap)
{
    Tasks tasks;
    
    MIDI::MIDISendMessageTask::Payload::SPtr payload(new MIDI::MIDISendMessageTask::Payload);
    BOOST_ASSERT(payload->messageDescription);
    payload->messageDescription->type = MIDI::MessageTypeNoteOff;
    payload->messageDescription->parameterMap[MIDI::MessageParameterKeyChannel] = (unsigned char)this->getChannel();
    payload->messageDescription->parameterMap[MIDI::MessageParameterKeyValue] = (unsigned char) valueMap[TargetValue];
    payload->midiOutput = this->getMIDIOutput();
    MIDI::MIDISendMessageTask::SPtr task(new MIDI::MIDISendMessageTask(boost::dynamic_pointer_cast<Task::Payload>(payload)));
    tasks.push_back(task);

    return tasks;
}

