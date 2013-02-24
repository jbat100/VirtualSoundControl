
#include "VSCMIDIInputManager.h"

#include "VSCMIDIInput.h"
#include "VSCException.h"

#include <boost/assert.hpp>
#include <boost/thread.hpp>
#include <boost/foreach.hpp>

boost::once_flag inputManagerSingletonInitilizedFlag = BOOST_ONCE_INIT;
static VSC::MIDI::InputManager::SPtr singletonInputManager = VSC::MIDI::InputManager::SPtr();


namespace VSC {
    namespace MIDI {
        void InitialiseSingletonInputManager();
    }
}

void VSC::MIDI::InitialiseSingletonInputManager()
{
    singletonInputManager = InputManager::SPtr(new InputManager);
}

VSC::MIDI::InputManager::SPtr VSC::MIDI::InputManager::singletonManager(void)
{
    boost::call_once(&InitialiseSingletonInputManager, inputManagerSingletonInitilizedFlag);
    BOOST_ASSERT(singletonInputManager);
    return singletonInputManager;
}

VSC::MIDI::InputManager::InputManager()
{
    mPortManager = PortManager::SPtr(new PortManager);
}

VSC::MIDI::InputManager::~InputManager()
{
    
}

bool VSC::MIDI::InputManager::checkListener(VSC::Listener::SPtr listener)
{
    InputManager::Listener::SPtr inputManagerListener = boost::dynamic_pointer_cast<InputManager::Listener>(listener);
    if (inputManagerListener) return true;
    
    return false;
}

void VSC::MIDI::InputManager::refreshInputs(void)
{
    boost::lock_guard<Mutex> lock(mMutex);
    
    BOOST_ASSERT(mPortManager);
    
    if (mPortManager)
    {
        mPortManager->refreshInputPorts();
        InputPorts ports = mPortManager->getInputPorts();
        BOOST_FOREACH(const InputPort& inputPort, ports)
        {
            Input::SPtr input = this->getInputForPort(inputPort);
            if (!input)
            {
                input = Input::SPtr(new Input(inputPort));
                mInputs.push_back(input);
            }
        }
        if (mTrace) std::cout << "Refreshed inputs (got " << mInputs.size() << " input(s))" << std::endl;
    }
    
}

const VSC::MIDI::Inputs& VSC::MIDI::InputManager::getInputs(void)
{
    boost::lock_guard<Mutex> lock(mMutex);
    return mInputs;
}

VSC::MIDI::Input::SPtr VSC::MIDI::InputManager::getFirstInput(void)
{
    boost::lock_guard<Mutex> lock(mMutex);
    Input::SPtr input = Input::SPtr();
    if (mInputs.size() > 0) input = *(mInputs.begin());
    return input;
}

VSC::MIDI::Input::SPtr VSC::MIDI::InputManager::getFirstOpenedInput(void)
{
    boost::lock_guard<Mutex> lock(mMutex);
    BOOST_FOREACH(Input::SPtr input, mInputs)
    {
        BOOST_ASSERT(input);
        if (input && input->getState() == Input::StateOpened) return input;
    }
    return Input::SPtr();
}

VSC::MIDI::Input::SPtr VSC::MIDI::InputManager::getInputWithDescription(const std::string& description)
{
    boost::lock_guard<Mutex> lock(mMutex);
    BOOST_FOREACH(Input::SPtr input, mInputs)
    {
        if (input->getDescription().compare(description) == 0) return input;
    }
    
    return Input::SPtr();
}

VSC::MIDI::Input::SPtr VSC::MIDI::InputManager::getInputForPort(const InputPort& port)
{
    boost::lock_guard<Mutex> lock(mMutex);
    BOOST_FOREACH(Input::SPtr input, mInputs)
    {
        if (input->getInputPort() == port) return input;
    }
    return Input::SPtr();
}



