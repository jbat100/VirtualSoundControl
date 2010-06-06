//
//  MeshWindowController.h
//  MeshFrames3
//
//  Created by Jonathan Thorpe on 27/03/2010.
//  Copyright 2010 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#import "MeshDisplayViewController.h"
#import "MeshDataViewController.h"
#import "ViewComponentsControlWindowController.h"

#import "JBAT_Object.h"


@interface MeshWindowController : NSWindowController <NSTabViewDelegate, 
MeshDataViewControllerDataSource, MeshDisplayViewControllerDataSource> {
	
	
	IBOutlet NSTextField *meshFilePathTextField;
	IBOutlet NSButton *importMeshButton;
	IBOutlet NSButton *viewComponentsButton;
	
	IBOutlet NSTabView *meshTabView;
	
	MeshDataViewController *meshDataViewController;
	MeshDisplayViewController *meshDisplayViewController;
	
	ViewComponentsControlWindowController *viewComponentsControlWindowController;
	
	
	JBAT_Object *modelMesh;
	NSString *meshFilePath;
	
}

@property JBAT_Object *modelMesh;

-(IBAction)	importMeshClicked:(id)sender;
-(IBAction)	viewComponentsClicked:(id)sender;


@end
