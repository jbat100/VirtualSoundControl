//
//  VSCOpenGLView.h
//  BulletCocoaTest
//
//  Created by Jonathan Thorpe on 21/04/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#include "VSCBaseApplication.h"

@protocol VSCOpenGLViewDelegate

-(VSCBaseApplication*)basicDemo;

@end



@interface VSCOpenGLView : NSOpenGLView {
	
	NSTimer* drawTimer;

	NSDate* lastDrawDate;
	
	id<VSCOpenGLViewDelegate> delegate;

}

@property (nonatomic, retain) NSDate* lastDrawDate;

@property (assign) id<VSCOpenGLViewDelegate> delegate;

-(void) startTimer;
-(void) setup;
-(void) prepareOpenGL;
-(void) reshape;

@end
