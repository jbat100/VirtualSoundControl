//
//  MeshDisplayView.h
//  MeshFrames3
//
//  Created by Jonathan Thorpe on 28/03/2010.
//  Copyright 2010 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "BaseDisplayViewController.h"
#import "JBAT_BaseMesh.h"


@protocol MeshDisplayViewControllerDataSource

-(JBAT_BaseMesh *) modelMesh;

@end

@interface MeshDisplayViewController : BaseDisplayViewController {
	
	id<MeshDisplayViewControllerDataSource> dataSource;

}

@property (assign) id<MeshDisplayViewControllerDataSource> dataSource;

@end
