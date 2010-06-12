//
//  MeshInterfaceManager.m
//  MeshFrames
//
//  Created by Jonathan Thorpe on 06/03/2010.
//  Copyright 2010 JBAT. All rights reserved.
//

#import "MeshDataViewController.h"
#import "JBAT_BaseMesh.h"


#define COORDINATES_KEY				@"Vertex Coordinates"
#define NORMALS_KEY					@"Vertex Normals"
#define TEXTURE_COORDINATES_KEY		@"Vertex Texture Coordinates"
#define COLORS_KEY					@"Vertex Colors"
#define TRIANGLE_INDECES			@"Triangle Indeces"



@interface MeshDataViewController () // private methods

@end


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
	
	[mainTableView setDelegate:self];
	[mainTableView setDataSource:self];
	[mainTableView setUsesAlternatingRowBackgroundColors:YES];
	
	[tableInputPopUpButton setPullsDown:NO];
	[tableInputPopUpButton removeAllItems];
	[tableInputPopUpButton addItemsWithTitles:[NSArray arrayWithObjects:
											   COORDINATES_KEY,
											   NORMALS_KEY,
											   TEXTURE_COORDINATES_KEY,
											   COLORS_KEY,
											   TRIANGLE_INDECES, nil]];
	
	[tableInputPopUpButton selectItemWithTitle:COORDINATES_KEY];
	[tableInputPopUpButton setTarget:self];
	[tableInputPopUpButton setAction:@selector(updateTable)];
	
	[self updateTable];
	
	
}


#pragma mark -
#pragma mark Interface Methods



-(IBAction) updateTableClicked:(id)sender {
	
	[self updateTable];
	
}

-(void) updateTable {
	
	NSString *selectedPopUpItemTitle = [tableInputPopUpButton titleOfSelectedItem];
	
	if ([selectedPopUpItemTitle isEqualToString:COORDINATES_KEY]) {
		[self setNumberOfColumnsTo:3];
		NSArray *columns = [mainTableView tableColumns];
		[[(NSTableColumn*)[columns objectAtIndex:0] headerCell] setStringValue:@"X"];
		[[(NSTableColumn*)[columns objectAtIndex:1] headerCell] setStringValue:@"Y"];
		[[(NSTableColumn*)[columns objectAtIndex:2] headerCell] setStringValue:@"Z"];
	}
	else if ([selectedPopUpItemTitle isEqualToString:NORMALS_KEY]) {
		[self setNumberOfColumnsTo:3];
		NSArray *columns = [mainTableView tableColumns];
		[[(NSTableColumn*)[columns objectAtIndex:0] headerCell] setStringValue:@"X"];
		[[(NSTableColumn*)[columns objectAtIndex:1] headerCell] setStringValue:@"Y"];
		[[(NSTableColumn*)[columns objectAtIndex:2] headerCell] setStringValue:@"Z"];
	}
	else if ([selectedPopUpItemTitle isEqualToString:TEXTURE_COORDINATES_KEY]) {
		[self setNumberOfColumnsTo:2];
		NSArray *columns = [mainTableView tableColumns];
		[[(NSTableColumn*)[columns objectAtIndex:0] headerCell] setStringValue:@"X"];
		[[(NSTableColumn*)[columns objectAtIndex:1] headerCell] setStringValue:@"Y"];
	}
	else if ([selectedPopUpItemTitle isEqualToString:COLORS_KEY]) {
		[self setNumberOfColumnsTo:4];
		NSArray *columns = [mainTableView tableColumns];
		[[(NSTableColumn*)[columns objectAtIndex:0] headerCell] setStringValue:@"R"];
		[[(NSTableColumn*)[columns objectAtIndex:1] headerCell] setStringValue:@"G"];
		[[(NSTableColumn*)[columns objectAtIndex:2] headerCell] setStringValue:@"B"];
		[[(NSTableColumn*)[columns objectAtIndex:3] headerCell] setStringValue:@"A"];
	}
	else if ([selectedPopUpItemTitle isEqualToString:TRIANGLE_INDECES]) {
		[self setNumberOfColumnsTo:3];
		NSArray *columns = [mainTableView tableColumns];
		[[(NSTableColumn*)[columns objectAtIndex:0] headerCell] setStringValue:@"1"];
		[[(NSTableColumn*)[columns objectAtIndex:1] headerCell] setStringValue:@"2"];
		[[(NSTableColumn*)[columns objectAtIndex:2] headerCell] setStringValue:@"3"];
	}
	
	NSArray *columns = [mainTableView tableColumns];
	
	//NSLog(@"At A, columns is %@", columns);
	
	[mainTableView reloadData];
	
}

-(void) setNumberOfColumnsTo:(NSInteger)n {
	
	NSArray *columns = [mainTableView tableColumns];
	
	//NSLog(@"Setting number of columns to %d", n);
	
	if (n == [columns count]) {
		return;
	}
	
	else if (n > [columns count]) {
		
		for (int i = [columns count]; i < n; i++) {
			NSTableColumn* c = [[NSTableColumn alloc] initWithIdentifier:[NSString stringWithFormat:@"%d", i+1]];
			[mainTableView addTableColumn:[c autorelease]];
		}
		
	}
	
	else if (n < [columns count]) {
		
		for (int i = [columns count] - 1; i > n - 1; i--) {
			NSTableColumn *c = [columns objectAtIndex:i];
			[mainTableView removeTableColumn:c];
		}
		 
	}

	//NSLog(@"Table view frame is %@", NSStringFromRect(mainTableView.frame));
	for (int i = 0; i < [columns count]; i++) {
		
		[[columns objectAtIndex:i] setWidth:mainTableView.frame.size.width/(float)n];
	}
	
	//NSLog(@"After empty columns is %@", columns);

}



