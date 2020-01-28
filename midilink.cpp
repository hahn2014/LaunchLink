/**
 * File:        midilink.cpp
 * Author(s):   Bryce Hahn
 * Dates:
 *   Created  - Jan 27th 2020
 *   Modified - Jan 28th 2020
 * Descrip:     MIDILink is the startup process to initialize and link a MIDI
 *      device to the process. This can then be used in an abstracted way to
 *      build off Launch in more projects.
 * Usage:       Defining a Launch process will allow access to both input and
 *      output retrievers which can then be used in conjunction with
 *      (input class).cpp and (output class).cpp
 */
 #include "midilink.h"

/**
 * Iterate through all command line arguments given at launch to determine if
 *      we can auto-link to a device (Must be given by name)
 *
 * @param argc Keeping track of the number of arguments, argv a double char array
 *      holding all CLA values (char*).
 * @return The port number to connect to, or -1 to move to user input
 */
int MIDILink::interpretCLA(int argc, char **argv) {
    for (int i = 1; i < argc; i++) { //start at 1 to ignore process call arg
        //check if any command arguments match device name, autoconnect if true
        for (int j = 0; j < portNames.size(); j++) { //loop through all connected device names
            if (argv[i] == portNames[j]) {
                //we found a match
                portConnection = j;
                return j;
            }
        }
    }
    return -1;
}

/**
 * This is the last attempt at connecting to a MIDI device before quitting. We seek
 *      device approval from the user input by printing a list of devices available
 *      and requesting the port number of their desired device.
 *
 * @param Nothing
 * @return The port the user wishes to connect to, or -1 to quit
 */
int MIDILink::getConnectionPort() {
    printf("Please indicate which device you would like to link:\n");
    printDeviceList();

    //get user input
    userInput:
        if (portNames.size() == 1) {
            std::cout << "You only have 1 connected device, type " << Green << "0"
                << Reset << " for " << portNames[0].c_str() << " or " << Red << "q"
                << Reset << " to quit.  >";
        } else {
            std::cout << "Type [" << Green << "0" << Reset << " -> " << Green
                << (int)portNames.size() << Reset <<
                "] to link to the port of your desired MIDI device, or " << Red
                << "q" << Reset <<" to quit.  >";
        }
        char responce;
        std::cin >> responce;
        std::cin.clear();
        fflush(stdin);

    if (isdigit(responce)) { //we will only accept a numerical value
        int port = responce - '0'; //convert char to integer
        if (port >= 0 && port <= portNames.size()) {
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

/**
 * Basic constructor class to attempt and create a new MIDI Input retriever.
 *
 * @param Nothing
 * @return RtMidiIn pointer if no errors were thrown
 */
RtMidiIn* MIDILink::newMidiIn() {
    try {
        return new RtMidiIn();
    } catch (RtMidiError &error) {
        error.printMessage();
        exit(EXIT_FAILURE);
    }
}

/**
 * Basic constructor class to attempt and create a new MIDI Output retriever.
 *
 * @param Nothing
 * @return RtMidiOut pointer if no errors were thrown
 */
RtMidiOut* MIDILink::newMidiOut() {
    try {
        return new RtMidiOut();
    } catch (RtMidiError &error) {
        error.printMessage();
        exit(EXIT_FAILURE);
    }
}

void MIDILink::cleanup(RtMidiIn* midiInput, RtMidiOut* midiOutput) {
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
bool MIDILink::probeUSB(RtMidiIn* midiInput, RtMidiOut* midiOutput) {
    inputPortCount  = midiInput->getPortCount();
    outputPortCount = midiOutput->getPortCount();

    //ensure we have at least 1 device plugged in!
    if (inputPortCount == 0 && outputPortCount == 0) {
        std::cerr << Red << "We could not find any input or output MIDI compatible devices." << Reset << std::endl;
        return false;
    }

    // Check inputs.
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
    }

    // Check outputs.
    for (unsigned int i = 0; i < outputPortCount; i++) {
        try {
            portName = midiOutput->getPortName(i);
        } catch (RtMidiError &error) {
            error.printMessage();
            cleanup(midiInput, midiOutput);
            return false;
        }
    }

    return true; //made it to the end, there was at LEAST 1 device found
}

void MIDILink::printDeviceList() {
    std::cout << "\nThere are " << Green << inputPortCount << Reset << " MIDI input sources available.\n";
    for (unsigned int i = 0; i < inputPortCount; i++) {
        std::cout << "  Input Port #" << i << ": " << portNames[i] << '\n';
    }

    std::cout << "\nThere are " << Green << outputPortCount << Reset << " MIDI output ports available.\n";
    for (unsigned int i = 0; i < outputPortCount; i++) {
        std::cout << "  Output Port #" << i << ": " << portNames[i] << '\n';
    }
    std::cout << '\n';
}

/**
 * Retriever initializer overlayer which will first attempt to list all MIDI
 *      devices available on the system. If none were found, it will continue to
 *      ask user input to plug in a compatible device, then wait for user to either
 *      reload or quit.
 *
 * @param midiInput and midiOutput to gain access to the midi retrievers
 * @return True if at least 1 midi device was successfully connected and loaded
 *      into the list of devices, else false if user quits
 */
bool MIDILink::initRetrievers(RtMidiIn* midiInput, RtMidiOut* midiOutput) {
    probe:
    //probe ports for midi input devices
    if (!probeUSB(midiInput, midiOutput)) {
        userInput:
            std::cerr << Red << "Please plug in a supported MIDI device!" << Reset << std::endl;
            std::cout << "Type " << Green << "r" << Reset <<" to reload, or " << Red << "q" << Reset <<" to quit.  >";
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

RtMidiIn* MIDILink::getInputRetriever() {
    return this->midiInput;
}

RtMidiOut* MIDILink::getOutputRetriever(){
    return this->midiOutput;
}

bool MIDILink::link(int argc, char** argv) {
    this->midiInput = newMidiIn();
    this->midiOutput = newMidiOut();

    //probe retrievers, and continue attempting if no devices found
    if (!initRetrievers(midiInput, midiOutput)) {
        return false; //user did not want to continuec
    }

    //Get port connection shortcut from Command Line Arguments
    if (interpretCLA(argc, argv) == -1) {
        //get port connection from user
        if (getConnectionPort() == -1) {
            //still have no connected devices, give up
            std::cerr << Red << "Error, we still could not connect to your MIDI device!" << Reset;
            return false;
        }
    }
    return true;
}

MIDILink::MIDILink() {
    this->midiInput = nullptr;
    this->midiOutput = nullptr;
    this->inputPortCount = 0;
    this->outputPortCount = 0;
    this->portConnection = -1;
}

MIDILink::~MIDILink() {
    cleanup(midiInput, midiOutput);
    printf("Goodbye! Thank you so much for using LaunchLink. Copyright 2020 (C) Bryce Hahn");
}
