#include <CoreMIDI/MIDIServices.h>
#include <CoreFoundation/CFRunLoop.h>
#include <stdio.h>
#import <Cocoa/Cocoa.h>

#import "CMMMConnection.h"

#define DESIRED_SOURCE_NAME_1 "Launchkey MIDI"
#define DESIRED_DESTINATION_NAME_1 "Launchkey MIDI"
#define DESIRED_SOURCE_NAME_2 "Launchpad S"
#define DESIRED_DESTINATION_NAME_2 "Launchpad S"


int main(int argc, const char * argv[])
{
    
    CMMMConnection *firstMMM = [[CMMMConnection alloc] init];
    CMMMConnection *secondMMM = [[CMMMConnection alloc] init];
    
    [firstMMM CreateMMMConnection:1 ToSource:@DESIRED_SOURCE_NAME_1 andDestination:@DESIRED_DESTINATION_NAME_1];
    [secondMMM CreateMMMConnection:2 ToSource:@DESIRED_SOURCE_NAME_2 andDestination:@DESIRED_DESTINATION_NAME_2];
    
    CFRunLoopRun();
    
    return 0;
}



