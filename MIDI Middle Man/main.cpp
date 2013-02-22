#include <stdio.h>
#include "MIDI Middle Man.hpp"

#define DESIRED_SOURCE_NAME "Launchpad S"
#define DESIRED_DESTINATION_NAME "Launchpad S"

int main(int argc, const char * argv[])
{
    MIDIClientRef           client;
    MIDIPortRef             inputPort, outputPort;
    MIDIEndpointRef         mySource, myDestination;
    OutputPortDevicePair    myOutputPortDevicePair;

    // create MIDI client
    MIDIClientCreate(CFSTR("MIDI Middle Man"), NotifyProc, NULL, &client);
    
    // create MIDI source and destination
    MIDISourceCreate(client, CFSTR("MIDI Middle Man"), &mySource);
    MIDIDestinationCreate(client, CFSTR("MIDI Middle Man"), DestinationReadProc, &myOutputPortDevicePair, &myDestination);
        
    // create MIDI ports
    MIDIInputPortCreate(client, CFSTR("MIDI Middle Man"), InputReadProc, &mySource , &inputPort);
    MIDIOutputPortCreate(client, CFSTR("MIDI Middle Man"), &outputPort);
    myOutputPortDevicePair.outputPort = outputPort;

    
    // create an empty array for source list
    sourceList = [ NSMutableArray array ];
    destinationList = [ NSMutableArray array];
    
    // get global number of sources and destinations
    gSources = MIDIGetNumberOfSources();
    gDestinations = MIDIGetNumberOfDestinations();
    
    CFStringRef desiredSourceName = CFSTR(DESIRED_SOURCE_NAME);
    NSUInteger indexOfDesiredSource = -1;
    ListCurrentSources(sourceList);
    indexOfDesiredSource = FindIndexOfDesiredSource(sourceList, desiredSourceName);
    ConnectInputs(indexOfDesiredSource, inputPort);
    
    CFStringRef desiredDestinationName = CFSTR(DESIRED_DESTINATION_NAME);
    NSUInteger indexOfDesiredDestination = -1;
    ListCurrentDestinations(destinationList);
    indexOfDesiredDestination = FindIndexOfDesiredDestination(destinationList, desiredDestinationName);
    ConnectOutputs(indexOfDesiredDestination, &myOutputPortDevicePair);
    
    
    CFRunLoopRun();

    
    
    return 0;
}



