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


@interface MeshWindowController : NSWindowController <NSWindowDelegate, NSTabViewDelegate, 
MeshDataViewControllerDataSource, MeshDisplayViewControllerDataSource> {
	
	
	IBOutlet NSTextField *meshFilePathTextField;
	IBOutlet NSButton *importMeshButton;
	
	IBOutlet NSTabView *meshTabView;
	
	MeshDataViewController *meshDataViewController;
	MeshDisplayViewController *meshDisplayViewController;
	
	JBAT_Object *modelMesh;
	NSString *meshFilePath;
	
	BOOL rendering;
	
}

@property JBAT_Object *modelMesh;

-(IBAction)	importMeshClicked:(id)sender;


@end
