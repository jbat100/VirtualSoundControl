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
	
	IBOutlet NSPopUpButton *tableInputPopUpButton;
	IBOutlet NSButton *updateTablesButton;
	
	IBOutlet NSTableView *mainTableView;
	
	NSView *waitingView;
	
	id<MeshDataViewControllerDataSource> dataSource;


}

@property (assign) id<MeshDataViewControllerDataSource> dataSource;

-(IBAction) updateTableClicked:(id)sender;
-(void) updateTable;
-(void) setup;
-(void) setNumberOfColumnsTo:(NSInteger)n;

-(void) showTableWaitingView;
-(void) hideTableWaitingView;

@end
