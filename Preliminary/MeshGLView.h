//
//  MeshGLView.h
//  MeshFrames3
//
//  Created by Jonathan Thorpe on 20/06/2010.
//  Copyright 2010 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "BaseGLView.h"
#import "JBAT_BufferedMesh.h"

@protocol MeshGLViewEnvironment

-(JBAT_BufferedMesh*) mesh;

@end



@interface MeshGLView : BaseGLView {
	
	id<MeshGLViewEnvironment> meshEnvironment;

}

@property (assign) id<MeshGLViewEnvironment> meshEnvironment;

@end
