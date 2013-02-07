
//
//  Created by Jonathan Thorpe on 4/21/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#include "VSCIMActionImplementations.h"
#include "VSCMIDIOutputManager.h"
#include "VSCMIDITasks.h"
#include "VSCMIDI.h"

#include <boost/foreach.hpp>

using namespace VSC;
using namespace VSC::IM;


//MARK: MIDINoteOn

const Tasks Action::ImplementationMIDINoteOn::generateTasksWithTargetValueMap(Event::TargetValueMap& valueMap)
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

void Action::ImplementationMIDINoteOn::setupMappings(Action::SPtr action)
{
    BOOST_ASSERT(action);
    if (action)
    {
        const Targets& targets = action->getRequiredMappingTargets();
        BOOST_FOREACH(const Trigger& trigger, AllowedTriggers())
        {
            BOOST_FOREACH(const Target& target, targets)
            {
                Mapping::SPtr mapping = action->getMapping(trigger, target);
                BOOST_ASSERT(mapping);
                if (mapping)
                {
                    mapping->setMappingType(MappingTypeConstant);
                    mapping->setOffset(64.0);
                }
            }
        }
    }
}

//MARK: MIDINoteOff

const Tasks Action::ImplementationMIDINoteOff::generateTasksWithTargetValueMap(Event::TargetValueMap& valueMap)
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

void Action::ImplementationMIDINoteOff::setupMappings(Action::SPtr action)
{
    BOOST_ASSERT(action);
    if (action)
    {
        const Targets& targets = action->getRequiredMappingTargets();
        BOOST_FOREACH(const Trigger& trigger, AllowedTriggers())
        {
            BOOST_FOREACH(const Target& target, targets)
            {
                Mapping::SPtr mapping = action->getMapping(trigger, target);
                BOOST_ASSERT(mapping);
                if (mapping)
                {
                    mapping->setMappingType(MappingTypeConstant);
                    mapping->setOffset(64.0);
                }
            }
        }
    }
}

//MARK: MIDINoteOnAndOff

const Tasks Action::ImplementationMIDINoteOnAndOff::generateTasksWithTargetValueMap(Event::TargetValueMap& valueMap)
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

void Action::ImplementationMIDINoteOnAndOff::setupMappings(Action::SPtr action)
{
    BOOST_ASSERT(action);
    if (action)
    {
        const Targets& targets = action->getRequiredMappingTargets();
        BOOST_FOREACH(const Trigger& trigger, AllowedTriggers())
        {
            BOOST_FOREACH(const Target& target, targets)
            {
                Mapping::SPtr mapping = action->getMapping(trigger, target);
                BOOST_ASSERT(mapping);
                if (mapping)
                {
                    mapping->setMappingType(MappingTypeConstant);
                    mapping->setOffset(64.0);
                }
            }
        }
    }
}

//MARK: MIDIControlChange

const Tasks Action::ImplementationMIDIControlChange::generateTasksWithTargetValueMap(Event::TargetValueMap& valueMap)
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

void Action::ImplementationMIDIControlChange::setupMappings(Action::SPtr action)
{
    BOOST_ASSERT(action);
    if (action)
    {
        const Targets& targets = action->getRequiredMappingTargets();
        BOOST_FOREACH(const Trigger& trigger, AllowedTriggers())
        {
            BOOST_FOREACH(const Target& target, targets)
            {
                Mapping::SPtr mapping = action->getMapping(trigger, target);
                BOOST_ASSERT(mapping);
                if (mapping)
                {
                    mapping->setMappingType(MappingTypeConstant);
                    mapping->setOffset(64.0);
                }
            }
        }
    }
}


