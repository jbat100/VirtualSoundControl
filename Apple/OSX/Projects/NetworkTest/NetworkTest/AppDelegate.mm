//
//  AppDelegate.m
//  NetworkTest
//
//  Created by Jonathan Thorpe on 9/18/13.
//  Copyright (c) 2013 Jonathan Thorpe. All rights reserved.
//

#import "AppDelegate.h"

#import "VSCOSXASIOTester.h"

@interface AppDelegate ()

@property (nonatomic, strong) IBOutlet VSCOSXASIOTester* asioTester;

@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    // Insert code here to initialize your application
    
    //self.asioTester = [[VSCOSXASIOTester alloc] init];
}

-(IBAction)test:(id)sender
{
    NSLog(@"Hello, I want to do a test");
}

@end
