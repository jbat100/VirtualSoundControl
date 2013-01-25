/*
 *  VSCSound.cpp
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 22/10/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */

#include "VSCTaskTest.h"

#include "VSCMIDITasks.h"
#include "VSCMIDIOutput.h"
#include "VSCMIDIOutputManager.h"

#include <boost/assert.hpp>
#include <boost/thread.hpp>

void VSC::TaskTest::performTestWithTaskQueue(TaskQueue::SPtr queue)
{
    this->internalPerformTestWithTaskQueue(queue);
}

void VSC::TaskTest::internalPerformTestWithTaskQueue(TaskQueue::SPtr queue)
{
    BOOST_ASSERT(queue);
    
    if (trace) std::cout << "Setting up test for queue" << std::endl;
    
    /*
     *  Make sure the queue is started
     */
    
    TaskQueue::State state = queue->getState();
    
    if (trace && state == TaskQueue::StateRunning) std::cout << "Queue should be running" << std::endl;
    if (trace && state == TaskQueue::StateStopped) std::cout << "Queue is stopped" << std::endl;
    
    if (state == TaskQueue::StateStopped)
    {
        queue->start();
        BOOST_ASSERT(queue->getState() == TaskQueue::StateRunning);
    }
    
    /*
     *  Check that we have a midi output to work with
     */
    
    MIDI::OutputManager::SPtr outputManager = MIDI::OutputManager::singletonManager();
    BOOST_ASSERT(outputManager);
    VSC::MIDI::Output::SPtr midiOutput = outputManager->getFirstOpenedOutput();
    BOOST_ASSERT(midiOutput);
    
    /*
     *  Create a midi on task and enqueue it...
     */
    
    {
        /*
         
        MIDI::MIDINoteOnTask::Payload::SPtr payload;
        payload = MIDI::MIDINoteOnTask::Payload::SPtr(new MIDI::MIDINoteOnTask::Payload);
        
        payload->channel    = 1;
        payload->pitch      = 64;
        payload->velocity   = 64;
        payload->midiOutput = midiOutput;
        
        Task::SPtr onTask = Task::SPtr(new MIDI::MIDINoteOnTask(payload));
        onTask->setExecutionStartTime(VSC::CurrentTime() + boost::posix_time::seconds(1.0));
        
        queue->enqueueTask(onTask);
         
         */
    }
    
    /*
     *  Create a midi off task and enqueue it with 1 second delay
     */
    
    {
        /*
        
        MIDI::MIDINoteOffTask::Payload::SPtr payload;
        payload = MIDI::MIDINoteOffTask::Payload::SPtr(new MIDI::MIDINoteOffTask::Payload);
        
        payload->channel    = 1;
        payload->pitch      = 64;
        payload->velocity   = 64;
        payload->midiOutput = midiOutput;
        
        Task::SPtr offTask = Task::SPtr(new MIDI::MIDINoteOffTask(payload));
        offTask->setExecutionStartTime(VSC::CurrentTime() + boost::posix_time::seconds(2.0));
        
        queue->enqueueTask(offTask);
         
         */
    }
    
}