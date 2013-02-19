#include <CoreMIDI/MIDIServices.h>
#include <CoreFoundation/CFRunLoop.h>
#include <stdio.h>
#import <Cocoa/Cocoa.h>
#include <typeinfo>

#define DESIRED_SOURCE_NAME "Launchkey MIDI"
#define DESIRED_DESTINATION_NAME "Launchkey MIDI"

MIDIEndpointRef     source, destination;
MIDIEndpointRef     inputDev, outputDev;
MIDIPortRef         inputPort, outputPort;
ItemCount           gSources, gDestinations;
NSMutableArray      *sourceList, *destinationList;

// when MIDI present at input port (should be connected to external source), distributes MIDI to source
static void	InputReadProc(const MIDIPacketList *pktlist, void *refCon, void *connRefCon)
{
	if (inputPort != NULL && source != NULL)
    {
	MIDIReceived(source, pktlist);
    }
}

// when MIDI present at destination, distributes MIDI from output port to chosen output devices
static void	DestinationReadProc(const MIDIPacketList *pktlist, void *refCon, void *connRefCon)
{
    if (outputPort != NULL && outputDev != NULL)
    {
    MIDISend(outputPort, outputDev, pktlist);
    }
}

// stores a list of current sources in sourceList and returns the number of sources
ItemCount ListCurrentSources(NSMutableArray* sourceList)
{
    ItemCount sources = MIDIGetNumberOfSources();
    CFStringRef sourceName;
    
    for (int nnn = 0; nnn < sources; ++nnn)
    {
        
        MIDIEndpointRef endpoint = MIDIGetSource(nnn);
        MIDIObjectGetStringProperty( endpoint, kMIDIPropertyName, &sourceName);
        NSString * pNSSourceName = (__bridge  NSString *)sourceName;
        [sourceList insertObject: pNSSourceName atIndex: nnn];

    }
    return sources;
}

// queries whether the desiredSourceName is in the list and returns the index
// returns NSNotFound if not in list
NSUInteger FindIndexOfDesiredSource(NSArray * sourceList, CFStringRef desiredSourceName)
{
    NSString * pNSDesiredSourceName = (__bridge NSString *) desiredSourceName;
    
    return [sourceList indexOfObject:pNSDesiredSourceName];
    
}

// connects a source at an index to the inputPort
void ConnectInputs(NSUInteger indexOfDesiredSource, MIDIEndpointRef inputPort)
{     
        MIDIEndpointRef inputDev = MIDIGetSource((int) indexOfDesiredSource); // more elegant cast here?
        MIDIPortConnectSource(inputPort, inputDev, NULL);
}

/*

void ConnectOutputs()
{

        MIDIEndpointRef endpoint = MIDIGetDestination(nnn);
        MIDIObjectGetStringProperty( endpoint, kMIDIPropertyName, &destinationName);
        
        // compared desired destination to available destination
        comparisonResult = CFStringCompare(destinationName, desiredDestinationName, kCFCompareCaseInsensitive);
        
        if (comparisonResult == kCFCompareEqualTo)
        {
            
            outputDev = endpoint;
            char cDestinationName[64];
            CFStringGetCString(destinationName, cDestinationName, sizeof(cDestinationName), 0);
            printf("%s connected to MIDI Middle Man Output\n", cDestinationName);
            
        };
        
    }
}
 
*/


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
        ConnectInputs();
        ConnectOutputs();
    }
    
}




int main(int argc, const char * argv[])
{
    // create MIDI client
    MIDIClientRef       client;
    MIDIClientCreate(CFSTR("MIDI Middle Man"), NotifyProc, NULL, &client);
    
    // create MIDI ports
    MIDIInputPortCreate(client, CFSTR("MIDI Middle Man"), InputReadProc, NULL , &inputPort);
    MIDIOutputPortCreate(client, CFSTR("MIDI Middle Man"), &outputPort);
    
    // create MIDI source and destination
    MIDISourceCreate(client, CFSTR("MIDI Middle Man"), &source);
    MIDIDestinationCreate(client, CFSTR("MIDI Middle Man"), DestinationReadProc, NULL, &destination);
    
    // create an empty array for source list
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



