
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

namespace VSC
{
    namespace MIDI
    {
        void FillControlNumberStringsMap();
        typedef std::map<ControlNumber, std::string> ControlNumberStringMap;
        static ControlNumberStringMap controlNumberStringMap;
        
        unsigned char statusByteForMessageType(MessageType messageType);
        MessageType messageTypeForStatusByte(unsigned char statusByte);
        unsigned char channelForStatusByte(unsigned char statusByte);
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

#pragma mark - Raw MIDI message / Description Conversions

unsigned char VSC::MIDI::statusByteForMessageType(MessageType messageType)
{
    
    switch (messageType)
    {
        case MessageTypeNoteOff:
            return 0x80;
            
        case MessageTypeNoteOn:
            return 0x90;
            
        case MessageTypePolyphonicAftertouch:
            return 0xA0;
            
        case MessageTypeControlChange:
            return 0xB0;
            
        case MessageTypeProgramChange:
            return 0xC0;
            
        case MessageTypeChannelAftertouch:
            return 0xD0;
            
        case MessageTypePitchWheel:
            return 0xE0;
            
        default:
            break;
    }
    
    throw VSCInvalidArgumentException("Invalid message type");
}

VSC::MIDI::MessageType VSC::MIDI::messageTypeForStatusByte(unsigned char statusByte)
{
    unsigned char maskedByte = statusByte & 0xF0;
    
    switch (maskedByte)
    {
            
        case 0x80:
            return MessageTypeNoteOff;
            
        case 0x90:
            return MessageTypeNoteOn;
            
        case 0xA0:
            return MessageTypePolyphonicAftertouch;
            
        case 0xB0:
            return MessageTypeControlChange;
            
        case 0xC0:
            return MessageTypeProgramChange;
            
        case 0xD0:
            return MessageTypeChannelAftertouch;
            
        case 0xE0:
            return MessageTypePitchWheel;
            
        default:
            break;
    }
    
    throw VSCInvalidArgumentException("Invalid status byte");
}

unsigned char VSC::MIDI::channelForStatusByte(unsigned char statusByte)
{
    return statusByte & 0x0F;
}


VSC::MIDI::Message VSC::MIDI::messageFromDescription(MessageDescription::SPtr description)
{
    
    if(!description) throw VSCInvalidArgumentException("Invalid description");
    
    unsigned char statusByte = statusByteForMessageType(description->type);
    
    switch (statusByte)
    {
            
        case MessageTypeNoteOff:
        {
            Message message(3,0);
            statusByte += description->parameterMap[MessageParameterKeyChannel];
            message[0] = statusByte;
            message[1] = description->parameterMap[MessageParameterKeyPitch];
            message[2] = description->parameterMap[MessageParameterKeyVelocity];
            return message;
        }
        case MessageTypeNoteOn:
        {
            Message message(3,0);
            statusByte += description->parameterMap[MessageParameterKeyChannel];
            message[0] = statusByte;
            message[1] = description->parameterMap[MessageParameterKeyPitch];
            message[2] = description->parameterMap[MessageParameterKeyVelocity];
            return message;
        }
        case MessageTypePolyphonicAftertouch:
        {
            Message message(3,0);
            statusByte += description->parameterMap[MessageParameterKeyChannel];
            message[0] = statusByte;
            message[1] = description->parameterMap[MessageParameterKeyPitch];
            message[2] = description->parameterMap[MessageParameterKeyPressure];
            return message;
        }
        case MessageTypeProgramChange:
        {
            Message message(3,0);
            statusByte += description->parameterMap[MessageParameterKeyChannel];
            message[0] = statusByte;
            message[1] = description->parameterMap[MessageParameterKeyProgram];
            message[2] = 0;
            return message;
        }
        case MessageTypeControlChange:
        {
            Message message(3,0);
            statusByte += description->parameterMap[MessageParameterKeyChannel];
            message[0] = statusByte;
            message[1] = description->parameterMap[MessageParameterKeyControlNumber];
            message[2] = description->parameterMap[MessageParameterKeyValue];
            return message;
        }
        case MessageTypeChannelAftertouch:
        {
            Message message(3,0);
            statusByte += description->parameterMap[MessageParameterKeyChannel];
            message[0] = statusByte;
            message[1] = description->parameterMap[MessageParameterKeyPressure];
            message[2] = 0;
            return message;
        }
        case MessageTypePitchWheel:
        {
            Message message(3,0);
            statusByte += description->parameterMap[MessageParameterKeyChannel];
            message[0] = statusByte;
            message[1] = description->parameterMap[MessageParameterKeyValueLSB];
            message[2] = description->parameterMap[MessageParameterKeyValueMSB];
            return message;
        }
            
        default:
            break;
    }
    
    throw VSCInvalidArgumentException("Invalid description");
    
}

VSC::MIDI::MessageDescription::SPtr VSC::MIDI::descriptionFromMessage(const Message& message)
{

    BOOST_ASSERT(message.size() > 0);
    if (message.size() == 0) return MessageDescription::SPtr();
    
    unsigned char statusByte = message[0];
    MessageType messageType = messageTypeForStatusByte(message[0]);
    
    MessageDescription::SPtr description(new MessageDescription(messageType));
    
    /*
     *  Common treatment for voice messages
     */
    
    switch (messageType)
    {
        case MessageTypeNoteOff:
        case MessageTypeNoteOn:
        case MessageTypePolyphonicAftertouch:
        case MessageTypeControlChange:
        case MessageTypeProgramChange:
        case MessageTypeChannelAftertouch:
        case MessageTypePitchWheel:
            BOOST_ASSERT(message.size() == 3);
            if (message.size() < 3)
                return MessageDescription::SPtr();
            description->parameterMap[MessageParameterKeyChannel] = channelForStatusByte(statusByte);
        default:
            break;
    }
    
    /*
     *  Type specific 
     */
    
    switch (messageType)
    {
        case MessageTypeNoteOff:
        {
            description->parameterMap[MessageParameterKeyPitch] = message[1];
            description->parameterMap[MessageParameterKeyVelocity] = message[2];
        }
        case MessageTypeNoteOn:
        {
            description->parameterMap[MessageParameterKeyPitch] = message[1];
            description->parameterMap[MessageParameterKeyVelocity] = message[2];
        }
        case MessageTypePolyphonicAftertouch:
        {
            description->parameterMap[MessageParameterKeyPitch] = message[1];
            description->parameterMap[MessageParameterKeyPressure] = message[2];
        }
        case MessageTypeProgramChange:
        {
            description->parameterMap[MessageParameterKeyProgram] = message[1];
        }
        case MessageTypeControlChange:
        {
            description->parameterMap[MessageParameterKeyControlNumber] = message[1];
            description->parameterMap[MessageParameterKeyValue] = message[2];
        }
        case MessageTypeChannelAftertouch:
        {
            description->parameterMap[MessageParameterKeyPressure] = message[1];
        }
        case MessageTypePitchWheel:
        {
            description->parameterMap[MessageParameterKeyValueLSB] = message[1];
            description->parameterMap[MessageParameterKeyValueMSB] = message[2];
        }
            
        default:
            break;
    }
    
    return description;

}


bool VSC::MIDI::floatValueToBytePair(Float value, unsigned char& MSB, unsigned char& LSB)
{
    return false;
}

#pragma mark - PortManager

VSC::MIDI::PortManager::PortManager(void) {
    
    // RtMidiOut constructor
    try
    {
        mMIDIOut = RtMidiOutPtr(new RtMidiOut());
    }
    catch ( RtError &error )
    {
        error.printMessage();
    }
    
    // RtMidiIn constructor
    try
    {
        mMIDIIn = RtMidiInPtr(new RtMidiIn());
    }
    catch (RtError &error)
    {
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
    for ( unsigned int i=0; i<nPorts; i++ )
    {
        try
        {
            portName = mMIDIOut->getPortName(i);
            OutputPort p = {i, portName, false};
            mOutputPorts.push_back(p);
            //std::cout << "  Output Port #" << i+1 << ": " << portName << '\n';
        }
        catch (RtError &error)
        {
            throw VSCMIDIException(error.getMessage());
        }
    }
    std::cout << std::endl;
    
}

void VSC::MIDI::PortManager::refreshInputPorts(void)
{
    
    mInputPorts.clear();
    
    // Check inputs.
    unsigned int nPorts = mMIDIIn->getPortCount();
    //std::cout << "\nThere are " << nPorts << " MIDI input sources available.\n";
    std::string portName;
    for ( unsigned int i=0; i<nPorts; i++ )
    {
        try
        {
            portName = mMIDIIn->getPortName(i);
            InputPort p = {i, portName, false};
            mInputPorts.push_back(p);
            //std::cout << "  Input Port #" << i+1 << ": " << portName << '\n';
        }
        catch ( RtError &error )
        {
            throw VSCMIDIException(error.getMessage());
        }
    }
    
}

const VSC::MIDI::OutputPorts& VSC::MIDI::PortManager::getOutputPorts(void) const
{
    return mOutputPorts;
}

const VSC::MIDI::InputPorts& VSC::MIDI::PortManager::getInputPorts(void) const
{
    return mInputPorts;
}

const std::string VSC::MIDI::PortManager::outputPortDescription(void) const
{
    
    std::ostringstream s;
    
    if (mOutputPorts.size() == 0)
    {
        s << "No output ports for VSCMIDI " << this << "\n";
    }
    else
    {
        s << mOutputPorts.size() <<  " output ports for VSCMIDI " << this << ":\n";
        for (OutputPorts::const_iterator it = mOutputPorts.begin(); it != mOutputPorts.end(); ++it)
        {
            s << "    " << *it << "\n";
        }
    }
    
    return s.str();
}

