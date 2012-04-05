//
//  MeshWindowController.m
//  MeshFrames3
//
//  Created by Jonathan Thorpe on 27/03/2010.
//  Copyright 2010 JBAT. All rights reserved.
//

#import "MeshWindowController.h"
#import <string>

@implementation MeshWindowController

@synthesize modelMesh;

#pragma mark -
#pragma mark NSWindowController Methods


-(id) init {
	if ([super initWithWindowNibName:@"MeshWindow"]) {
	}
	return self;
}

-(void) windowDidLoad {
	
	[self.window setDelegate:self];
	
	rendering = NO;
	
	meshFilePath = [[NSString stringWithString:@"/Users/jonathanthorpe/Programming/Files/VTK/SphereOutput.vtk"] retain];
	modelMesh = nil;
	
	meshDataViewController = [[MeshDataViewController alloc] initWithNibName:@"MeshDataView" 
																	  bundle:[NSBundle mainBundle]];
	
	meshDisplayViewController = [[MeshDisplayViewController alloc] initWithNibName:@"MeshDisplayView" 
																			bundle:[NSBundle mainBundle]];
	
	[meshDataViewController setDataSource:self];
	[meshDisplayViewController setDataSource:self];
	
	[meshTabView setAllowsTruncatedLabels:NO];
	[meshTabView setControlTint:NSGraphiteControlTint];
	
	NSEnumerator *tabEnumerator = [[meshTabView tabViewItems] objectEnumerator];
	NSTabViewItem *tabViewItem;
	
	while (tabViewItem = (NSTabViewItem*)[tabEnumerator nextObject]) {
		
		if ([[tabViewItem identifier] isEqualToString:@"Mesh data"]) {
			[tabViewItem setView:[meshDataViewController view]];
		}

		if ([[tabViewItem identifier] isEqualToString:@"Display"]) {
			[tabViewItem setView:[meshDisplayViewController view]];
			
		}
		
	}
	
	[meshDataViewController setup];
	[meshDisplayViewController setup];
	
	// Load default mesh
	
//	std::string filename = [meshFilePath UTF8String];
//	modelMesh = new JBAT_Object(); 
//	modelMesh->readVTKFile(filename);

	
}
		 
-(void) dealloc {
	[meshFilePath release];
	delete modelMesh;
	[super dealloc];
}		


#pragma mark -
#pragma mark Interface Callback Methods

- (void)keyDown:(NSEvent *)theEvent {
	
	//NSLog(@"In MeshWindowController key down, event is %@", theEvent);
	[meshDisplayViewController keyDown:theEvent];
	
}

- (void)keyUp:(NSEvent *)theEvent {
	
	//NSLog(@"In MeshWindowController key up, event is %@", theEvent);
	[meshDisplayViewController keyUp:theEvent];
	
}

-(IBAction)	importMeshClicked:(id)sender {
	
	NSArray *fileTypes = [NSArray arrayWithObjects:@"vtk", @"vtp", nil];
	NSOpenPanel *oPanel = [NSOpenPanel openPanel];
	[oPanel setAllowsMultipleSelection:NO];
	int result = [oPanel runModalForDirectory:[meshFilePath stringByDeletingLastPathComponent]
										 file:nil types:fileTypes];
    if (result == NSOKButton) {
        NSArray *filesToOpen = [oPanel URLs];
		[meshFilePath release];
		meshFilePath = [[[filesToOpen objectAtIndex:0] path] retain];
		
		// CREATE MESH FROM FILE
		std::string filename = [meshFilePath UTF8String];
		
		if (modelMesh) {
			delete modelMesh;
		}
		
		modelMesh = new JBAT_Object(); 
		modelMesh->readVTKFile(filename);
		
		
		NSLog(@"Mesh has %d points and %d cells", modelMesh->getVertexCount(), modelMesh->getCellCount());
		
		[meshDataViewController updateTable];
		
    }
	
}



#pragma mark -
#pragma mark Interface Callback Methods

- (void)tabView:(NSTabView *)tabView didSelectTabViewItem:(NSTabViewItem *)tabViewItem {
	
}

- (void)windowDidResize:(NSNotification *)notification {
	
	NSLog(@"Window frame is %@", NSStringFromRect(self.window.frame));
	
}

@end