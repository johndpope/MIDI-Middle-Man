//
//  CMMMConnection.h
//  MIDI Middle Man
//
//  Created by Joe Noel on 27/02/2013.
//  Copyright (c) 2013 Focusrite Audio Engineering Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreMIDI/CoreMIDI.h>
#include <CoreFoundation/CoreFoundation.h>
#import <CoreFoundation/CFString.h>

@interface CMMMConnection : NSObject
{
    MIDIClientRef     client;
    CFStringRef       clientName;
    MIDIPortRef       inputPort, outputPort;
    MIDIEndpointRef   source, destination;
    MIDIEndpointRef   inputDevice, outputDevice;
    
    ItemCount         gSources, gDestinations;
    NSMutableArray    *sourceList, *destinationList;
    
    bool              isSourceConnected, isDestinationConnected;
}

// name and number for the instance
@property int instanceNumber;
@property CFStringRef instanceName;

// name of desired source and destination from external device (e.g. Launchkey MIDI)
@property CFStringRef       desiredSourceName, desiredDestinationName;

// create MMM connections with desired source
- (id)  CreateMMMConnection:(int)number ToSource:(NSString *)desiredSourceName andDestination:(NSString *)desiredDestinationName;
- (id)  CreateMMMConnection:(int)number ToSource:(NSString *)desiredSourceName;
- (id)  CreateMMMConnection:(int)number ToDestination:(NSString *)desiredDestinationName;


@end