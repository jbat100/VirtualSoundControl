//
//  VSCOgreBulletWindow.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 7/12/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCOgreBulletWindow.h"
#import "VSCOgreView.h"

@implementation VSCOgreBulletWindow

@synthesize ogreView;

-(BOOL)acceptsFirstResponder {
    return YES;
}

- (void)keyDown:(NSEvent *)theEvent {
    NSLog(@"%@ keyDown %@", self, theEvent);
}

- (void)keyUp:(NSEvent *)theEvent {
    NSLog(@"%@ keyUp %@", self, theEvent);
}


- (IBAction)test1:(id)sender {
    BOOL accepts = [self.ogreView acceptsFirstResponder];
    NSLog(@"VSCOgreView %@ accept first responder", accepts ? @"DOES" : @"DOES NOT");
    
    if (accepts) {
        BOOL isFirstResponder = [self firstResponder] != self.ogreView;
        NSLog(@"VSCOgreView %@ first responder", isFirstResponder ? @"IS" : @"IS NOT");
    }
}

- (IBAction)test2:(id)sender {
    BOOL isFirstResponder = [self firstResponder] != self.ogreView;
    if (isFirstResponder) {
        [self.ogreView resignFirstResponder];
    }
    else {
        [self.ogreView becomeFirstResponder];
    }
    isFirstResponder = [self firstResponder] != self.ogreView;
     NSLog(@"VSCOgreView %@ first responder", isFirstResponder ? @"IS" : @"IS NOT");
}

- (IBAction)test3:(id)sender {
    
}
                                 
                                 

@end
