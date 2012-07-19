//
//  VSCOgreView.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 6/22/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCOgreView.h"

#include "VSCOgreBulletCocoaInputAdapter.h"
#include <boost/assert.hpp>

@implementation VSCOgreView

static const bool mTraceUI = false;

@synthesize inputAdapter = _inputAdapter;

- (id)initWithFrame:(NSRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code here.
    }
    
    return self;
}

#pragma mark - NSResponder Stuff

-(BOOL)acceptsFirstResponder {
    return YES;
}

-(BOOL)acceptsFirstMouse:(NSEvent *)theEvent {
    return YES;
}

#pragma mark - Keyboard

- (void)keyDown:(NSEvent *)theEvent {
    if (mTraceUI) NSLog(@"%@ keyDown: %@", self, theEvent);
    BOOST_ASSERT_MSG(self.inputAdapter, "expected input adapter");
    self.inputAdapter->keyDown(theEvent); 
}

- (void)keyUp:(NSEvent *)theEvent {
    if (mTraceUI) NSLog(@"%@ keyUp: %@", self, theEvent);
    self.inputAdapter->keyUp(theEvent); 
}

#pragma mark - Mouse

- (void)mouseEntered:(NSEvent *)theEvent {
    if (mTraceUI) NSLog(@"%@ mouseEntered: %@", self, theEvent);
    self.inputAdapter->mouseEntered(theEvent); 
}

- (void)mouseExited:(NSEvent *)theEvent {
    if (mTraceUI) NSLog(@"%@ mouseExited: %@", self, theEvent);
    self.inputAdapter->mouseExited(theEvent); 
}

- (void)mouseMoved:(NSEvent *)theEvent {
    if (mTraceUI) NSLog(@"%@ mouseMoved: %@", self, theEvent);
    self.inputAdapter->mouseMoved(theEvent); 
}

#pragma mark Left Mouse Button

- (void)mouseDown:(NSEvent *)theEvent {
    if (mTraceUI) NSLog(@"%@ mouseDown: %@", self, theEvent);
    self.inputAdapter->mouseDown(theEvent);
}

- (void)mouseUp:(NSEvent *)theEvent {
    if (mTraceUI) NSLog(@"%@ mouseUp: %@", self, theEvent);
    self.inputAdapter->mouseUp(theEvent);
}

- (void)mouseDragged:(NSEvent *)theEvent {
    if (mTraceUI) NSLog(@"%@ mouseDragged: %@", self, theEvent);
    self.inputAdapter->mouseDragged(theEvent);
}

#pragma mark Right Mouse Button

- (void)rightMouseDown:(NSEvent *)theEvent {
    if (mTraceUI) NSLog(@"%@ rightMouseDown: %@", self, theEvent);
    self.inputAdapter->rightMouseDown(theEvent);
}

- (void)rightMouseUp:(NSEvent *)theEvent {
    if (mTraceUI) NSLog(@"%@ rightMouseUp: %@", self, theEvent);
    self.inputAdapter->rightMouseUp(theEvent);
}

- (void)rightMouseDragged:(NSEvent *)theEvent {
    if (mTraceUI) NSLog(@"%@ rightMouseDragged: %@", self, theEvent);
    self.inputAdapter->rightMouseDragged(theEvent);
}

#pragma mark Other Mouse Button

- (void)otherMouseDown:(NSEvent *)theEvent {
    if (mTraceUI) NSLog(@"%@ otherMouseDown: %@", self, theEvent);
    self.inputAdapter->otherMouseDown(theEvent);
}

- (void)otherMouseUp:(NSEvent *)theEvent {
    if (mTraceUI) NSLog(@"%@ otherMouseUp: %@", self, theEvent);
    self.inputAdapter->otherMouseUp(theEvent);
}

- (void)otherMouseDragged:(NSEvent *)theEvent {
    if (mTraceUI) NSLog(@"%@ otherMouseDragged: %@", self, theEvent);
    self.inputAdapter->otherMouseDragged(theEvent);
}

#pragma mark Scroll Wheel

- (void) scrollWheel:(NSEvent *)theEvent {
    if (mTraceUI) NSLog(@"%@ scrollWheel: %@", self, theEvent);
    self.inputAdapter->scrollWheel(theEvent);
}



@end
