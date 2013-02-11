#include <CoreMIDI/MIDIServices.h>
#include <CoreFoundation/CFRunLoop.h>
// #include <CoreMIDI/CoreMIDI.h>
#include <stdio.h>

#define DESIRED_SOURCE_NAME "launchpad S"

// globals
MIDIEndpointRef     source;
int                 gChannel = 0;
MIDIEndpointRef     inputDev;
MIDIPortRef         inputPort;

// send MIDI data from port to endpoint
static void	ReadProc(const MIDIPacketList *pktlist, void *refCon, void *connRefCon)
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
        
        // MIDISend( outputPort, source , pktlist);
        MIDIReceived(source, pktlist);

	}
}

int main(int argc, const char * argv[])
{
    OSStatus err = noErr;
        
    // create MIDI client
    MIDIClientRef       client;
    MIDIClientCreate(CFSTR("MIDI Middle Man"), NULL, NULL, &client);
    
    // create MIDI input port to receive MIDI from device
    MIDIInputPortCreate(client, CFSTR("MIDI Middle Man"), ReadProc, NULL, &inputPort);
    // MIDIOutputPortCreate(client, CFSTR("MIDI Middle Man"), &outputPort);
    
    // create MIDI source - where applications pull MIDI from client
    MIDISourceCreate(client, CFSTR("MIDI Middle Man"), &source);
    

    // Connect source with chosen name
    ItemCount sources = MIDIGetNumberOfSources();
    MIDIEndpointRef endpoint;
    CFStringRef sourceName;
    CFStringRef desiredSourceName = CFSTR(DESIRED_SOURCE_NAME);
    CFComparisonResult comparisonResult = kCFCompareEqualTo;
    for (int nnn = 0; nnn < sources; ++nnn) {
        endpoint = MIDIGetSource(nnn);
        MIDIObjectGetStringProperty( endpoint, kMIDIPropertyName, &sourceName);
        
        comparisonResult = CFStringCompare(sourceName, desiredSourceName, kCFCompareCaseInsensitive);
        
        if (comparisonResult == kCFCompareEqualTo)
        {
            
            inputDev = endpoint;
            MIDIPortConnectSource( inputPort, inputDev, NULL);

        };

    }    
    
    

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
    
    
   
    
    CFRunLoopRun();
	// run until aborted with control-C
    
    
    return 0;
}



