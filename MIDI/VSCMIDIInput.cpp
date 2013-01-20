
#include "VSCMIDIInput.h"
#include "VSCException.h"

#include "RtError.h"

#include <iostream>
#include <vector>

namespace VSC
{
    namespace MIDI
    {
        void InputCallback(double deltatime, std::vector<unsigned char> *message, void* userData);
        
        MessageAnalyzer::SPtr callbackMessageAnalyzer;
    }
}

VSC::MIDI::MessageAnalyzer::SPtr callbackMessageAnalyzer = VSC::MIDI::MessageAnalyzer::SPtr();

void VSC::MIDI::InputCallback(double deltatime, std::vector<unsigned char>* message, void* userData)
{
    BOOST_ASSERT(message);
    if (!message) return;
    
    BOOST_ASSERT(userData);
    Input* rawInput = dynamic_cast<Input*>((Input*)userData);
    BOOST_ASSERT(rawInput);
    if(!rawInput)
    {
        std::cerr << "ERROR: MIDI Callback UNEXPECTED USERDATA" << std::endl;
        return;
    }
    
    Input::SPtr input = rawInput->shared_from_this();
    
    const Message& midiMessage = *message;
    
    MessageType messageType = callbackMessageAnalyzer->messageTypeForMessage(midiMessage);
}

VSC::MIDI::Input::Input(const InputPort& inputPort) : mState(StateClosed) {
    
    if (inputPort == InputPortVoid) throw VSCMIDIException("Expected non InputPortVoid");
    
    this->createRtMidiIn();
    
    this->setInputPort(inputPort); // will throw if does not succeed
    
}

VSC::MIDI::Input::~Input()
{
    this->close();
    mMIDIIn = RtMidiInPtr();
    mInputPort = InputPortVoid;
}

std::string VSC::MIDI::Input::getDescription(void)
{
    BOOST_ASSERT(mInputPort != InputPortVoid);
    
    if (mInputPort == InputPortVoid) return "Invalid (void) MIDI Input";
    
    std::stringstream stream;
    
    stream << mInputPort.number << " - " << mInputPort.name << (mInputPort.isVirtual ? " (Virtual)" : "");
    
    return stream.str();
}

void VSC::MIDI::Input::createRtMidiIn(void) {
    
    boost::lock_guard<boost::mutex> lock(mMutex);
    
    // RtMidiIn constructor
    try
    {
        mMIDIIn = RtMidiInPtr(new RtMidiIn());
    }
    catch ( RtError &error )
    {
        throw VSCMIDIException(error.getMessage());
    }
    
}


void VSC::MIDI::Input::open()
{
    
    boost::lock_guard<boost::mutex> lock(mMutex);
    
    BOOST_ASSERT(mInputPort != InputPortVoid);
    BOOST_ASSERT(mMIDIIn);
    
    if (mInputPort != InputPortVoid && mMIDIIn) {
        
        mMIDIIn->closePort();
        
        if (mInputPort.isVirtual) {
            try
            {
                mMIDIIn->openVirtualPort(mInputPort.name);
                mMIDIIn->setCallback(&InputCallback, this);
                mMIDIIn->ignoreTypes(false, false, false);
                mState = StateOpened;
            }
            catch (RtError &error)
            {
                error.printMessage();
                mState = StateClosed;
                throw VSCMIDIException(error.getMessage());
            }
            std::cout << "Successfully opened " << mInputPort << std::endl;
            return;
        }
        
        else
        {
            try
            {
                if (mInputPort.name.empty()) mMIDIIn->openPort(mInputPort.number);
                else mMIDIIn->openPort(mInputPort.number, mInputPort.name);
                mMIDIIn->setCallback(&InputCallback, this);
                mMIDIIn->ignoreTypes(false, false, false);
                mState = StateOpened;
            }
            catch (RtError &error)
            {
                error.printMessage();
                mState = StateClosed;
                throw VSCMIDIException(error.getMessage());
            }
            std::cout << "Successfully opened " << mInputPort << std::endl;
        }
        
    }
    
    else {
        
        if (mMIDIIn)
        {
            mInputPort = InputPortVoid;
            mMIDIIn->closePort();
            mMIDIIn->cancelCallback();
            mState = StateClosed;
        }
        
        else
        {
            mInputPort = InputPortVoid;
            mMIDIIn->closePort();
            mMIDIIn->cancelCallback();
            mState = StateClosed;
            throw VSCMIDIException("Expected non nil mMIDIIn");
        }
    }
    
}

void VSC::MIDI::Input::close()
{
    if (mMIDIIn)
    {
        mMIDIIn->closePort();
        mMIDIIn->cancelCallback();
        mState = StateClosed;
    }
}


const VSC::MIDI::InputPort& VSC::MIDI::Input::getInputPort(void) const
{
    return mInputPort;
}

void VSC::MIDI::Input::setInputPort(InputPort const& port)
{
    mInputPort = port;
}


std::ostream& VSC::MIDI::operator<<(std::ostream& s, const Input& p)
{
    s << "VSCMIDIInput (" << p.getInputPort() << ")";
    return s;
}
