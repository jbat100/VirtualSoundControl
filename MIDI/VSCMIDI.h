/*
 *  VSCBoost.h
 *  EnveloppeEditor
 *
 *  Created by Jonathan Thorpe on 26/08/2011.
 *  Copyright 2011 JBAT. All rights reserved.
 *
 */

#ifndef _VSC_MIDI_H_
#define _VSC_MIDI_H_

#include <list>
#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>
#include "RtMidi.h"

typedef  boost::shared_ptr<RtMidiIn>    RtMidiInPtr;
typedef  boost::shared_ptr<RtMidiOut>   RtMidiOutPtr;

struct VSCMIDIOutputPort {
    unsigned int number;
    std::string name;
    bool isVirtual;
    bool operator!=(const VSCMIDIOutputPort& p) const;
    bool operator==(const VSCMIDIOutputPort& p) const;
};

std::ostream& operator<<(std::ostream& output, const VSCMIDIOutputPort& p);

struct VSCMIDIInputPort {
    unsigned int number;
    std::string name;
    bool isVirtual;
    bool operator!=(const VSCMIDIInputPort& p) const;
    bool operator==(const VSCMIDIInputPort& p) const;
};

std::ostream& operator<<(std::ostream& output, const VSCMIDIInputPort& p);

extern const VSCMIDIOutputPort VSCMIDIOutputPortVoid;
extern const VSCMIDIInputPort VSCMIDIInputPortVoid;

class VSCMIDI;
typedef boost::shared_ptr<VSCMIDI> VSCMIDIPtr;

class VSCMIDI {
    
public:
    
    typedef std::vector<unsigned char>      Message;
    typedef std::list<VSCMIDIOutputPort>    OutputPortList;
    typedef std::list<VSCMIDIInputPort>     InputPortList;
    
    //static VSCMIDIPtr sharedInstance(void);
    
    static Message messageForNote(unsigned int channel, unsigned int pitch, unsigned int velocity, bool on);
    static Message messageForPolyphonicAftertouch(unsigned int channel, unsigned int pitch, unsigned int pressure);
    static Message messageForChannelAftertouch(unsigned int channel, unsigned int pitch, unsigned int pressure);
    static Message messageForControl(unsigned int channel, unsigned int control, unsigned int value);
    static std::string messageDescription(const Message& m);
    
    VSCMIDI(void);
    ~VSCMIDI(void); // no need to make the destructor virtual if not subclassed, it might slow things down
    
    void refreshInputPorts(void);
    void refreshOutputPorts(void);
    
    const OutputPortList& getOutputPorts(void) const;
    const InputPortList& getInputPorts(void) const;
    
    const std::string outputPortDescription(void) const;
    
    const std::vector<unsigned int>& controlChannels(void);
    
private:
    
    RtMidiInPtr     _midiIn;
    RtMidiOutPtr    _midiOut;
    
    InputPortList _inputPorts;
    OutputPortList _outputPorts;
    
    std::vector<unsigned int> _controlChannels;
    
};

#endif
