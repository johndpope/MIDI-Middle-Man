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
NSMutableArray*     sourceList;

static void	InputReadProc(const MIDIPacketList *pktlist, void *refCon, void *connRefCon)
{
	if (inputPort != NULL && source != NULL)
    {
	MIDIReceived(source, pktlist);
    }
}

static void	DestinationReadProc(const MIDIPacketList *pktlist, void *refCon, void *connRefCon)
{
    if (outputPort != NULL && outputDev != NULL)
    {
    MIDISend(outputPort, outputDev, pktlist);
    }
}

// stores a list of current sources in sourceList and returns the number of sources
ItemCount AcquireCurrentSources(NSMutableArray* sourceList)
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

void ConnectInputs()
{
    
    // connect source with chosen name
    ItemCount sources = MIDIGetNumberOfSources();
    CFStringRef sourceName;
    CFStringRef desiredSourceName = CFSTR(DESIRED_SOURCE_NAME);
    CFComparisonResult comparisonResult;
    for (int nnn = 0; nnn < sources; ++nnn)
    {
        MIDIEndpointRef endpoint = MIDIGetSource(nnn);
        
        // get name of source
        MIDIObjectGetStringProperty( endpoint, kMIDIPropertyName, &sourceName);
        
        comparisonResult = CFStringCompare(sourceName, desiredSourceName, kCFCompareCaseInsensitive);
        
        if (comparisonResult == kCFCompareEqualTo)
        {
            
            inputDev = endpoint;
            MIDIPortConnectSource( inputPort, inputDev, NULL);
            char cSourceName[64];
            CFStringGetCString(sourceName, cSourceName, sizeof(cSourceName), 0);
            printf("%s connected to MIDI Middle Man Input\n", cSourceName);
            
        };
        
    }
}


void ConnectOutputs()
{
    // connect destination
    ItemCount destinations = MIDIGetNumberOfDestinations();
    CFStringRef destinationName;
    CFStringRef desiredDestinationName = CFSTR(DESIRED_DESTINATION_NAME);
    CFComparisonResult comparisonResult;
    for (int nnn = 0; nnn < destinations; ++nnn)
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

void NotifyProc (const MIDINotification *message, void *refCon)
{

    if ( HaveSourcesChanged() || HaveDestinationsChanged())
    {
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
    
    // create and return an empty array for source list
    sourceList = [ NSMutableArray array ];
    
    
    gSources = MIDIGetNumberOfSources();
    gDestinations = MIDIGetNumberOfDestinations();
    
    // ConnectInputs();
    //  ConnectOutputs();
    
    AcquireCurrentSources(sourceList);
    CFStringRef desiredSourceName = CFSTR(DESIRED_SOURCE_NAME);
    NSUInteger indexOfDesiredSource = -1;
    indexOfDesiredSource = FindIndexOfDesiredSource(sourceList, desiredSourceName);
    if (indexOfDesiredSource == NSNotFound)
    {
        NSLog(@"Desired source not connected");
    }
    else
    {
        NSLog(@"Desired source is at index: %lu", (unsigned long) indexOfDesiredSource);
        
    }
        
    CFRunLoopRun();

    
    
    return 0;
}



