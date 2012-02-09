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


struct VSCMidiOutputPort {
    unsigned int number;
    std::string name;
};

struct VSCMidiInputPort {
    unsigned int number;
    std::string name;
};

#endif