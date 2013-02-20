#include <CoreMIDI/MIDIServices.h>
#include <CoreFoundation/CFRunLoop.h>
#include <stdio.h>
#import <Cocoa/Cocoa.h>

#define DESIRED_SOURCE_NAME "Launchkey MIDI"
#define DESIRED_DESTINATION_NAME "Launchkey MIDI"

MIDIEndpointRef     source, destination;
MIDIEndpointRef     inputDev, outputDev;
MIDIPortRef         inputPort, outputPort;
ItemCount           gSources, gDestinations;

// when MIDI present at input port (should be connected to external source), distributes MIDI to source
// should be passed source from MIDIInputPortCreate
static void	InputReadProc(const MIDIPacketList *pktlist, void * refCon, void * connRefCon)
{
    UInt32 * sourceNumber;
    sourceNumber = (UInt32 *) refCon;
    MIDIEndpointRef source = * sourceNumber;
	MIDIReceived(source, pktlist);
}

// when MIDI present at destination, distributes MIDI from output port to chosen output devices
static void	DestinationReadProc(const MIDIPacketList *pktlist, void *refCon, void *connRefCon)
{
    MIDISend(outputPort, outputDev, pktlist);
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
    return destinations;
}

// queries whether the desiredSourceName is in the list and returns the index
// returns NSNotFound if not in list
NSUInteger FindIndexOfDesiredSource(NSArray * sourceList, CFStringRef desiredSourceName)
{
    NSString * pNSDesiredSourceName = (__bridge NSString *) desiredSourceName;
    
    return [sourceList indexOfObject:pNSDesiredSourceName];
    
}

// queries whether the desiredDestinationName is in the list and returns the index
// returns NSNotFound if not in list
NSUInteger FindIndexOfDesiredDestination(NSArray * destinationList, CFStringRef desiredDestinationName)
{
    NSString * pNSDesiredDestinationName = (__bridge NSString *) desiredDestinationName;
    
    return [destinationList indexOfObject:pNSDesiredDestinationName];
    
}

// connects a source at an index to the inputPort
void ConnectInputs(NSUInteger indexOfDesiredSource, MIDIEndpointRef inputPort)
{     
        MIDIEndpointRef inputDev = MIDIGetSource((int) indexOfDesiredSource); // more elegant cast here?
        MIDIPortConnectSource(inputPort, inputDev, NULL);
}

MIDIEndpointRef GetDeviceAtDestination(NSUInteger indexOfDesiredDestination)
{
        MIDIEndpointRef outputDev = MIDIGetDestination(indexOfDesiredDestination);
        return outputDev;
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
    }
    
}

int main(int argc, const char * argv[])
{    
    // create MIDI client
    MIDIClientRef       client;
    MIDIClientCreate(CFSTR("MIDI Middle Man"), NotifyProc, NULL, &client);
    
    // create MIDI source and destination
    MIDISourceCreate(client, CFSTR("MIDI Middle Man"), &source);
    MIDIDestinationCreate(client, CFSTR("MIDI Middle Man"), DestinationReadProc, NULL, &destination);
    
    // create MIDI ports
    MIDIInputPortCreate(client, CFSTR("MIDI Middle Man"), InputReadProc, NULL , &inputPort);
    MIDIOutputPortCreate(client, CFSTR("MIDI Middle Man"), &outputPort);
    
    // create an empty array for source list
    NSMutableArray      *sourceList, *destinationList;
    sourceList = [ NSMutableArray array ];
    destinationList = [ NSMutableArray array];
    
    // get global number of sources and destinations
    gSources = MIDIGetNumberOfSources();
    gDestinations = MIDIGetNumberOfDestinations();
    
    // ConnectInputs();
    //  ConnectOutputs();
    
    CFStringRef desiredSourceName = CFSTR(DESIRED_SOURCE_NAME);
    NSUInteger indexOfDesiredSource = -1;
    ListCurrentSources(sourceList);
    indexOfDesiredSource = FindIndexOfDesiredSource(sourceList, desiredSourceName);
    
    
    CFRunLoopRun();

    
    
    return 0;
}



