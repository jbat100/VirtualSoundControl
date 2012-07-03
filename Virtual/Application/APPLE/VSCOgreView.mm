//
//  VSCOgreView.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 6/22/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCOgreView.h"

#include "VSCOgreBulletCocoaInputAdapter.h"

@implementation VSCOgreView

@synthesize inputAdapter = _inputAdapter;

- (id)initWithFrame:(NSRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code here.
    }
    
    return self;
}

#pragma mark Keyboard

- (void)keyDown:(NSEvent *)theEvent {
    self.inputAdapter->keyDown(theEvent); 
}

- (void)keyUp:(NSEvent *)theEvent {
    self.inputAdapter->keyUp(theEvent); 
}

#pragma mark Mouse

- (void)mouseEntered:(NSEvent *)theEvent {
    self.inputAdapter->mouseEntered(theEvent); 
}

- (void)mouseExited:(NSEvent *)theEvent {
    self.inputAdapter->mouseExited(theEvent); 
}

- (void)mouseMoved:(NSEvent *)theEvent {
    self.inputAdapter->mouseMoved(theEvent); 
}

#pragma mark Left Mouse Button

- (void)mouseDown:(NSEvent *)theEvent {
    self.inputAdapter->mouseDown(theEvent);
}

- (void)mouseUp:(NSEvent *)theEvent {
    self.inputAdapter->mouseUp(theEvent);
}

- (void)mouseDragged:(NSEvent *)theEvent {
    self.inputAdapter->mouseDragged(theEvent);
}

#pragma mark Right Mouse Button

- (void)rightMouseDown:(NSEvent *)theEvent {
    self.inputAdapter->rightMouseDown(theEvent);
}

- (void)rightMouseUp:(NSEvent *)theEvent {
    self.inputAdapter->rightMouseUp(theEvent);
}

- (void)rightMouseDragged:(NSEvent *)theEvent {
    self.inputAdapter->rightMouseDragged(theEvent);
}

#pragma mark Other Mouse Button

- (void)otherMouseDown:(NSEvent *)theEvent {
    self.inputAdapter->otherMouseDown(theEvent);
}

- (void)otherMouseUp:(NSEvent *)theEvent {
    self.inputAdapter->otherMouseUp(theEvent);
}

- (void)otherMouseDragged:(NSEvent *)theEvent {
    self.inputAdapter->otherMouseDragged(theEvent);
}

#pragma mark Scroll Wheel

- (void) scrollWheel:(NSEvent *)theEvent {
    self.inputAdapter->scrollWheel(theEvent);
}



@end
