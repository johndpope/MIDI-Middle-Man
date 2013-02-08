#include <CoreMIDI/MIDIServices.h>
#include <CoreFoundation/CFRunLoop.h>
#include <stdio.h>

// globals
MIDIEndpointRef     inputDev, source;
MIDIClientRef       client;
MIDIPortRef         inputPort;
int                 gChannel = 0;

// send MIDI data from port to endpoint
static void	ReadProcReceive(const MIDIPacketList *pktlist, void *refCon, void *connRefCon)
{
	if (inputPort != NULL && source != NULL) {
		MIDIPacket *packet = (MIDIPacket *)pktlist->packet;	// remove const (!)
		for (unsigned int j = 0; j < pktlist->numPackets; ++j) {
			for (int i = 0; i < packet->length; ++i) {
                //				printf("%02X ", packet->data[i]);
                
				// rechannelize status bytes
				if (packet->data[i] >= 0x80 && packet->data[i] < 0xF0)
					packet->data[i] = (packet->data[i] & 0xF0) | gChannel;
			}
            
            //			printf("\n");
			packet = MIDIPacketNext(packet);
		}
        
		MIDIReceived( inputDev , pktlist ); // fill in
        MIDISend( inputPort, source , pktlist);

	}
}

/*

static void	ReadProcSend(const MIDIPacketList *pktlist, void *refCon, void *connRefCon)
{
	if (inputPort != NULL && source != NULL) { // adjust params
		MIDIPacket *packet = (MIDIPacket *)pktlist->packet;	// remove const (!)
		for (unsigned int j = 0; j < pktlist->numPackets; ++j) {
			for (int i = 0; i < packet->length; ++i) {
                //				printf("%02X ", packet->data[i]);
                
				// rechannelize status bytes
				if (packet->data[i] >= 0x80 && packet->data[i] < 0xF0)
					packet->data[i] = (packet->data[i] & 0xF0) | gChannel;
			}
            
            //			printf("\n");
			packet = MIDIPacketNext(packet);
		}
        
        MIDISend( inputPort, source , pktlist);
	}
}

*/

int main(int argc, const char * argv[])
{
    OSStatus err = noErr;
    
    err = MIDIClientCreate(CFSTR("MIDI Middle Man"), NULL, NULL, &client);
    err = MIDIInputPortCreate(client, CFSTR("MIDI Middle Man"), ReadProcReceive, NULL, &inputPort);
    err = MIDISourceCreate(client, CFSTR("MIDI Middle Man"), &source);

    inputDev = MIDIGetSource(0);

    err = MIDIPortConnectSource( inputPort, source, NULL);
    


    // print the name, manufacturer and model for all connected devices
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
    
    
    // print out list of sources
    CFStringRef sourceName;
    char sourceNameC[50];
    ItemCount sources = MIDIGetNumberOfSources();
    printf("\nNumber of sources: %i\n", sources);
    for (int ppp = 0; ppp < sources; ++ppp)
    {
        MIDIEndpointRef endpoint = MIDIGetSource(ppp);
        MIDIObjectGetStringProperty( endpoint, kMIDIPropertyName, &sourceName);
        CFStringGetCString(sourceName, sourceNameC, sizeof(sourceNameC), 0);
        printf("%i: %s\n", ppp, sourceNameC);
    }
    
    /*
    
    // print out list of destinations
    CFStringRef destinationName;
    char destinationNameC[50];
    ItemCount destinations = MIDIGetNumberOfSources();
    printf("\nNumber of destinations: %i\n", destinations);
    for (int ppp = 0; ppp < sources; ++ppp)
    {
        MIDIEndpointRef endpoint = MIDIGetDestination(ppp);
        MIDIObjectGetStringProperty( endpoint, kMIDIPropertyName, &destinationName);
        CFStringGetCString(destinationName, destinationNameC, sizeof(destinationNameC), 0);
        printf("%i: %s\n", ppp, destinationNameC);
    }
    //
    
    */
    
    
    CFRunLoopRun();
	// run until aborted with control-C
    
    
    return 0;
}



