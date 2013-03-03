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
#define DESIRED_DESTINATION_NAME_1 "Launchkey MIDI"
#define DESIRED_SOURCE_NAME_2 "Launchpad S"
#define DESIRED_DESTINATION_NAME_2 "Launchpad S"


@interface AppDelegate : NSObject <NSApplicationDelegate, ConnectionStatusDelegate>
{
CMMMConnection *firstMMM;
CMMMConnection *secondMMM;
}

// labels
@property (weak) IBOutlet NSTextField *sourceLabel_1;
@property (weak) IBOutlet NSTextField *destinationLabel_1;
@property (weak) IBOutlet NSTextField *sourceLabel_2;
@property (weak) IBOutlet NSTextField *destinationLabel_2;

// text field outlets
@property (weak) IBOutlet NSTextField *sourceTextField1;
@property (weak) IBOutlet NSTextField *destinationTextField1;
@property (weak) IBOutlet NSTextField *sourceTextField2;
@property (weak) IBOutlet NSTextField *destinationTextField2;

- (IBAction)comboBox:(id)sender;
@property (weak) IBOutlet NSComboBox *comboBox;

// text field actions
- (IBAction)sourceTextField1A:(NSTextField *)sender;
- (IBAction)destinationTextField:(NSTextField *)sender;
- (IBAction)sourceTextField2:(NSTextField *)sender;
- (IBAction)destinationTextField2:(NSTextField *)sender;

@property (assign) IBOutlet NSWindow *window;

@end
