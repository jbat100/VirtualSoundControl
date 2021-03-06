/*
 *  VSCBoost.cpp
 *  EnveloppeEditor
 *
 *  Created by Jonathan Thorpe on 26/08/2011.
 *  Copyright 2011 JBAT. All rights reserved.
 *
 */

#include "VSCMIDIOutputManager.h"

#include "VSCMIDIOutput.h"
#include "VSCException.h"

#include <boost/assert.hpp>
#include <boost/thread.hpp>
#include <boost/foreach.hpp>

boost::once_flag outputManagerSingletonInitilizedFlag = BOOST_ONCE_INIT;
static VSC::MIDI::OutputManager::SPtr singletonOutputManager = VSC::MIDI::OutputManager::SPtr();


namespace VSC {
    namespace MIDI {
        void InitialiseSingletonOutputManager();
    }
}

void VSC::MIDI::InitialiseSingletonOutputManager()
{
    singletonOutputManager = OutputManager::SPtr(new OutputManager);
}

VSC::MIDI::OutputManager::SPtr VSC::MIDI::OutputManager::singletonManager(void)
{
    boost::call_once(&InitialiseSingletonOutputManager, outputManagerSingletonInitilizedFlag);
    BOOST_ASSERT(singletonOutputManager);
    return singletonOutputManager;
}

VSC::MIDI::OutputManager::OutputManager()
{
    mPortManager = PortManager::SPtr(new PortManager);
}

VSC::MIDI::OutputManager::~OutputManager()
{
    
}

bool VSC::MIDI::OutputManager::checkListener(VSC::Listener::SPtr listener)
{
    OutputManager::Listener::SPtr outputManagerListener = boost::dynamic_pointer_cast<OutputManager::Listener>(listener);
    if (outputManagerListener) return true;
    
    return false;
}

void VSC::MIDI::OutputManager::refreshOutputs(void)
{
    boost::lock_guard<Mutex> lock(mMutex);
    
    BOOST_ASSERT(mPortManager);
    
    if (mPortManager) {
        
        mPortManager->refreshOutputPorts();
        
        OutputPorts ports = mPortManager->getOutputPorts();
        
        BOOST_FOREACH(const OutputPort& outputPort, ports)
        {
            Output::SPtr output = this->getOutputForPort(outputPort);
            if (!output)
            {
                output = Output::SPtr(new Output(outputPort));
                mOutputs.push_back(output);
            }
        }
        
        if (mTrace) std::cout << "Refreshed outputs (got " << mOutputs.size() << " output(s))" << std::endl;
        
    }
    
}

const VSC::MIDI::Outputs& VSC::MIDI::OutputManager::getOutputs(void)
{
    boost::lock_guard<Mutex> lock(mMutex);
    
    return mOutputs;
}

VSC::MIDI::Output::SPtr VSC::MIDI::OutputManager::getFirstOutput(void)
{
    boost::lock_guard<Mutex> lock(mMutex);
    Output::SPtr output = Output::SPtr();
    if (mOutputs.size() > 0) output = *(mOutputs.begin());
    return output;
}

VSC::MIDI::Output::SPtr VSC::MIDI::OutputManager::getFirstOpenedOutput(void)
{
    boost::lock_guard<Mutex> lock(mMutex);
    BOOST_FOREACH(Output::SPtr output, mOutputs)
    {
        BOOST_ASSERT(output);
        if (output && output->getState() == Output::StateOpened) return output;
    }
    return Output::SPtr();
}

VSC::MIDI::Output::SPtr VSC::MIDI::OutputManager::getOutputWithDescription(const std::string& description)
{
    boost::lock_guard<Mutex> lock(mMutex);
    
    BOOST_FOREACH(Output::SPtr output, mOutputs)
    {
        if (output->getDescription().compare(description) == 0) return output;
    }
    
    return Output::SPtr();
}

VSC::MIDI::Output::SPtr VSC::MIDI::OutputManager::getOutputForPort(const OutputPort& port)
{
    boost::lock_guard<Mutex> lock(mMutex);
    
    BOOST_FOREACH(Output::SPtr output, mOutputs)
    {
        if (output->getOutputPort() == port) return output;
    }
    return Output::SPtr();
}



