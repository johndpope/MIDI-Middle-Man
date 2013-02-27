#include <CoreMIDI/MIDIServices.h>
#include <CoreFoundation/CFRunLoop.h>
#include <stdio.h>
#import <Cocoa/Cocoa.h>

#import "CMMMConnection.h"

#define DESIRED_SOURCE_NAME "Launchpad S"
#define DESIRED_DESTINATION_NAME "Launchpad S"


int main(int argc, const char * argv[])
{
    
    CMMMConnection *firstMMM = [[CMMMConnection alloc] init];
    CMMMConnection *secondMMM = [[CMMMConnection alloc] init];
    
    [firstMMM CreateMMMConnection:1 ToSource:@"Port 1" andDestination:@"Port 1"];
    [secondMMM CreateMMMConnection:2 ToSource:@"Hi" andDestination:@"Hi"];
    
    CFRunLoopRun();
    
    return 0;
}



