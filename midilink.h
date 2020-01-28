#ifndef MIDILINK_H
#define MIDILINK_H

#include <iostream>
#include <cstdlib>
#include "includes/rtmidi/RtMidi.h"
#include "colors.h"

class MIDILink {
    private:
        int interpretCLA(int argc, char** argv);
        int getConnectionPort();
        RtMidiIn* newMidiIn();
        RtMidiOut* newMidiOut();
        void cleanup(RtMidiIn* midiInput, RtMidiOut* midiOutput);
        bool probeUSB(RtMidiIn* midiInput, RtMidiOut* midiOutput);

        //private variables
        RtMidiIn* midiInput;   //Input Retriever
        RtMidiOut* midiOutput; //Output Retriever
        unsigned int inputPortCount;
        unsigned int outputPortCount;

    public:
        MIDILink();                       //default constructor
        ~MIDILink();                      //class deconstructor
        int portConnection;
        std::vector<std::string> portNames;

        bool link(int argc, char** argv);
        bool initRetrievers(RtMidiIn* midiInput, RtMidiOut* midiOutput);

        void printDeviceList();
        RtMidiIn* getInputRetriever();
        RtMidiOut* getOutputRetriever();
};
#endif
