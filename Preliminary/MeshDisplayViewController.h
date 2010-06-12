//
//  MeshDisplayView.h
//  MeshFrames3
//
//  Created by Jonathan Thorpe on 28/03/2010.
//  Copyright 2010 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "BaseGLView.h"
#import "JBAT_BaseMesh.h"
#import "JBAT_Camera.h"

@protocol MeshDisplayViewControllerDataSource

-(JBAT_BaseMesh *) modelMesh;

@end

@interface MeshDisplayViewController : NSViewController {
	
	BaseGLView *glView;
	id<MeshDisplayViewControllerDataSource> dataSource;
	
	JBAT_Camera *camera;

}

@property (assign) BaseGLView *glView;
@property (assign) id<MeshDisplayViewControllerDataSource> dataSource;
@property (nonatomic) JBAT_Camera *camera;

-(void) setup;

@end
