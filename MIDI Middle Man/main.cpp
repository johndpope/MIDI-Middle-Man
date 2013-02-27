#include <CoreMIDI/MIDIServices.h>
#include <CoreFoundation/CFRunLoop.h>
#include <stdio.h>
#import <Cocoa/Cocoa.h>

#import "CMMMConnection.h"

#define DESIRED_SOURCE_NAME "Launchpad S"
#define DESIRED_DESTINATION_NAME "Launchpad S"


int main(int argc, const char * argv[])
{
    
    CMMMConnection *firstMMM;
    [firstMMM CreateMMMConnection:1 ToSource:@"Launchpad S" andDestination:@"Launchpad S"];
    
    CFRunLoopRun();

    
    
    return 0;
}



