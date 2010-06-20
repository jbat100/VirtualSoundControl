//
//  MeshDisplayView.h
//  MeshFrames3
//
//  Created by Jonathan Thorpe on 28/03/2010.
//  Copyright 2010 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "BaseDisplayViewController.h"
#import "JBAT_BufferedMesh.h"
#import "MeshGLView.h"

@protocol MeshDisplayViewControllerDataSource

-(JBAT_BufferedMesh *) modelMesh;

@end

@interface MeshDisplayViewController : BaseDisplayViewController <MeshDataSource> {
	
	id<MeshDisplayViewControllerDataSource> dataSource;

}

@property (assign) id<MeshDisplayViewControllerDataSource> dataSource;

@end
