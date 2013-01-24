#include "VSCMIDIOutput.h"
#include "VSCException.h"

#include "RtError.h"

#include <iostream>

VSC::MIDI::Output::Output(const OutputPort& outputPort) : mState(StateClosed) {
    
    if (outputPort == OutputPortVoid) throw VSCMIDIException("Expected non OutputPortVoid");
    
    this->createRtMidiOut();
    
    this->setOutputPort(outputPort); // will throw if does not succeed
    
    mValidControlNumbers.push_back(ControlBankSelect);
    mValidControlNumbers.push_back(ControlModulationWheel);
    mValidControlNumbers.push_back(ControlBreath);
    mValidControlNumbers.push_back(ControlFootController);
    mValidControlNumbers.push_back(ControlChannelVolume);
    mValidControlNumbers.push_back(ControlBalance);
    mValidControlNumbers.push_back(ControlUndefined1);
    mValidControlNumbers.push_back(ControlPan);
    mValidControlNumbers.push_back(ControlExpressionController);
    mValidControlNumbers.push_back(ControlEffectControl1);
    mValidControlNumbers.push_back(ControlEffectControl2);
    mValidControlNumbers.push_back(ControlUndefined2);
    mValidControlNumbers.push_back(ControlUndefined3);
    mValidControlNumbers.push_back(ControlGeneralPurposeController1);
    mValidControlNumbers.push_back(ControlGeneralPurposeController2);
    mValidControlNumbers.push_back(ControlGeneralPurposeController3);
    mValidControlNumbers.push_back(ControlGeneralPurposeController4);
    mValidControlNumbers.push_back(ControlUndefined4);
    mValidControlNumbers.push_back(ControlUndefined5);
    mValidControlNumbers.push_back(ControlUndefined6);
    mValidControlNumbers.push_back(ControlUndefined7);
    mValidControlNumbers.push_back(ControlUndefined8);
    mValidControlNumbers.push_back(ControlUndefined9);
    mValidControlNumbers.push_back(ControlUndefined10);
    mValidControlNumbers.push_back(ControlUndefined11);
    mValidControlNumbers.push_back(ControlUndefined12);
    mValidControlNumbers.push_back(ControlUndefined13);
    mValidControlNumbers.push_back(ControlUndefined14);
    mValidControlNumbers.push_back(ControlUndefined15);
    
}

VSC::MIDI::Output::~Output()
{
    close();
    
    mMIDIOut = RtMidiOutPtr();
    
    mOutputPort = OutputPortVoid;
}

std::string VSC::MIDI::Output::getDescription(void)
{
    BOOST_ASSERT(mOutputPort != OutputPortVoid);
    
    if (mOutputPort == OutputPortVoid) return "Invalid (void) MIDI Output";
    
    std::stringstream stream;
    
    stream << mOutputPort.number << " - " << mOutputPort.name << (mOutputPort.isVirtual ? " (Virtual)" : "");
    
    return stream.str();
}

void VSC::MIDI::Output::createRtMidiOut(void) {
    
    boost::lock_guard<boost::mutex> lock(mMutex);
    
    // RtMidiOut constructor
    try {
        mMIDIOut = RtMidiOutPtr(new RtMidiOut());
    }
    catch ( RtError &error ) {
        throw VSCMIDIException(error.getMessage());
    }
    
}


void VSC::MIDI::Output::open()
{
    
    boost::lock_guard<boost::mutex> lock(mMutex);
    
    BOOST_ASSERT(mOutputPort != OutputPortVoid);
    BOOST_ASSERT(mMIDIOut);
    
    if (mOutputPort != OutputPortVoid && mMIDIOut) {
        
        mMIDIOut->closePort();
        
        if (mOutputPort.isVirtual) {
            try {
                mMIDIOut->openVirtualPort(mOutputPort.name);
                mState = StateOpened;
            } catch (RtError &error) {
                error.printMessage();
                mState = StateClosed;
                throw VSCMIDIException(error.getMessage());
            }
            std::cout << "Successfully opened " << mOutputPort << std::endl;
            return;
        }
        
        else {
            try {
                if (mOutputPort.name.empty()) mMIDIOut->openPort(mOutputPort.number);
                else mMIDIOut->openPort(mOutputPort.number, mOutputPort.name);
                mState = StateOpened;
            } catch (RtError &error) {
                error.printMessage();
                mState = StateClosed;
                throw VSCMIDIException(error.getMessage());
            }
            std::cout << "Successfully opened " << mOutputPort << std::endl;
        }
        
    }
    
    else {
        
        if (mMIDIOut) {
            mOutputPort = OutputPortVoid;
            mMIDIOut->closePort();
            mState = StateClosed;
        }
        
        else {
            mOutputPort = OutputPortVoid;
            mState = StateClosed;
            throw VSCMIDIException("Expected non nil mMIDIOut");
        }
    }
    
}

void VSC::MIDI::Output::close()
{
    if (mMIDIOut) {
        mMIDIOut->closePort();
        mState = StateClosed;
    }
}

const VSC::MIDI::ControlNumbers& VSC::MIDI::Output::getValidControlNumbers(void)
{
    return mValidControlNumbers;
}

bool VSC::MIDI::Output::controlNumberIsValid(const ControlNumber& number)
{
    ControlNumbers::iterator it = std::find(mValidControlNumbers.begin(), mValidControlNumbers.end(), number);
    return it != mValidControlNumbers.end();
}


const VSC::MIDI::OutputPort& VSC::MIDI::Output::getOutputPort(void) const {
    return mOutputPort;
}

void VSC::MIDI::Output::setOutputPort(OutputPort const& port)
{
    mOutputPort = port;
}

bool VSC::MIDI::Output::sendMessage(MessageDescription::SPtr description)
{
    Message message = messageFromDescription(description);
    return this->sendMessage(message);
}

bool VSC::MIDI::Output::sendMessage(Message& m) {
    
    boost::lock_guard<boost::mutex> lock(mMutex);
    
    if (mMIDIOut && mOutputPort != OutputPortVoid)
    {
        std::cout << *this << " sending " << messageDescription(m) << std::endl;
        mMIDIOut->sendMessage(&m);
        return true;
    }
    
    return false;
    
}

std::ostream& VSC::MIDI::operator<<(std::ostream& output, const Output& p)
{
    output << "VSCMIDIOutput (" << p.getOutputPort() << ")";
    return output;
}
