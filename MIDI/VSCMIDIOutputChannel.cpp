#include "VSCMIDIOutputChannel.h"
#include "VSCException.h"

VSC::MIDI::OutputChannel::OutputChannel(void) : mMIDIChannel(0)
{
    mMessageGenerator = MessageGenerator::SPtr(new MessageGenerator);
}

VSC::MIDI::OutputChannel::OutputChannel(Output::SPtr output, unsigned int channel) : mMIDIOutput(output), mMIDIChannel(channel)
{
    mMessageGenerator = MessageGenerator::SPtr(new MessageGenerator);
}

bool VSC::MIDI::OutputChannel::sendNoteOn(unsigned int pitch, unsigned int velocity)
{
    Message m = mMessageGenerator->messageForNote(mMIDIChannel, pitch, velocity, true);
    mMIDIOutput->sendMessage(m);
    
    return true;
}

bool VSC::MIDI::OutputChannel::sendNoteOff(unsigned int pitch, unsigned int velocity)
{
    Message m = mMessageGenerator->messageForNote(mMIDIChannel, pitch, velocity, false);
    mMIDIOutput->sendMessage(m);
    
    return true;
}

bool VSC::MIDI::OutputChannel::sendControlChange(ControlNumber controlNumber, unsigned int value)
{
    Message m = mMessageGenerator->messageForControlChange(mMIDIChannel, controlNumber, value);
    mMIDIOutput->sendMessage(m);
    
    return true;
}

bool VSC::MIDI::OutputChannel::sendPolyphonicAftertouch(unsigned int pitch, unsigned int pressure)
{
    return false;
}

bool VSC::MIDI::OutputChannel::sendChannelAftertouch(unsigned int pressure)
{
    return false;
}

std::ostream& VSC::MIDI::operator<<(std::ostream& output, const OutputChannel& p) {
    output << "MIDIOutputChannel (" << p.getMIDIOutput() << ", Channel: " << p.getMIDIChannel() << ")";
    return output;
}
