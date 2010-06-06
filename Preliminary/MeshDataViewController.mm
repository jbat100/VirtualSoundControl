//
//  MeshInterfaceManager.m
//  MeshFrames
//
//  Created by Jonathan Thorpe on 06/03/2010.
//  Copyright 2010 JBAT. All rights reserved.
//

#import "MeshDataViewController.h"
#import "JBAT_BaseMesh.h"




@implementation MeshDataViewController

@synthesize dataSource;

#pragma mark -
#pragma mark View Methods

-(void) awakeFromNib {
	
	
	
}

-(void) dealloc {

	[super dealloc];
}



-(void) setup {
	
	[meshVerticesTableView setDelegate:self];
	[meshVerticesTableView setDataSource:self];
	
	[meshNormalsTableView setDelegate:self];
	[meshNormalsTableView setDataSource: self];
	
	[meshPolysTableView setDelegate:self];
	[meshPolysTableView setDataSource:self];
	
}


#pragma mark -
#pragma mark Interface Methods


-(IBAction)	updateTablesClicked:(id)sender {
	
	[self updateTables];
	
}

-(void) updateTables {
	
	[meshVerticesTableView reloadData];
	[meshNormalsTableView reloadData];
	[meshPolysTableView reloadData];
	
}





#pragma mark -
#pragma mark Table View Delegate Methods

- (NSInteger)numberOfRowsInTableView:(NSTableView *)aTableView {
	
	if (dataSource.modelMesh) {
		if (aTableView == meshVerticesTableView) {
			//NSLog(@"Returning %d rows for vertex table",modelMesh->getVertexCount());
			return dataSource.modelMesh->getVertexCount();
		}
		else if (aTableView == meshNormalsTableView) {
			//NSLog(@"Returning %d rows for normals table",modelMesh->getVertexCount());
			return dataSource.modelMesh->getVertexCount();
		}
		else if (aTableView == meshPolysTableView) {
			//NSLog(@"Returning %d rows for poly table",modelMesh->getCellCount());
			return dataSource.modelMesh->getCellCount();
		}
	}

	return 0;
}

- (id)tableView:(NSTableView *)tableView objectValueForTableColumn:(NSTableColumn *)tableColumn row:(int)row {
	
	NSLog(@"column identifier is %@", [tableColumn identifier]);
	
	
	if (dataSource.modelMesh) {
		if (tableView == meshVerticesTableView) {
			
			M3DVector3f coordinates;
			dataSource.modelMesh->copyVertexCoordinates(row, coordinates);
			
			if ([[tableColumn identifier] isEqualToString:@"X"]) {
				return [NSString stringWithFormat:@"%f", coordinates[0]];
			}
			
			else if ([[tableColumn identifier] isEqualToString:@"Y"]) {
				return [NSString stringWithFormat:@"%f", coordinates[1]];
			}
			
			else if ([[tableColumn identifier] isEqualToString:@"Z"]) {
				return [NSString stringWithFormat:@"%f", coordinates[2]];
			}

		}
		else if (tableView == meshNormalsTableView) {
			
			M3DVector3f normals;
			dataSource.modelMesh->copyVertexNormals(row, normals);
			
			if ([[tableColumn identifier] isEqualToString:@"X"]) {
				return [NSString stringWithFormat:@"%f", normals[0]];
			}
			
			else if ([[tableColumn identifier] isEqualToString:@"Y"]) {
				return [NSString stringWithFormat:@"%f", normals[1]];
			}
			
			else if ([[tableColumn identifier] isEqualToString:@"Z"]) {
				return [NSString stringWithFormat:@"%f", normals[2]];
			}
			
		}
		else if (tableView == meshPolysTableView) {
			
			int n1, n2, n3;
			dataSource.modelMesh->copyTriangleNodeIndexes(row, n1, n2, n3);
			
			if ([[tableColumn identifier] isEqualToString:@"Node 1 id"]) {
				return [NSString stringWithFormat:@"%d", n1];
			}
			
			else if ([[tableColumn identifier] isEqualToString:@"Node 2 id"]) {
				return [NSString stringWithFormat:@"%d", n2];
			}
			
			else if ([[tableColumn identifier] isEqualToString:@"Node 3 id"]) {
				return [NSString stringWithFormat:@"%d", n3];
			}

		}
	}
	
	return 0;
}

@end
