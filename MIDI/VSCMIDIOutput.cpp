/*
 *  VSCBoost.cpp
 *  EnveloppeEditor
 *
 *  Created by Jonathan Thorpe on 26/08/2011.
 *  Copyright 2011 JBAT. All rights reserved.
 *
 */

#include "VSCMIDIOutput.h"
#include "VSCException.h"

#include "RtError.h"

#include <iostream>

VSC::MIDI::Output::Output(const OutputPort& outputPort) : mState(StateClosed) {
    
    if (outputPort == OutputPortVoid) throw VSCMIDIException("Expected non OutputPortVoid");
    
    this->createRtMidiOut();
    
    this->setOutputPort(outputPort); // will throw if does not succeed
    
    mMessageGenerator = MessageGenerator::SPtr(new MessageGenerator);
    
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
    
    stream << mOutputPort.number << " - " << mOutputPort.name << mOutputPort.name << (mOutputPort.isVirtual ? " (Virtual)" : "");
    
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
    return mMessageGenerator->getValidControlNumbers();
}

bool VSC::MIDI::Output::controlNumberIsValid(const ControlNumber& number)
{
    return mMessageGenerator->controlNumberIsValid(number);
}


const VSC::MIDI::OutputPort& VSC::MIDI::Output::getOutputPort(void) const {
    return mOutputPort;
}

void VSC::MIDI::Output::setOutputPort(OutputPort const& port) {
    mOutputPort = port;
}

bool VSC::MIDI::Output::sendNoteOn(unsigned int channel, unsigned int pitch, unsigned int velocity)
{
    Message m = mMessageGenerator->messageForNote(channel, pitch, velocity, true);
    this->sendMessage(m);
    
    return true;
}

bool VSC::MIDI::Output::sendNoteOff(unsigned int channel, unsigned int pitch, unsigned int velocity)
{
    Message m = mMessageGenerator->messageForNote(channel, pitch, velocity, false);
    this->sendMessage(m);
    
    return true;
}

bool VSC::MIDI::Output::sendControlChange(unsigned int channel, ControlNumber controlNumber, unsigned int value)
{
    Message m = mMessageGenerator->messageForControlChange(channel, controlNumber, value);
    this->sendMessage(m);
    
    return true;
}

bool VSC::MIDI::Output::sendPolyphonicAftertouch(unsigned int channel, unsigned int pitch, unsigned int pressure)
{
    return false;
}

bool VSC::MIDI::Output::sendChannelAftertouch(unsigned int channel, unsigned int pressure)
{
    return false;
}

bool VSC::MIDI::Output::sendMessage(Message& m) {
    
    boost::lock_guard<boost::mutex> lock(mMutex);
    
    if (mMIDIOut && mOutputPort != OutputPortVoid) {
        std::cout << *this << " sending " << messageDescription(m) << std::endl;
        mMIDIOut->sendMessage(&m);
        return true;
    }
    
    return false;
    
}

std::ostream& VSC::MIDI::operator<<(std::ostream& output, const Output& p) {
    output << "VSCMIDIOutput (" << p.getOutputPort() << ")";
    return output;
}
