#include "midilink.h"

bool done = false;

static void finish(int ignore){ done = true; }

void test(MIDILink* linker) {
    RtMidiIn* midiin = linker->getInputRetriever();
    std::vector<unsigned char> message;
    int nBytes, i;
    double stamp;


    midiin->openPort(linker->portConnection); //link to the desired port
    // Don't ignore sysex, timing, or active sensing messages.
    midiin->ignoreTypes(false, false, false);

    (void)signal(SIGINT, finish);

    std::cout << "Reading MIDI from port ... quit with Ctrl-C.\n";
    while (!done) {
        stamp = midiin->getMessage(&message);
        nBytes = message.size();
        for (i = 0; i < nBytes; i++) {
            std::cout << "Byte " << i << " = " << (int)message[i] << ", ";
        }
        if (nBytes > 0)
            std::cout << "stamp = " << stamp << std::endl;
        // Sleep for 10 milliseconds ... platform-dependent.
        //SLEEP(10);
    }
}



int main(int argc, char** argv) {
    MIDILink* linker = new MIDILink();
    if (!linker->link(argc, argv)) {
        std::cerr << Red << "Error! We couldn't link to a MIDI compatible device!" << Reset << std::endl;
        delete linker;
        return -1;
    }

    //open ports and begin communication phase
    test(linker);

    //cleanup
    delete linker;
}
