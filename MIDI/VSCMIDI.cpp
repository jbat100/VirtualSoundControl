/*
 *  VSCBoost.cpp
 *  EnveloppeEditor
 *
 *  Created by Jonathan Thorpe on 26/08/2011.
 *  Copyright 2011 JBAT. All rights reserved.
 *
 */

#include "VSCMIDI.h"
#include "VSCException.h"
#include "VSCTaskQueue.h"

#include <boost/thread.hpp>
#include <boost/foreach.hpp>
#include <boost/assert.hpp>

#include <limits>
#include <assert.h>

#pragma mark - MIDI Ports

const VSC::MIDI::OutputPort     VSC::MIDI::OutputPortVoid   = {std::numeric_limits<unsigned int>::max(), "", false};
const VSC::MIDI::InputPort      VSC::MIDI::InputPortVoid    = {std::numeric_limits<unsigned int>::max(), "", false};


std::ostream& VSC::MIDI::operator<<(std::ostream& output, const OutputPort& p) {
    output << "VSCMIDIOutputPort {" << p.number << "; " << p.name;
    if (p.isVirtual) output << "; virtual";
    output << "}";
    return output;
}

std::ostream& VSC::MIDI::operator<<(std::ostream& output, const InputPort& p) {
    output << "VSCMIDIInputPort {" << p.number << "; " << p.name;
    if (p.isVirtual) output << "; virtual";
    output << "}";
    return output;
}

bool VSC::MIDI::OutputPort::operator!=(const OutputPort& p) const {
    return !(*this == p);
}

bool VSC::MIDI::OutputPort::operator==(const OutputPort& p) const {
    if (p.isVirtual != isVirtual || p.number != number || p.name.compare(name) != 0) return false;
    return true;
}

bool VSC::MIDI::OutputPort::operator<(const OutputPort& p) const {
    if (number != p.number) return number < p.number;
    if (isVirtual != p.isVirtual) return isVirtual;
    return p.name.compare(name) < 0;
}

bool VSC::MIDI::InputPort::operator!=(const InputPort& p) const {
    return !(*this == p);
}

bool VSC::MIDI::InputPort::operator==(const InputPort& p) const {
    if (p.isVirtual != isVirtual || p.number != number || p.name.compare(name) != 0) return false;
    return true;
}

bool VSC::MIDI::InputPort::operator<(const InputPort& p) const {
    if (number != p.number) return number < p.number;
    if (isVirtual != p.isVirtual) return isVirtual;
    return p.name.compare(name) < 0;
}

#pragma mark - MIDI Control Numbers

boost::once_flag filledControlNumberStringsMapFlag = BOOST_ONCE_INIT;

namespace VSC {
    namespace MIDI {
        void FillControlNumberStringsMap();
        typedef std::map<ControlNumber, std::string> ControlNumberStringMap;
        static ControlNumberStringMap controlNumberStringMap;
    }
}

VSC::MIDI::ControlNumberStringMap controlNumberStringMap;

void VSC::MIDI::FillControlNumberStringsMap()
{
    
    BOOST_ASSERT(controlNumberStringMap.empty());
    
    controlNumberStringMap[ControlBankSelect]                   = "Bank Select";
    controlNumberStringMap[ControlModulationWheel]              = "Modulation Wheel";
    controlNumberStringMap[ControlBreath]                       = "Breath";
    controlNumberStringMap[ControlFootController]               = "Foot Controller";
    controlNumberStringMap[ControlChannelVolume]                = "Channel Volume";
    controlNumberStringMap[ControlBalance]                      = "Balance";
    controlNumberStringMap[ControlUndefined1]                   = "Undefined 1";
    controlNumberStringMap[ControlPan]                          = "Pan";
    controlNumberStringMap[ControlExpressionController]         = "Expression Controller";
    controlNumberStringMap[ControlEffectControl1]               = "Effect Control 1";
    controlNumberStringMap[ControlEffectControl2]               = "Effect Control 2";
    controlNumberStringMap[ControlUndefined2]                   = "Undefined 2";
    controlNumberStringMap[ControlUndefined3]                   = "Undefined 3";
    controlNumberStringMap[ControlGeneralPurposeController1]    = "General Purpose Controller 1";
    controlNumberStringMap[ControlGeneralPurposeController2]    = "General Purpose Controller 2";
    controlNumberStringMap[ControlGeneralPurposeController3]    = "General Purpose Controller 3";
    controlNumberStringMap[ControlUndefined4]                   = "Undefined 4";
    controlNumberStringMap[ControlUndefined5]                   = "Undefined 5";
    controlNumberStringMap[ControlUndefined6]                   = "Undefined 6";
    controlNumberStringMap[ControlUndefined7]                   = "Undefined 7";
    controlNumberStringMap[ControlUndefined8]                   = "Undefined 8";
    controlNumberStringMap[ControlUndefined9]                   = "Undefined 9";
    controlNumberStringMap[ControlUndefined10]                  = "Undefined 10";
    controlNumberStringMap[ControlUndefined11]                  = "Undefined 11";
    controlNumberStringMap[ControlUndefined12]                  = "Undefined 12";
    controlNumberStringMap[ControlUndefined13]                  = "Undefined 13";
    controlNumberStringMap[ControlUndefined14]                  = "Undefined 14";
    controlNumberStringMap[ControlUndefined15]                  = "Undefined 15";
    
    controlNumberStringMap[ControlInvalid]                      = "Invalid MIDI Control Number";
    controlNumberStringMap[ControlNone]                         = "No MIDI Control Number";

}


