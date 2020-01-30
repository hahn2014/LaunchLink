// qmidiin.cpp
#include "midiInput.h"
bool keepReceiving;

static void finish(int ignore) {
    keepReceiving = false;
}

MIDIInput::MIDIInput() {

}

MIDIInput::~MIDIInput() {

}

void MIDIInput::initPort(MIDILink* linker) {
    midiin = linker->getInputRetriever();
    std::cout << "Reading MIDI from port" << std::endl;
    midiin->openPort(linker->portConnection);
    // Don't ignore sysex, timing, or active sensing messages.
    midiin->ignoreTypes(false, false, false);
    keepReceiving = true;
}

void MIDIInput::startReading() {
    //set signal type to sigint
    (void)signal(SIGINT, finish);
    while (keepReceiving) {
        stamp = midiin->getMessage(&inputMessage);
        nBytes = inputMessage.size();
        for (i = 0; i < nBytes; i++) {
            std::cout << "Byte " << i << " = " << (int)inputMessage[i] << ", ";
        }
        if (nBytes > 0)
            std::cout << "stamp = " << stamp << std::endl;
        // Sleep for 10 milliseconds ... platform-dependent.
        //SLEEP(10);
    }
}

void MIDIInput::stopReading() {
    keepReceiving = false;
}
