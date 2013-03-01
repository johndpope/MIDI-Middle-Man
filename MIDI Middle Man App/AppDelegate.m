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
@synthesize destinationLabel_1;
@synthesize sourceLabel_2;
@synthesize destinationLabel_2;

@synthesize sourceTextField1, sourceTextField2, destinationTextField1, destinationTextField2;

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    firstMMM = [[CMMMConnection alloc] init];
    [firstMMM setDelegate:self];
    secondMMM = [[CMMMConnection alloc] init];
    [secondMMM setDelegate:self];
    
    [firstMMM CreateMMMConnection:1 ToSource:@DESIRED_SOURCE_NAME_1 andDestination:@DESIRED_DESTINATION_NAME_1];
    [secondMMM CreateMMMConnection:2 ToSource:@DESIRED_SOURCE_NAME_2 andDestination:@DESIRED_DESTINATION_NAME_2];
    
    [sourceTextField1 setStringValue:@DESIRED_SOURCE_NAME_1];
    [sourceTextField2 setStringValue:@DESIRED_SOURCE_NAME_2];
    [destinationTextField1 setStringValue:@DESIRED_DESTINATION_NAME_1];
    [destinationTextField2 setStringValue:@DESIRED_DESTINATION_NAME_2];

    
}

- (void) ConnectionStatusChanged: (id) whereChanged
{

if ([whereChanged isSourceConnected])
    {
        switch ([whereChanged instanceNumber])
        {
            case 1:
                [sourceLabel_1 setTextColor:[NSColor greenColor]];
                break;
            case 2:
                [sourceLabel_2 setTextColor:[NSColor greenColor]];
            default:
                break;
        }

    }
   else
    {
        switch ([whereChanged instanceNumber])
        {
            case 1:
                [sourceLabel_1 setTextColor:[NSColor redColor]];
                break;
            case 2:
                [sourceLabel_2 setTextColor:[NSColor redColor]];
            default:
                break;
        }
    }

if ([whereChanged isDestinationConnected])
    {
        switch ([whereChanged instanceNumber])
        {
            case 1:
                [destinationLabel_1 setTextColor:[NSColor greenColor]];
                break;
            case 2:
                [destinationLabel_2 setTextColor:[NSColor greenColor]];
            default:
                break;
        }
        
    }
    else
    {
        switch ([whereChanged instanceNumber])
        {
            case 1:
                [destinationLabel_1 setTextColor:[NSColor redColor]];
                break;
            case 2:
                [destinationLabel_2 setTextColor:[NSColor redColor]];
            default:
                break;
        }
    }
    
}

- (IBAction)sourceTextField1A:(NSTextField *)sender
{
    NSString * name = [sender stringValue];
    if (name)
    {
        [firstMMM ChangeSourceNameTo:name];
    }
}

- (IBAction)destinationTextField:(NSTextField *)sender
{
    NSString * name = [sender stringValue];
    if (name)
    {
        [firstMMM ChangeDestinationNameTo:name];
    }
}

- (IBAction)sourceTextField2:(NSTextField *)sender
{
    NSString * name = [sender stringValue];
    if (name)
    {
        [secondMMM ChangeSourceNameTo:name];
    }
}

- (IBAction)destinationTextField2:(NSTextField *)sender
{
    NSString * name = [sender stringValue];
    if (name)
    {
        [secondMMM ChangeDestinationNameTo:name];
    }
}
@end
