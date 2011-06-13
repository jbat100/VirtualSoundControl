//
//  VSCOpenGLView.h
//  BulletCocoaTest
//
//  Created by Jonathan Thorpe on 21/04/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#include "VSCBaseApplication.h"
#include "VSCControlSetup.h"

@protocol VSCOpenGLViewDelegate
-(VSCRootApplication*)rootApplication;
@end



@interface VSCOpenGLView : NSOpenGLView {
	
	NSTimer* drawTimer;
	NSDate* lastDrawDate;
	id<VSCOpenGLViewDelegate> delegate;

}

@property (nonatomic, retain) NSDate* lastDrawDate;

@property (assign) id<VSCOpenGLViewDelegate> delegate;

-(void) mouseDown:(NSEvent *)theEvent withButton:(VSCMouseButton)b;
-(void) mouseUp:(NSEvent *)theEvent withButton:(VSCMouseButton)b;
-(void) mouseDragged:(NSEvent *)theEvent withButton:(VSCMouseButton)b;

-(void) startTimer;
-(void) setup;
-(void) prepareOpenGL;
-(void) reshape;

@end
