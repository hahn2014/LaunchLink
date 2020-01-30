#ifndef MIDIINPUT_H
#define MIDIINPUT_H

#include <iostream>
#include <cstdlib>
#include <signal.h>
#include "./includes/rtmidi/RtMidi.h"
#include "midilink.h"

class MIDIInput {
private:
    std::vector<unsigned char> inputMessage;
    int nBytes, i;
    double stamp;
    RtMidiIn* midiin;
public:
    MIDIInput();
    ~MIDIInput();
    void initPort(MIDILink* linker);
    void startReading();
    void stopReading();
};

#endif
