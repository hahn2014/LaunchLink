#include "midilink.h"
#include "midiInput.h"
//#include "midiOutput.h"

int main(int argc, char** argv) {
    MIDILink* linker = new MIDILink();
    if (!linker->link(argc, argv)) {
        std::cerr << Red << "Error! We couldn't link to a MIDI compatible device!" << Reset << std::endl;
        delete linker;
        return -1;
    }

    //open ports and begin communication phase
    //INPUT
    MIDIInput* input = new MIDIInput();
    input->initPort(linker);
    input->startReading();

    //cleanup
    delete linker;
    delete input;
    //delete output;
}
