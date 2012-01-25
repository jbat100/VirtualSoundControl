//
//  VSCOpenGLView.h
//  BulletCocoaTest
//
//  Created by Jonathan Thorpe on 21/04/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "VSCDocumentProtocol.h"
#include "VSCRootApplication.h"
#include "VSCControlSetup.h"


@interface VSCOpenGLView : NSOpenGLView {
	
	NSTimer* drawTimer;
	NSDate* lastDrawDate;
	id<VSCDocumentProtocol> delegate;

}

@property (nonatomic, retain) NSDate* lastDrawDate;

@property (assign) id<VSCDocumentProtocol> delegate;

-(void) mouseDown:(NSEvent *)theEvent withButton:(VSCMouseButton)b;
-(void) mouseUp:(NSEvent *)theEvent withButton:(VSCMouseButton)b;
-(void) mouseDragged:(NSEvent *)theEvent withButton:(VSCMouseButton)b;

-(void) startTimer;
-(void) setup;
-(void) prepareOpenGL;
-(void) reshape;

@end