#pragma mark -
#pragma mark Table View Delegate Methods


- (NSInteger)numberOfRowsInTableView:(NSTableView *)aTableView {
	
	NSString *selectedPopUpItemTitle = [tableInputPopUpButton titleOfSelectedItem];
	
	if (dataSource.modelMesh) {
		if ([selectedPopUpItemTitle isEqualToString:COORDINATES_KEY]			||
			[selectedPopUpItemTitle isEqualToString:NORMALS_KEY]				||
			[selectedPopUpItemTitle isEqualToString:TEXTURE_COORDINATES_KEY]	||
			[selectedPopUpItemTitle isEqualToString:COLORS_KEY]) {
			//NSLog(@"Returning %d rows for vertex table",modelMesh->getVertexCount());
			return dataSource.modelMesh->getVertexCount();
		}
		else if ([selectedPopUpItemTitle isEqualToString:TRIANGLE_INDECES]) {
			//NSLog(@"Returning %d rows for poly table",modelMesh->getCellCount());
			return dataSource.modelMesh->getCellCount();
		}
	}

	return 0;
}

- (id)tableView:(NSTableView *)tableView objectValueForTableColumn:(NSTableColumn *)tableColumn row:(int)row {
	
	//NSLog(@"column identifier is %@", [tableColumn identifier]);
	
	
	if (dataSource.modelMesh) {
		
		if ([[tableInputPopUpButton titleOfSelectedItem] isEqualToString:COORDINATES_KEY]) {
			if ([[tableColumn identifier] isEqualToString:@"1"]) {
				return [NSNumber numberWithFloat:dataSource.modelMesh->getVertexCoordinates(row)[0]];
			}
			else if ([[tableColumn identifier] isEqualToString:@"2"]) {
				return [NSNumber numberWithFloat:dataSource.modelMesh->getVertexCoordinates(row)[1]];
			}
			else if ([[tableColumn identifier] isEqualToString:@"3"]) {
				return [NSNumber numberWithFloat:dataSource.modelMesh->getVertexCoordinates(row)[2]];
			}
		}
		
		else if ([[tableInputPopUpButton titleOfSelectedItem] isEqualToString:NORMALS_KEY]) {
			if ([[tableColumn identifier] isEqualToString:@"1"]) {
				return [NSNumber numberWithFloat:dataSource.modelMesh->getVertexNormals(row)[0]];
			}
			else if ([[tableColumn identifier] isEqualToString:@"2"]) {
				return [NSNumber numberWithFloat:dataSource.modelMesh->getVertexNormals(row)[1]];
			}
			else if ([[tableColumn identifier] isEqualToString:@"3"]) {
				return [NSNumber numberWithFloat:dataSource.modelMesh->getVertexNormals(row)[2]];
			}
		}
		
		else if ([[tableInputPopUpButton titleOfSelectedItem] isEqualToString:TEXTURE_COORDINATES_KEY]) {
			if ([[tableColumn identifier] isEqualToString:@"1"]) {
				return [NSNumber numberWithFloat:dataSource.modelMesh->getVertexTextureCoordinates(row)[0]];
			}
			else if ([[tableColumn identifier] isEqualToString:@"2"]) {
				return [NSNumber numberWithFloat:dataSource.modelMesh->getVertexTextureCoordinates(row)[0]];
			}
		}
		
		else if ([[tableInputPopUpButton titleOfSelectedItem] isEqualToString:COLORS_KEY]) {
			if ([[tableColumn identifier] isEqualToString:@"1"]) {
				return [NSNumber numberWithFloat:dataSource.modelMesh->getVertexColors(row)[0]];
			}
			else if ([[tableColumn identifier] isEqualToString:@"2"]) {
				return [NSNumber numberWithFloat:dataSource.modelMesh->getVertexColors(row)[1]];
			}
			else if ([[tableColumn identifier] isEqualToString:@"3"]) {
				return [NSNumber numberWithFloat:dataSource.modelMesh->getVertexColors(row)[2]];
			}
			else if ([[tableColumn identifier] isEqualToString:@"4"]) {
				return [NSNumber numberWithFloat:dataSource.modelMesh->getVertexColors(row)[3]];
			}
		}
		
		else if ([[tableInputPopUpButton titleOfSelectedItem] isEqualToString:TRIANGLE_INDECES]) {
			int n1, n2, n3;
			dataSource.modelMesh->copyTriangleNodeIndexes(row, n1, n2, n3);
			if ([[tableColumn identifier] isEqualToString:@"1"]) {
				return [NSString stringWithFormat:@"%d", n1];
			}
			else if ([[tableColumn identifier] isEqualToString:@"2"]) {
				return [NSString stringWithFormat:@"%d", n2];
			}
			else if ([[tableColumn identifier] isEqualToString:@"3"]) {
				return [NSString stringWithFormat:@"%d", n3];
			}
		}
		

	}
	
	return 0;
}

@end
