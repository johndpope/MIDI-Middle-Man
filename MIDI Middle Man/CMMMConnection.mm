//
//  CMMMConnection.m
//  MIDI Middle Man
//
//  Created by Joe Noel on 27/02/2013.
//  Copyright (c) 2013 Focusrite Audio Engineering Ltd. All rights reserved.
//

#import "CMMMConnection.h"

@interface CMMMConnection()

// Callback functions
static void	InputReadProc(const MIDIPacketList *pktlist, void * refCon, void * connRefCon);
static void	DestinationReadProc(const MIDIPacketList *pktlist, void *refCon, void *connRefCon);
static void NotifyProc (const MIDINotification *message, void *refCon);


@end

@implementation CMMMConnection

@synthesize instanceNumber;
@synthesize instanceName;

static void	InputReadProc(const MIDIPacketList *pktlist, void * refCon, void * connRefCon)
{
    MIDIEndpointRef source = self->source;
    
	MIDIReceived(source, pktlist); // distribute MIDI to source
}

static void	DestinationReadProc(const MIDIPacketList *pktlist, void *refCon, void *connRefCon)
{
    
    /*
    OutputPortDevicePair *outputPortDevicePair = (OutputPortDevicePair *) refCon;
    MIDIPortRef outputPort = outputPortDevicePair->outputPort;
    MIDIEndpointRef outputDevice = outputPortDevicePair->outputDevice;
    
    if (outputPort != NULL && outputDevice != NULL)
    {
        MIDISend(outputPort,outputDevice, pktlist); // distribute MIDI from outputPort to outputDev
        // printf("Sending MIDI\n");
    }
     */
    
}

static void NotifyProc (const MIDINotification *message, void *refCon)
{
    /*
    
    if ( HaveSourcesChanged() || HaveDestinationsChanged())
    {
        ListCurrentSources(sourceList);
        
        ListCurrentDestinations(destinationList);
    }
     */
    
}


- (id) CreateMMMConnection:(int)number ToSource:(NSString *)desiredSourceName andDestination:(NSString *)desiredDestinationName
{
    self.instanceNumber = number;
    self.instanceName = CFStringCreateWithFormat(NULL, NULL, CFSTR("MIDI Middle Man %i"), self.instanceNumber);
    
    void* pointer = &(self->source);
    
    // create client
    MIDIClientCreate(
                     self.instanceName,
                     NotifyProc,
                     NULL,
                     &(self->client));

    // create input port
    MIDIInputPortCreate(
                        self->client,
                        self.instanceName,
                        InputReadProc,
                        NULL,
                        &(self->inputPort));
    
    // create output port
    MIDIOutputPortCreate(
                         self->client,
                         self.instanceName,
                         &(self->outputPort));

    // create source
    MIDISourceCreate(
                     self->client,
                     self.instanceName,
                     &(self->source));
    
    // create destination
    MIDIDestinationCreate(
                          self->client,
                          self.instanceName,
                          DestinationReadProc,
                          NULL,
                          &(self->destination));


    
    // check if desired source is connected, connect up
    
    
    // check if desired destination is connected, connect up
    
    return self;
}


@end
