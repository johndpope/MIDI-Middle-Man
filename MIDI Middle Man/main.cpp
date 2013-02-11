#include <CoreMIDI/MIDIServices.h>
#include <CoreFoundation/CFRunLoop.h>
#include <stdio.h>

#define DESIRED_SOURCE_NAME "launchpad s"
#define DESIRED_DESTINATION_NAME "launchpad s"

MIDIEndpointRef     source, destination;
MIDIEndpointRef     inputDev, outputDev;
MIDIPortRef         inputPort, outputPort;
ItemCount           gSources, gDestinations;


// send MIDI data from port to endpoint
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
    
    // create MIDI input port to receive MIDI from device
    MIDIInputPortCreate(client, CFSTR("MIDI Middle Man"), InputReadProc, NULL , &inputPort);
    MIDIOutputPortCreate(client, CFSTR("MIDI Middle Man"), &outputPort);
    
    // create MIDI source - where applications pull MIDI from client
    MIDISourceCreate(client, CFSTR("MIDI Middle Man"), &source);
    MIDIDestinationCreate(client, CFSTR("MIDI Middle Man"), DestinationReadProc, NULL, &destination);
        
    gSources = MIDIGetNumberOfSources();
    gDestinations = MIDIGetNumberOfDestinations();
    
    ConnectInputs();
    ConnectOutputs();
     
    CFRunLoopRun();

    
    /*

    // print the name, manufacturer and model for all devices
	CFStringRef pname, pmanuf, pmodel;
	char name[64], manuf[64], model[64];
    
    ItemCount n = MIDIGetNumberOfDevices();
	for (int i = 0; i < n; ++i) {
		MIDIDeviceRef dev = MIDIGetDevice(i);
		
		MIDIObjectGetStringProperty(dev, kMIDIPropertyName, &pname);
		MIDIObjectGetStringProperty(dev, kMIDIPropertyManufacturer, &pmanuf);
		MIDIObjectGetStringProperty(dev, kMIDIPropertyModel, &pmodel);
		
		CFStringGetCString(pname, name, sizeof(name), 0);
		CFStringGetCString(pmanuf, manuf, sizeof(manuf), 0);
		CFStringGetCString(pmodel, model, sizeof(model), 0);
		CFRelease(pname);
		CFRelease(pmanuf);
		CFRelease(pmodel);
        
        
		printf("number=%i name=%s, manuf=%s, model=%s\n", i, name, manuf, model);
        
	}
    
    */
    
	// run until aborted with control-C
    
    
    return 0;
}