std::string VSC::MIDI::controlNumberToString(ControlNumber num)
{
    boost::call_once(&FillControlNumberStringsMap, filledControlNumberStringsMapFlag);
    
    BOOST_ASSERT(!controlNumberStringMap.empty());
    std::string desc = controlNumberStringMap[num];
    //BOOST_ASSERT(!desc.empty());
    if (desc.empty()) desc = "Unknown MIDI Control Number";
    
    return desc;
}

VSC::MIDI::ControlNumber VSC::MIDI::stringToControlNumber(const std::string& desc)
{
    boost::call_once(&FillControlNumberStringsMap, filledControlNumberStringsMapFlag);
    
    BOOST_ASSERT(!controlNumberStringMap.empty());
    
    BOOST_FOREACH(const ControlNumberStringMap::value_type& mapValue, controlNumberStringMap)
    {
        if (desc.compare(mapValue.second) == 0) return mapValue.first;
    }
    
    return ControlInvalid;
}

#pragma mark - MIDI Messages

std::string VSC::MIDI::messageDescription(const Message& m) {
    
    std::stringstream o;
    
    o << "MIDI Message: ";
    
    // if note message ff
    if (m[0] >= 128 && m[0] < 144)
    {
        o << "Note OFF (channel " << (unsigned int) m[0]-127;
        o << ", pitch " << (unsigned int) m[1];
        o << ", velocity " << (unsigned int) m[2] << ")";
    }
    
    // if note message on
    else if (m[0] >= 144 && m[0] < 160)
    {
        o << "Note ON (channel " << (unsigned int) m[0]-143;
        o << ", pitch " << (unsigned int) m[1];
        o << ", velocity " << (unsigned int) m[2] << ")";
    }
    
    // if control
    else if (m[0] >= 176 && m[0] < 192)
    {
        o << "Control (channel " << (unsigned int) m[0]-175;
        o << ", control " << controlNumberToString((ControlNumber)m[1]);
        o << ", value " << (unsigned int) m[2] << ")";
    }
    
    return o.str();
    
}

#pragma mark - MIDI MessageGenerator

