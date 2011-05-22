//
//  VSCDocument.h
//  BulletCocoaTest
//
//  Created by Jonathan Thorpe on 21/04/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "btBulletDynamicsCommon.h"
#import "VSCOpenGLView.h"
#include "GLDebugDrawer.h"
class VSCBaseApplication;


@interface VSCDocument : NSDocument <VSCOpenGLViewDelegate> {
	
	VSCOpenGLView* vscOpenGLView;
	VSCBaseApplication* baseApplication;

	GLDebugDrawer sDebugDraw;
	
}

@property (retain, nonatomic) IBOutlet VSCOpenGLView* vscOpenGLView;

-(VSCBaseApplication*) baseApplication;

@end
