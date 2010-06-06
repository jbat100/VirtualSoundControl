//
//  MeshInterfaceManager.h
//  MeshFrames
//
//  Created by Jonathan Thorpe on 06/03/2010.
//  Copyright 2010 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "JBAT_BaseMesh.h"

@protocol MeshDataViewControllerDataSource

-(JBAT_BaseMesh *) modelMesh;

@end



@interface MeshDataViewController : NSViewController <NSTableViewDelegate, NSTableViewDataSource> {
	
	IBOutlet NSButton *updateTablesButton;
	
	IBOutlet NSTableView *meshVerticesTableView;
	IBOutlet NSTableView *meshNormalsTableView;
	IBOutlet NSTableView *meshPolysTableView;
	
	id<MeshDataViewControllerDataSource> dataSource;


}

@property (assign) id<MeshDataViewControllerDataSource> dataSource;

-(IBAction)	updateTablesClicked:(id)sender;
-(void) updateTables;

-(void) setup;

@end
