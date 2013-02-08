//
//  Fist Attenot.cpp
//  MIDI Middle Man
//
//  Created by Joe Noel on 08/02/2013.
//  Copyright (c) 2013 Focusrite Audio Engineering Ltd. All rights reserved.
//

#include "First Attempt.h"

/* First Attempt
 
 MIDIDeviceRef midiMiddleMan = NULL;
 // MIDISetupAddExternalDevice(* midiMiddleMan);
 
 CFStringRef deviceName CFSTR("MIDI Middle Man");
 CFStringRef manufacturerName CFSTR("Focusrite");
 CFStringRef modelName CFSTR("MIDI Middle Man v1.0");
 OSStatus result;
 
 try {
 result =  MIDIDeviceCreate (NULL,
 deviceName, // Name
 manufacturerName, // Manufacturer
 modelName, // Model
 *midiMiddleMan); // Pointer for new device
 throw result;
 }
 catch (OSStatus e)
 {
 printf("Error : %i", e);
 }
 
 ItemCount numSourceEndpoints = 1;
 ItemCount numDestinationEndpoints = 1;
 MIDIEntityRef newEntity = NULL;
 
 MIDIDeviceAddEntity(
 midiMiddleMan,
 deviceName,
 true,
 numSourceEndpoints,
 numDestinationEndpoints,
 newEntity) ;
 
 */