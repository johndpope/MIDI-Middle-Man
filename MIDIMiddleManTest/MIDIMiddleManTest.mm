//
//  MIDIMiddleManTest.m
//  MIDIMiddleManTest
//
//  Created by Joe Noel on 27/02/2013.
//  Copyright (c) 2013 Focusrite Audio Engineering Ltd. All rights reserved.
//

#import "MIDIMiddleManTest.hh"
#import "MIDIMiddleMan.hpp"
#include <Cocoa/Cocoa.h>


@implementation MIDIMiddleManTest

- (void)setUp
{
    [super setUp];
    
    // Set-up code here.
}

- (void)tearDown
{
    // Tear-down code here.
    
    [super tearDown];
}

- (void)testFindIndexOfDesiredSource_FalsePositive
{
    CFStringRef desiredSourceName = CFSTR("Hi");

    // Create array without desiredSourceName
    NSMutableArray* sourceList = [NSMutableArray array];
    
    [sourceList insertObject:@"0" atIndex:0];
    [sourceList insertObject:@"1" atIndex:1];
    [sourceList insertObject:@"2" atIndex:2];
    [sourceList insertObject:@"3" atIndex:3];

   
    NSUInteger res = -1;
    
    // Test function
    res = FindIndexOfDesiredSource(sourceList,desiredSourceName);

    if (res != NSNotFound) // If function finds something
    {
        STFail(@"FindIndexOfDesiredSource - False positive\n");
    }

}

- (void)testFindIndexOfDesiredSource_FalseNegative
{
    CFStringRef desiredSourceName = CFSTR("Hi");
    
    // Create array with desiredSourceName
    NSMutableArray* sourceList = [NSMutableArray array];
    
    [sourceList insertObject:@"0" atIndex:0];
    [sourceList insertObject:@"1" atIndex:1];
    [sourceList insertObject:@"2" atIndex:2];
    [sourceList insertObject:@"Hi" atIndex:3];
    
    
    NSUInteger res = -1;
    
    // Test function
    res = FindIndexOfDesiredSource(sourceList,desiredSourceName);
    
    if (res != 3) // If function doesn't find something
    {
        STFail(@"FindIndexOfDesiredSource - False negative\n");
    }
    
}


@end
