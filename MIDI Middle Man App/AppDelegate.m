//
//  AppDelegate.m
//  MIDI Middle Man App
//
//  Created by Joe Noel on 28/02/2013.
//  Copyright (c) 2013 Focusrite Audio Engineering Ltd. All rights reserved.
//

#import "AppDelegate.h"

@implementation AppDelegate

@synthesize sourceLabel_1;

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    firstMMM = [[CMMMConnection alloc] init];
    [firstMMM setDelegate:self];
    secondMMM = [[CMMMConnection alloc] init];
    [secondMMM setDelegate:self];
    
    [firstMMM CreateMMMConnection:1 ToSource:@DESIRED_SOURCE_NAME_1 andDestination:@DESIRED_DESTINATION_NAME_1];
    [secondMMM CreateMMMConnection:2 ToSource:@DESIRED_SOURCE_NAME_2 andDestination:@DESIRED_DESTINATION_NAME_2];
    
}

- (void) ConnectionStatusChanged: (id) whereChanged
{
   if ([whereChanged instanceNumber] == 1)
   {
    if ([whereChanged isSourceConnected])
    {
       
       [sourceLabel_1 setTextColor:[NSColor greenColor]];
    }
   else
    {
        [sourceLabel_1 setTextColor:[NSColor redColor]];
    }
   }

}

@end
