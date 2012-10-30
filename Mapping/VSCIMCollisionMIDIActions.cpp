
//
//  Created by Jonathan Thorpe on 4/21/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#include "VSCIMCollisionMIDIActions.h"
#include "VSCMIDITasks.h"


VSC::IM::CollisionMIDIAction::CollisionMIDIAction() : 
mChannel(0)
{
    this->setTaskQueue(MIDI::SingletonMIDITaskQueue());
}

VSC::Task::SPtr VSC::IM::CollisionMIDINoteOnAction::createTaskForCollision(OB::Scene::Element::SPtr element,
                                                                           OB::Scene::Collision::SPtr collision)
{
    CollisionMapping::SPtr pitchMapping = this->getMappingForTarget(Pitch);
    BOOST_ASSERT(pitchMapping);
    CollisionMapping::SPtr velocityMapping = this->getMappingForTarget(VelocityOn);
    BOOST_ASSERT(velocityMapping);
    
    if (pitchMapping && velocityMapping)
    {
        MIDI::MIDINoteOnTask::Payload::SPtr payload = MIDI::MIDINoteOnTask::Payload::SPtr(new MIDI::MIDINoteOnTask::Payload);
        
        payload->channel    = this->getChannel();
        payload->pitch      = (unsigned int) pitchMapping->mappedValue(element, collision);
        payload->velocity   = (unsigned int) velocityMapping->mappedValue(element, collision);
        payload->midiOutput = this->getMIDIOuput();
        
        return Task::SPtr(new MIDI::MIDINoteOnTask(payload));
    }
    
    return Task::SPtr();
}

VSC::Task::SPtr VSC::IM::CollisionMIDINoteOffAction::createTaskForCollision(OB::Scene::Element::SPtr element,
                                                                            OB::Scene::Collision::SPtr collision)
{
    CollisionMapping::SPtr pitchMapping = this->getMappingForTarget(Pitch);
    BOOST_ASSERT(pitchMapping);
    CollisionMapping::SPtr velocityMapping = this->getMappingForTarget(VelocityOn);
    BOOST_ASSERT(velocityMapping);
    
    if (pitchMapping && velocityMapping)
    {
        MIDI::MIDINoteOffTask::Payload::SPtr payload = MIDI::MIDINoteOffTask::Payload::SPtr(new MIDI::MIDINoteOffTask::Payload);
        
        payload->channel    = this->getChannel();
        payload->pitch      = (unsigned int) pitchMapping->mappedValue(element, collision);
        payload->velocity   = (unsigned int) velocityMapping->mappedValue(element, collision);
        payload->midiOutput = this->getMIDIOuput();
        
        return Task::SPtr(new MIDI::MIDINoteOffTask(payload));
    }
    
    return Task::SPtr();
}

VSC::Task::SPtr VSC::IM::CollisionMIDINoteOnAndOffAction::createTaskForCollision(OB::Scene::Element::SPtr element,
                                                                                 OB::Scene::Collision::SPtr collision)
{
    CollisionMapping::SPtr pitchMapping = this->getMappingForTarget(Pitch);
    BOOST_ASSERT(pitchMapping);
    CollisionMapping::SPtr velocityOnMapping = this->getMappingForTarget(VelocityOn);
    BOOST_ASSERT(velocityOnMapping);
    CollisionMapping::SPtr durationMapping = this->getMappingForTarget(Duration);
    BOOST_ASSERT(durationMapping);
    CollisionMapping::SPtr velocityOffMapping = this->getMappingForTarget(VelocityOff);
    BOOST_ASSERT(velocityOffMapping);
    
    if (pitchMapping && velocityOnMapping && durationMapping && velocityOffMapping)
    {
        MIDI::MIDINoteOnAndOffTask::Payload::SPtr payload = MIDI::MIDINoteOnAndOffTask::Payload::SPtr(new MIDI::MIDINoteOnAndOffTask::Payload);
        
        payload->channel        = this->getChannel();
        payload->pitch          = (unsigned int) pitchMapping->mappedValue(element, collision);
        payload->onVelocity     = (unsigned int) velocityOnMapping->mappedValue(element, collision);
        
        Float duration          = durationMapping->mappedValue(element, collision);
        long milliseconds       = (long) std::floor(duration * 1000.0);
        payload->duration       = boost::posix_time::millisec(milliseconds);
        
        payload->midiOutput     = this->getMIDIOuput();
        
        return Task::SPtr(new MIDI::MIDINoteOffTask(payload));
    }
    
    return Task::SPtr();
}

VSC::Task::SPtr VSC::IM::CollisionMIDIControlChangeAction::createTaskForCollision(OB::Scene::Element::SPtr element,
                                                                                  OB::Scene::Collision::SPtr collision)
{
    CollisionMapping::SPtr valueMapping = this->getMappingForTarget(ControlValue);
    BOOST_ASSERT(valueMapping);
    
    if (valueMapping)
    {
        MIDI::MIDIControlChangeTask::Payload::SPtr payload = MIDI::MIDIControlChangeTask::Payload::SPtr(new MIDI::MIDIControlChangeTask::Payload);
        
        payload->channel = this->getChannel();
        payload->controlNumber = this->getControlNumber();
        payload->value   = (unsigned int) valueMapping->mappedValue(element, collision);
        payload->midiOutput = this->getMIDIOuput();
        
        return Task::SPtr(new MIDI::MIDIControlChangeTask(payload));
    }
    
    return Task::SPtr();
}