VSC::MIDI::MessageGenerator::MessageGenerator()
{
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

bool VSC::MIDI::MessageGenerator::controlNumberIsValid(const ControlNumber& number)
{
    ControlNumbers::iterator it = std::find(mValidControlNumbers.begin(), mValidControlNumbers.end(), number);
    return it != mValidControlNumbers.end();
}

VSC::MIDI::Message VSC::MIDI::MessageGenerator::messageForNote(unsigned int channel,
                                                               unsigned int pitch,
                                                               unsigned int velocity,
                                                               bool on) {
    
    if (channel > 16 || channel < 1) 
        throw VSCInvalidArgumentException("Channel must be > 0 and < 17");
    if (pitch > 127) 
        throw VSCInvalidArgumentException("Pitch must < 128");
    if (velocity > 127) 
        throw VSCInvalidArgumentException("Velocity must < 128");
    
    Message message (3, 0);
    
    unsigned int status;
    
    if (on) status = 143;
    else status = 127;
    
    status += channel;
    
    message[0] = status;
    message[1] = pitch;
    message[2] = velocity;
    
    return message;
    
}

VSC::MIDI::Message VSC::MIDI::MessageGenerator::messageForPolyphonicAftertouch(unsigned int channel,
                                                                               unsigned int pitch,
                                                                               unsigned int pressure) {
    
    if (channel > 16 || channel < 1) 
        throw VSCInvalidArgumentException("Channel must be > 0 and < 17");
    if (pitch > 127) 
        throw VSCInvalidArgumentException("Pitch must < 128");
    if (pressure > 127) 
        throw VSCInvalidArgumentException("Pressure must < 128");
    
    Message message (3, 0);
    
    unsigned int status = 159;
    
    status += channel;
    
    message[0] = status;
    message[1] = pitch;
    message[2] = pressure;
    
    return message;
    
}

VSC::MIDI::Message VSC::MIDI::MessageGenerator::messageForChannelAftertouch(unsigned int channel, unsigned int pressure) {
    
    if (channel > 16 || channel < 1) 
        throw VSCInvalidArgumentException("Channel must be > 0 and < 17");
    if (pressure > 127) 
        throw VSCInvalidArgumentException("Pressure must < 128");
    
    Message message (3, 0);
    
    unsigned int status = 207;
    
    status += channel;
    
    message[0] = status;
    message[1] = pressure;
    message[2] = 0;
    
    return message;
    
}

VSC::MIDI::Message VSC::MIDI::MessageGenerator::messageForControlChange(unsigned int channel, ControlNumber control, unsigned int value) {
    
    if (channel > 16 || channel < 1) 
        throw VSCInvalidArgumentException("Channel must be > 0 and < 17");
    if (control > 127) 
        throw VSCInvalidArgumentException("Control must < 128");
    if (value > 127) 
        throw VSCInvalidArgumentException("Pressure must < 128");
    
    Message message (3, 0);
    
    unsigned int status = 175;
    
    status += channel;
    
    message[0] = status;
    message[1] = control;
    message[2] = value;
    
    return message;
    
}

VSC::MIDI::MessagePair VSC::MIDI::MessageGenerator::messagePairForControlChange(unsigned int channel, ControlNumber control, Float value)
{
    return MessagePair(Message(3, 0), Message(3, 0));
}


VSC::MIDI::Message VSC::MIDI::MessageGenerator::messageForPitchWheel(unsigned int channel, Float value)
{
    return Message(3, 0);
}


bool VSC::MIDI::MessageGenerator::floatValueToBytePair(Float value, unsigned char& MSB, unsigned char& LSB)
{
    return false;
}


#pragma mark - PortManager

VSC::MIDI::PortManager::PortManager(void) {
    
    // RtMidiOut constructor
    try {
        mMIDIOut = RtMidiOutPtr(new RtMidiOut());
    }
    catch ( RtError &error ) {
        error.printMessage();
    }
    
    // RtMidiIn constructor
    try {
        mMIDIIn = RtMidiInPtr(new RtMidiIn());
    }
    catch (RtError &error) {
        // Handle the exception here
        error.printMessage();
    }
    
}

VSC::MIDI::PortManager::~PortManager(void) {
    
    // no need (WE MUST NOT!) to delete the rtmidi in/out, we are using smart pointers :)
    
}

void VSC::MIDI::PortManager::refreshOutputPorts(void) {
    
    BOOST_ASSERT_MSG(mMIDIOut, "Expected MIDI Out");
    
    mOutputPorts.clear();
    
    // Check outputs.
    unsigned int nPorts = mMIDIOut->getPortCount();
    std::string portName;
    //std::cout << "\nThere are " << nPorts << " MIDI output ports available.\n";
    for ( unsigned int i=0; i<nPorts; i++ ) {
        try {
            portName = mMIDIOut->getPortName(i);
            OutputPort p = {i, portName, false};
            mOutputPorts.push_back(p);
            //std::cout << "  Output Port #" << i+1 << ": " << portName << '\n';
        }
        catch (RtError &error) {
            throw VSCMIDIException(error.getMessage());
        }
    }
    std::cout << std::endl;
    
}

void VSC::MIDI::PortManager::refreshInputPorts(void) {
    
    mInputPorts.clear();
    
    // Check inputs.
    unsigned int nPorts = mMIDIIn->getPortCount();
    //std::cout << "\nThere are " << nPorts << " MIDI input sources available.\n";
    std::string portName;
    for ( unsigned int i=0; i<nPorts; i++ ) {
        try {
            portName = mMIDIIn->getPortName(i);
            InputPort p = {i, portName, false};
            mInputPorts.push_back(p);
            //std::cout << "  Input Port #" << i+1 << ": " << portName << '\n';
        }
        catch ( RtError &error ) {
            throw VSCMIDIException(error.getMessage());
        }
    }
    
}

const VSC::MIDI::OutputPorts& VSC::MIDI::PortManager::getOutputPorts(void) const {
    return mOutputPorts;
}

const VSC::MIDI::InputPorts& VSC::MIDI::PortManager::getInputPorts(void) const {
    return mInputPorts;
}

const std::string VSC::MIDI::PortManager::outputPortDescription(void) const {
    
    std::ostringstream s;
    
    if (mOutputPorts.size() == 0) {
        s << "No output ports for VSCMIDI " << this << "\n";
    }
    else {
        s << mOutputPorts.size() <<  " output ports for VSCMIDI " << this << ":\n";
        for (OutputPorts::const_iterator it = mOutputPorts.begin(); it != mOutputPorts.end(); ++it) {
            s << "    " << *it << "\n";
        }
    }
    
    return s.str();
}

