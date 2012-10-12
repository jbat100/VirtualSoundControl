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

VSC::MIDI::Output::SPtr VSC::MIDI::OutputManager::getFirstOutput(void)
{
    Output::SPtr output = Output::SPtr();
    
    BOOST_FOREACH(OutputMap::value_type mapValue, mOutputMap)
    {
        const OutputPort& port = mapValue.first;
        if (!output || port.number < output->getOutputPort().number) {
            output = mapValue.second;
        }
    }
    
    return output;
}

VSC::MIDI::Output::SPtr VSC::MIDI::OutputManager::getOutputForPort(const OutputPort& port)
{
    OutputMap::iterator it = mOutputMap.find(port);
    if (it != mOutputMap.end())
    {
        return (*it).second;
    }
    else
    {
        Output::SPtr output = Output::SPtr(new Output(port));
        if (output)
        {
            mOutputMap[port] = output;
        }
        return output;
    }
    return Output::SPtr();
}



