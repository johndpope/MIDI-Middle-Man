#include <CoreMIDI/MIDIServices.h>
#include <CoreFoundation/CFRunLoop.h>
#include <stdio.h>
#import <Cocoa/Cocoa.h>

#define DESIRED_SOURCE_NAME "Launchpad S"
#define DESIRED_DESTINATION_NAME "Launchpad S"

ItemCount           gSources, gDestinations;
NSMutableArray      *sourceList, *destinationList;

struct OutputPortDevicePair
{
    MIDIPortRef outputPort;
    MIDIEndpointRef outputDevice;
};

// when MIDI present at input port (should be connected to external source), distributes MIDI to source
// should be passed source from MIDIInputPortCreate
static void	InputReadProc(const MIDIPacketList *pktlist, void * refCon, void * connRefCon)
{
    MIDIEndpointRef *mySource = (MIDIEndpointRef *) refCon;
    
	MIDIReceived(*mySource, pktlist); // distribute MIDI to source
}

// when MIDI present at destination, distributes MIDI from output port to chosen output devices
static void	DestinationReadProc(const MIDIPacketList *pktlist, void *refCon, void *connRefCon)
{
    
    
    OutputPortDevicePair *outputPortDevicePair = (OutputPortDevicePair *) refCon;
    MIDIPortRef outputPort = outputPortDevicePair->outputPort;
    MIDIEndpointRef outputDevice = outputPortDevicePair->outputDevice;
    
    if (outputPort != NULL && outputDevice != NULL)
    {
        MIDISend(outputPort,outputDevice, pktlist); // distribute MIDI from outputPort to outputDev
        // printf("Sending MIDI\n");
    }
    
}

// stores a list of current sources in sourceList and returns the number of sources
ItemCount ListCurrentSources(NSMutableArray* sourceList)
{
    ItemCount sources = MIDIGetNumberOfSources(); // count the number of sources
    CFStringRef sourceName;
    
    for (int nnn = 0; nnn < sources; ++nnn) // for each source
    {
        
        MIDIEndpointRef endpoint = MIDIGetSource(nnn);
        MIDIObjectGetStringProperty( endpoint, kMIDIPropertyName, &sourceName); // get name of endpoint
        NSString * pNSSourceName = (__bridge  NSString *)sourceName;
        [sourceList insertObject: pNSSourceName atIndex: nnn];

    }
    printf("Created list of sources\n");
    return sources;
}

// stores the current available destinations in destinationList array
ItemCount ListCurrentDestinations(NSMutableArray* destinationList)
{
    ItemCount destinations = MIDIGetNumberOfDestinations();
    CFStringRef destinationName;
    
    for (int nnn = 0; nnn < destinations; ++nnn)
    {
        
        MIDIEndpointRef endpoint = MIDIGetDestination(nnn);
        MIDIObjectGetStringProperty( endpoint, kMIDIPropertyName, &destinationName);
        NSString * pNSDestinationName = (__bridge  NSString *)destinationName;
        [destinationList insertObject: pNSDestinationName atIndex: nnn];
        
    }
    printf("Created list of destinations\n");
    return destinations;
}

// queries whether the desiredSourceName is in the list and returns the index
// returns NSNotFound if not in list
NSUInteger FindIndexOfDesiredSource(NSArray * sourceList, CFStringRef desiredSourceName)
{
    NSString * pNSDesiredSourceName = (__bridge NSString *) desiredSourceName;
    
    int indexOfDesiredSource = [sourceList indexOfObject:pNSDesiredSourceName];
    printf("Desired source at index: %i\n", indexOfDesiredSource);
    
    return [sourceList indexOfObject:pNSDesiredSourceName];
    
}

// queries whether the desiredDestinationName is in the list and returns the index
// returns NSNotFound if not in list
NSUInteger FindIndexOfDesiredDestination(NSArray * destinationList, CFStringRef desiredDestinationName)
{
    NSString * pNSDesiredDestinationName = (__bridge NSString *) desiredDestinationName;
    
    int indexOfDesiredDestination = [destinationList indexOfObject:pNSDesiredDestinationName];
    printf("Desired destination at index: %i\n", indexOfDesiredDestination);
    
    return [destinationList indexOfObject:pNSDesiredDestinationName];
}

// connects a source at an index to the inputPort
void ConnectInputs(NSUInteger indexOfDesiredSource, MIDIPortRef inputPort)
{     
    if (indexOfDesiredSource != NSNotFound)
    {
        MIDIEndpointRef inputDev = MIDIGetSource((int) indexOfDesiredSource); // more elegant cast here?
        MIDIPortConnectSource(inputPort, inputDev, NULL);
    }
    else
    {
        printf("Desired source not connected\n");
    }
}

void ConnectOutputs(NSUInteger indexOfDesiredDestination, OutputPortDevicePair* outputPortDevicePair)
{
    if (indexOfDesiredDestination != NSNotFound)
    {
        MIDIEndpointRef endpoint= MIDIGetDestination(indexOfDesiredDestination);
        outputPortDevicePair->outputDevice = endpoint;
    }
    else
    {
        printf("Desired destination not connected\n");
    }
    
}



// returns true if the number of sources have changed
bool HaveSourcesChanged()
{
    ItemCount sources = MIDIGetNumberOfSources();
    
    if ( sources - gSources != 0 )
    {
        gSources = sources;
        return true;
    }
    else
    {
        return false;
    }
    
}

// returns true if the number of destinations has changed
bool HaveDestinationsChanged()
{
    ItemCount destinations = MIDIGetNumberOfDestinations();
    
    if ( (destinations - gDestinations) != 0 )
    {
        gDestinations = destinations;
        return true;
    }
    else
    {
        return false;
    }
    
}

// does something if the number of sources or destinations has changed
void NotifyProc (const MIDINotification *message, void *refCon)
{

    if ( HaveSourcesChanged() || HaveDestinationsChanged())
    {
        // something
        ListCurrentSources(sourceList);
        
        ListCurrentDestinations(destinationList);
    }
    
}

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



