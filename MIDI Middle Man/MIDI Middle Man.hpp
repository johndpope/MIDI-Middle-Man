//
//  MIDI Middle Man.h
//  MIDI Middle Man
//
//  Created by Joe on 22/02/2013.
//  Copyright (c) 2013 Focusrite Audio Engineering Ltd. All rights reserved.
//

#ifndef __MIDI_Middle_Man__MIDI_Middle_Man__
#define __MIDI_Middle_Man__MIDI_Middle_Man__

#include <iostream>

#include <CoreMIDI/MIDIServices.h>
#include <CoreFoundation/CFRunLoop.h>
#include <stdio.h>
#import <Cocoa/Cocoa.h>

#define DESIRED_SOURCE_NAME "Launchpad S"
#define DESIRED_DESTINATION_NAME "Launchpad S"

ItemCount           gSources, gDestinations;
NSMutableArray      *sourceList, *destinationList;

struct OutputPortDevicePair
{
    MIDIPortRef outputPort;
    MIDIEndpointRef outputDevice;
};

void	InputReadProc(const MIDIPacketList *pktlist, void * refCon, void * connRefCon);

void	DestinationReadProc(const MIDIPacketList *pktlist, void *refCon, void *connRefCon);


// stores a list of current sources in sourceList and returns the number of sources
ItemCount ListCurrentSources(NSMutableArray* sourceList);

// stores the current available destinations in destinationList array
ItemCount ListCurrentDestinations(NSMutableArray* destinationList);


// queries whether the desiredSourceName is in the list and returns the index
// returns NSNotFound if not in list
NSUInteger FindIndexOfDesiredSource(NSArray * sourceList, CFStringRef desiredSourceName);


// queries whether the desiredDestinationName is in the list and returns the index
// returns NSNotFound if not in list
NSUInteger FindIndexOfDesiredDestination(NSArray * destinationList, CFStringRef desiredDestinationName);

// connects a source at an index to the inputPort
void ConnectInputs(NSUInteger indexOfDesiredSource, MIDIPortRef inputPort);

void ConnectOutputs(NSUInteger indexOfDesiredDestination, OutputPortDevicePair* outputPortDevicePair);

// returns true if the number of sources have changed
bool HaveSourcesChanged();

// returns true if the number of destinations has changed
bool HaveDestinationsChanged();

// does something if the number of sources or destinations has changed
void NotifyProc (const MIDINotification *message, void *refCon);

#endif /* defined(__MIDI_Middle_Man__MIDI_Middle_Man__) */
