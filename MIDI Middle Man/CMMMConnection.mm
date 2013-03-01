//
//  CMMMConnection.m
//  MIDI Middle Man
//
//  Created by Joe Noel on 27/02/2013.
//  Copyright (c) 2013 Focusrite Audio Engineering Ltd. All rights reserved.
//

#import "CMMMConnection.h"

@implementation CMMMConnection

@synthesize instanceNumber, instanceName;
@synthesize isSourceConnected, isDestinationConnected;
@synthesize desiredSourceName, desiredDestinationName;
@synthesize delegate;

static void	InputReadProc(const MIDIPacketList *pktlist, void * refCon, void * connRefCon)
{
    MIDIEndpointRef *source = (MIDIEndpointRef *)refCon;
    
	MIDIReceived(*source, pktlist); // distribute MIDI to source
}

static void	DestinationReadProc(const MIDIPacketList *pktlist, void *refCon, void *connRefCon)
{
    
    CMMMConnection *thisOne = (__bridge CMMMConnection *) refCon; // memory leak?
    
    MIDIPortRef outputPort = thisOne->outputPort;
    MIDIEndpointRef outputDevice = thisOne->outputDevice;
    
    if (outputPort != NULL && outputDevice != NULL)
    {
        MIDISend(outputPort,outputDevice, pktlist); // distribute MIDI from outputPort to outputDev
        // printf("Sending MIDI\n");
    }
}

static void NotifyProc (const MIDINotification *message, void *refCon)
{
    CMMMConnection *thisOne = (__bridge CMMMConnection *) refCon;
    
    if ([thisOne HaveSourcesChanged] || [thisOne HaveDestinationsChanged])
    {
        [thisOne RefreshInput];
        
        [thisOne RefreshOutput];
    }

}

- (bool) HaveSourcesChanged
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
- (bool) HaveDestinationsChanged
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

// stores a list of current sources in sourceList and returns the number of sources
- (ItemCount) ListCurrentSources
{
    ItemCount sources = MIDIGetNumberOfSources(); // count the number of sources
    CFStringRef sourceName;
    [sourceList removeAllObjects];
    
    for (int nnn = 0; nnn < sources; ++nnn) // for each source
    {
        
        MIDIEndpointRef endpoint = MIDIGetSource(nnn);
        MIDIObjectGetStringProperty( endpoint, kMIDIPropertyName, &sourceName); // get name of endpoint
        NSString * pNSSourceName = (__bridge NSString *)sourceName;
        [sourceList insertObject: pNSSourceName atIndex: nnn];
        
    }
    // printf("Created list of sources\n");
    return sources;
}

// stores the current available destinations in destinationList array
- (ItemCount) ListCurrentDestinations
{
    ItemCount destinations = MIDIGetNumberOfDestinations();
    CFStringRef destinationName;
    
    [destinationList removeAllObjects];
    
    for (int nnn = 0; nnn < destinations; ++nnn)
    {
        
        MIDIEndpointRef endpoint = MIDIGetDestination(nnn);
        MIDIObjectGetStringProperty( endpoint, kMIDIPropertyName, &destinationName);
        NSString * pNSDestinationName = (__bridge  NSString *)destinationName;
        [destinationList insertObject: pNSDestinationName atIndex: nnn];
        
    }
    // printf("Created list of destinations\n");
    return destinations;
}

// queries whether the desiredSourceName is in the list and returns the index
// returns NSNotFound if not in list
- (NSUInteger) FindIndexOfDesiredSource
{
    NSString * sourceName = (__bridge NSString *) desiredSourceName;
    
    return [sourceList indexOfObject:sourceName];
    
}

// queries whether the desiredDestinationName is in the list and returns the index
// returns NSNotFound if not in list
- (NSUInteger) FindIndexOfDesiredDestination
{
    NSString * destName = (__bridge NSString *) desiredDestinationName;
    
    return [destinationList indexOfObject:destName];
}

// connects a source at an index to the inputPort
- (void) ConnectInputs:(NSUInteger) indexOfDesiredSource
{
    if (indexOfDesiredSource != NSNotFound)
    {
        MIDIEndpointRef inputDev = MIDIGetSource((int) indexOfDesiredSource); // more elegant cast here?
        MIDIPortConnectSource(inputPort, inputDev, NULL);
        isSourceConnected = true;
        [[self delegate] ConnectionStatusChanged: self]; // delegate

        
    }
    else
    {
        isSourceConnected = false;
        [[self delegate] ConnectionStatusChanged: self]; // delegate

    }
}

- (void) ConnectOutputs:(NSUInteger) indexOfDesiredDestination
{
    if (indexOfDesiredDestination != NSNotFound)
    {
        MIDIEndpointRef endpoint= MIDIGetDestination(indexOfDesiredDestination);
        self->outputDevice = endpoint;
        isDestinationConnected = true;
        [[self delegate] ConnectionStatusChanged: self]; // delegate


    }
    else
    {
        isDestinationConnected = false;
        [[self delegate] ConnectionStatusChanged: self]; // delegate


    }
    
}

- (void) RefreshInput
{
    NSUInteger indexOfDesiredSource = NSNotFound;
    gSources = [self ListCurrentSources];
    indexOfDesiredSource = [self FindIndexOfDesiredSource];
    [self ConnectInputs:indexOfDesiredSource];
}


- (void) RefreshOutput
{
    NSUInteger indexOfDesiredDestination = NSNotFound;
    gDestinations = [self ListCurrentDestinations];
    indexOfDesiredDestination = [self FindIndexOfDesiredDestination];
    [self ConnectOutputs:indexOfDesiredDestination];
}

- (id) CreateMMMConnection:(int)number ToSource:(NSString *)sName andDestination:(NSString *)dName
{
    instanceNumber = number;
    instanceName = CFStringCreateWithFormat(NULL, NULL, CFSTR("MIDI Middle Man %i"), self.instanceNumber);
    
    desiredSourceName = (__bridge CFStringRef) sName;
    desiredDestinationName = (__bridge CFStringRef) dName;
    
    isSourceConnected = false;
    isDestinationConnected = false;
    
    sourceList = [NSMutableArray array];
    destinationList = [NSMutableArray array];
    
    void* classPointer = (__bridge void *)self;
    
    // create client
    MIDIClientCreate(
                     instanceName,
                     NotifyProc,
                     classPointer,
                     &(client));

    // create input port
    MIDIInputPortCreate(
                        client,
                        instanceName,
                        InputReadProc,
                        &(source),
                        &(inputPort));
    
    // create output port
    MIDIOutputPortCreate(
                         client,
                         instanceName,
                         &(outputPort));

    // create source
    MIDISourceCreate(
                     client,
                     instanceName,
                     &(source));
    
    // create destination
    MIDIDestinationCreate(
                          client,
                          instanceName,
                          DestinationReadProc,
                          classPointer,
                          &(destination));

    // connect it all up
    [self RefreshInput];
    [self RefreshOutput];
        
    return self;
}


@end
