#include <CoreMIDI/MIDIServices.h>
#include <CoreFoundation/CFRunLoop.h>
#include <stdio.h>

MIDIPortRef		gOutPort = NULL;
MIDIEndpointRef	gDest = NULL;
int				gChannel = 0;


// Send MIDI data from port to endpoint
static void	MyReadProc(const MIDIPacketList *pktlist, void *refCon, void *connRefCon)
{
	if (gOutPort != NULL && gDest != NULL) {
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
        
		MIDISend(gOutPort, gDest, pktlist);
	}
}

int main(int argc, const char * argv[])
{
    OSStatus err = noErr;
    
    // create client
	MIDIClientRef client = NULL;
	err = MIDIClientCreate(CFSTR("MIDI Middle Man"), NULL, NULL, &client);
	
    // create ports
	MIDIPortRef inPort = NULL;
	err = MIDIInputPortCreate(client, CFSTR("MIDI Middle Man - Input"), MyReadProc, NULL, &inPort);
	err = MIDIOutputPortCreate(client, CFSTR("MIDI Middle Man - Output"), &gOutPort);
    
    // create input endpoint
    MIDIEndpointRef gSource = NULL;
    
    // Create source and destination
    err = MIDISourceCreate(client, CFSTR("MIDI Middle Man"), &gSource);
    err = MIDIDestinationCreate(client, CFSTR("MIDI Middle Man"), MyReadProc, NULL, &gDest);
    
    // connect input endpoint to input port
    gSource = MIDIGetSource(0); // Put source number in here
    err = MIDIPortConnectSource(inPort, gSource, NULL);
    
    
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
    
    
    
    CFRunLoopRun();
	// run until aborted with control-C
    
    
    return 0;
}



