#include <iostream>
#include <cstdlib>
#include "includes/rtmidi/RtMidi.h"

unsigned int inputPortCount  = 0;
unsigned int outputPortCount = 0;
int portConnection = -1;
std::vector<std::string> portNames;


int interpretCLA(int argc, char **argv) {
    for (int i = 1; i < argc; i++) { //start at 1 to ignore process call arg
        //check if any command arguments match device name, autoconnect if true
        for (int j = 0; j < portNames.size(); j++) { //loop through all connected device names
            if (argv[i] == portNames[j]) {
                //we found a match
                portConnection = j;
                printf("Attempting to connect to %s\n", argv[i]);
                return j;
            }
        }
    }
    return -1;
}

int getConnectionPort() {
    printf("Please indicate which device you would like to link:\n");
    for (int i = 0; i < portNames.size(); i++) { //loop through all connected device names
        printf(" -%s [Port %i]\n", portNames[i].c_str(), (i+1));
    }

    //get user input
    userInput:
        printf("Type ports 1 -> %i to link to your desired MIDI device, or q to quit.  >", (int)portNames.size());
        char responce;
        std::cin >> responce;
        std::cin.clear();
        fflush(stdin);

    if (isdigit(responce)) { //we will only accept a numerical value
        int port = responce - '0'; //convert char to integer
        if (port > 0 && port <= portNames.size()) {
            portConnection = port;
            return port;
        } else {
            printf("The entered port number [%i] is out of bounds! There are only %i connected devices.\n", port, (int)portNames.size());
            goto userInput;
        }
    } else if (responce == 'q') {
        return -1;
    } else {
        goto userInput;
    }
    return -1;
}

RtMidiIn* newMidiIn() {
    try {
        return new RtMidiIn();
    } catch (RtMidiError &error) {
        error.printMessage();
        exit(EXIT_FAILURE);
    }
}

RtMidiOut* newMidiOut() {
    try {
        return new RtMidiOut();
    } catch (RtMidiError &error) {
        error.printMessage();
        exit(EXIT_FAILURE);
    }
}

void cleanup(RtMidiIn* midiInput, RtMidiOut* midiOutput) {
    // Clean up
    delete midiInput;
    delete midiOutput;
}

/**
 * Probe the current device's USB ports, listing all available MiDi compatible
 *      devices, allowing the user to choose which they would like to use.
 *
 * @param midiInput and midiOutput as pointers to our port readers
 * @return True if at least 1 device was found, else False if none were found
 */
bool probeUSB(RtMidiIn* midiInput, RtMidiOut* midiOutput) {
    inputPortCount  = midiInput->getPortCount();
    outputPortCount = midiOutput->getPortCount();

    //ensure we have at least 1 device plugged in!
    if (inputPortCount == 0 && outputPortCount == 0) {
        std::cerr << "We could not find any input or output MIDI compatible devices." << std::endl;
        return false;
    }

    // Check inputs.
    std::cout << "\nThere are " << inputPortCount << " MIDI input sources available.\n";
    std::string portName;
    for (unsigned int i = 0; i < inputPortCount; i++) {
        try {
            portName = midiInput->getPortName(i);
            portNames.push_back(portName.c_str());
        } catch (RtMidiError &error) {
            error.printMessage();
            cleanup(midiInput, midiOutput);
            return false;
        }
        std::cout << "  Input Port #" << i+1 << ": " << portName << '\n';
    }

    // Check outputs.
    std::cout << "\nThere are " << outputPortCount << " MIDI output ports available.\n";
    for (unsigned int i = 0; i < outputPortCount; i++) {
        try {
            portName = midiOutput->getPortName(i);
        } catch (RtMidiError &error) {
            error.printMessage();
            cleanup(midiInput, midiOutput);
            return false;
        }
        std::cout << "  Output Port #" << i+1 << ": " << portName << '\n';
    }
    std::cout << '\n';

    return true; //made it to the end, there was at LEAST 1 device found
}

bool initRetrievers(RtMidiIn* midiInput, RtMidiOut* midiOutput) {
    probe:
    //probe ports for midi input devices
    if (!probeUSB(midiInput, midiOutput)) {
        userInput:
            std::cerr << "Please plug in a supported MIDI device!" << std::endl;
            std::cout << "Type r to reload, or q to quit.  >";
            char responce;
            std::cin >> responce;
            std::cin.clear();
            fflush(stdin);

        //printf("\nresponce: %c", responce);
        if (responce == 'r') {
            printf("Restarting probe process...\n\n");
            goto probe;
        } else if (responce == 'q') {
            return false;
        } else {
            goto userInput;
        }
    }
    return true;
}


int main(int argc, char **argv) {
    RtMidiIn  *midiInput = newMidiIn();   //Input Retriever
    RtMidiOut *midiOutput = newMidiOut(); //Output Retriever

    //probe retrievers, and continue attempting if no devices found
    if (!initRetrievers(midiInput, midiOutput)) {
        exit(0); //user did not want to continuec
    }

    //Get port connection shortcut from Command Line Arguments
    if (interpretCLA(argc, argv) == -1) {
        //get port connection from user
        if (getConnectionPort() == -1) {
            //still have no connected devices, give up
            std::cerr << "Error, we still could not connect to your MIDI device!";
            goto clean;
        }
    }

    //connect to ports and begin communication
    


    //cleanup the memory allocations
    clean:
        cleanup(midiInput, midiOutput);
        return 0;
}
