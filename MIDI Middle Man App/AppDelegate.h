//
//  AppDelegate.h
//  MIDI Middle Man App
//
//  Created by Joe Noel on 28/02/2013.
//  Copyright (c) 2013 Focusrite Audio Engineering Ltd. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#include "CMMMConnection.h"

#define DESIRED_SOURCE_NAME_1 "Launchkey MIDI"
#define DESIRED_DESTINATION_NAME_1 "Port 1"
#define DESIRED_SOURCE_NAME_2 "Launchpad S"
#define DESIRED_DESTINATION_NAME_2 "Launchpad S"


@interface AppDelegate : NSObject <NSApplicationDelegate, ConnectionStatusDelegate>
{
CMMMConnection *firstMMM;
CMMMConnection *secondMMM;
}
@property (weak) IBOutlet NSTextField *sourceLabel_1;
@property (weak) IBOutlet NSTextField *destinationLabel_1;
@property (weak) IBOutlet NSTextField *sourceLabel_2;
@property (weak) IBOutlet NSTextField *destinationLabel_2;



@property (assign) IBOutlet NSWindow *window;

@end
