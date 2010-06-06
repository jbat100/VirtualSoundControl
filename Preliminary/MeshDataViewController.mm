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

-(void) emptyTableColums;

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
	
	[tableInputPopUpButton setPullsDown:NO];
	
	[tableInputPopUpButton addItemsWithTitles:[NSArray arrayWithObjects:
											   COORDINATES_KEY,
											   NORMALS_KEY,
											   TEXTURE_COORDINATES_KEY,
											   COLORS_KEY,
											   TRIANGLE_INDECES, nil]];
	
	[tableInputPopUpButton selectItemWithTitle:COORDINATES_KEY];
	[tableInputPopUpButton setTarget:self];
	[tableInputPopUpButton setAction:@selector(updateTable)];
	
	tableColumns = [[NSMutableDictionary alloc] initWithCapacity:12];
	
	NSTableColumn *column;
	NSMutableArray *columns;
	NSUInteger resizingMask = NSTableColumnAutoresizingMask;
	NSTextAlignment alignment = NSCenterTextAlignment;
	
	
	columns = [[[NSMutableArray alloc] initWithCapacity:3] autorelease];
	column = [[[NSTableColumn alloc] initWithIdentifier:@"X"] autorelease];
	[[column headerCell] setStringValue:@"X"];
	//[[column headerCell] setTextAlignment:alignment];
	[column setResizingMask:resizingMask];
	[columns addObject:column];
	column = [[[NSTableColumn alloc] initWithIdentifier:@"Y"] autorelease];
	[[column headerCell] setStringValue:@"Y"];
	//[[column headerCell] setTextAlignment:alignment];
	[column setResizingMask:resizingMask];
	[columns addObject:column];
	column = [[[NSTableColumn alloc] initWithIdentifier:@"Z"] autorelease];
	[[column headerCell] setStringValue:@"Z"];
	//[[column headerCell] setTextAlignment:alignment];
	[column setResizingMask:resizingMask];
	[columns addObject:column];
	[tableColumns setObject:[NSArray arrayWithArray:columns] forKey:COORDINATES_KEY];
	
	columns = [[[NSMutableArray alloc] initWithCapacity:3] autorelease];
	column = [[[NSTableColumn alloc] initWithIdentifier:@"X"] autorelease];
	[[column headerCell] setStringValue:@"X"];
	//[[column headerCell] setTextAlignment:alignment];
	[column setResizingMask:resizingMask];
	[columns addObject:column];
	column = [[[NSTableColumn alloc] initWithIdentifier:@"Y"] autorelease];
	[[column headerCell] setStringValue:@"Y"];
	//[[column headerCell] setTextAlignment:alignment];
	[column setResizingMask:resizingMask];
	[columns addObject:column];
	column = [[[NSTableColumn alloc] initWithIdentifier:@"Z"] autorelease];
	[[column headerCell] setStringValue:@"Z"];
	//[[column headerCell] setTextAlignment:alignment];
	[column setResizingMask:resizingMask];
	[columns addObject:column];
	[tableColumns setObject:[NSArray arrayWithArray:columns] forKey:NORMALS_KEY];
	
	columns = [[[NSMutableArray alloc] initWithCapacity:2] autorelease];
	column = [[[NSTableColumn alloc] initWithIdentifier:@"X"] autorelease];
	[[column headerCell] setStringValue:@"X"];
	//[[column headerCell] setTextAlignment:alignment];
	[column setResizingMask:resizingMask];
	[columns addObject:column];
	column = [[[NSTableColumn alloc] initWithIdentifier:@"Y"] autorelease];
	[[column headerCell] setStringValue:@"Y"];
	//[[column headerCell] setTextAlignment:alignment];
	[column setResizingMask:resizingMask];
	[columns addObject:column];
	[tableColumns setObject:[NSArray arrayWithArray:columns] forKey:TEXTURE_COORDINATES_KEY];
	
	columns = [[[NSMutableArray alloc] initWithCapacity:2] autorelease];
	column = [[[NSTableColumn alloc] initWithIdentifier:@"R"] autorelease];
	[[column headerCell] setStringValue:@"R"];
	//[[column headerCell] setTextAlignment:alignment];
	[column setResizingMask:resizingMask];
	[columns addObject:column];
	column = [[[NSTableColumn alloc] initWithIdentifier:@"G"] autorelease];
	[[column headerCell] setStringValue:@"G"];
	//[[column headerCell] setTextAlignment:alignment];
	[column setResizingMask:resizingMask];
	[columns addObject:column];
	column = [[[NSTableColumn alloc] initWithIdentifier:@"B"] autorelease];
	[[column headerCell] setStringValue:@"B"];
	//[[column headerCell] setTextAlignment:alignment];
	[column setResizingMask:resizingMask];
	[columns addObject:column];
	column = [[[NSTableColumn alloc] initWithIdentifier:@"A"] autorelease];
	[[column headerCell] setStringValue:@"A"];
	//[[column headerCell] setTextAlignment:alignment];
	[column setResizingMask:resizingMask];
	[columns addObject:column];
	[tableColumns setObject:[NSArray arrayWithArray:columns] forKey:COLORS_KEY];
	
	columns = [[[NSMutableArray alloc] initWithCapacity:3] autorelease];
	column = [[[NSTableColumn alloc] initWithIdentifier:@"1"] autorelease];
	[[column headerCell] setStringValue:@"Index 1"];
	//[[column headerCell] setTextAlignment:alignment];
	[column setResizingMask:resizingMask];
	[columns addObject:column];
	column = [[[NSTableColumn alloc] initWithIdentifier:@"2"] autorelease];
	[[column headerCell] setStringValue:@"Index 2"];
	//[[column headerCell] setTextAlignment:alignment];
	[column setResizingMask:resizingMask];
	[columns addObject:column];
	column = [[[NSTableColumn alloc] initWithIdentifier:@"3"] autorelease];
	[[column headerCell] setStringValue:@"Index 3"];
	//[[column headerCell] setTextAlignment:alignment];
	[column setResizingMask:resizingMask];
	[columns addObject:column];
	[tableColumns setObject:[NSArray arrayWithArray:columns] forKey:TRIANGLE_INDECES];
	
	
}


#pragma mark -
#pragma mark Interface Methods



-(IBAction) updateTableClicked:(id)sender {
	
	[self updateTable];
	
}

-(void) updateTable {
	
	NSString *selectedPopUpItemTitle = [tableInputPopUpButton titleOfSelectedItem];
	[self emptyTableColums];
	
	NSArray *columns = [tableColumns objectForKey:selectedPopUpItemTitle];
	
	NSLog(@"tableColumns is %@, Columns is %@", tableColumns, columns);
	
	CGFloat width = mainTableView.frame.size.width;
	
	for (NSInteger i = 0; i < [columns count]; i++) {
		[[columns objectAtIndex:i] setWidth:(width / [columns count])];
		[mainTableView addTableColumn:[columns objectAtIndex:i]];
	}
	
	[mainTableView reloadData];
	
}

-(void) emptyTableColums {
	
	NSArray *columns = [mainTableView tableColumns];
	
	NSLog(@"Before empty columns is %@", columns);
	
	
	for (NSInteger i = 0; i < [columns count]; i++) {
		[mainTableView removeTableColumn:[columns objectAtIndex:i]];
	}
	
	columns = [mainTableView tableColumns];
	
	NSLog(@"After	empty columns is %@", columns);

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
			if ([[tableColumn identifier] isEqualToString:@"X"]) {
				return [NSNumber numberWithFloat:dataSource.modelMesh->getVertexCoordinates(row)[0]];
			}
			else if ([[tableColumn identifier] isEqualToString:@"Y"]) {
				return [NSNumber numberWithFloat:dataSource.modelMesh->getVertexCoordinates(row)[1]];
			}
			else if ([[tableColumn identifier] isEqualToString:@"Z"]) {
				return [NSNumber numberWithFloat:dataSource.modelMesh->getVertexCoordinates(row)[2]];
			}
		}
		
		else if ([[tableInputPopUpButton titleOfSelectedItem] isEqualToString:NORMALS_KEY]) {
			if ([[tableColumn identifier] isEqualToString:@"X"]) {
				return [NSNumber numberWithFloat:dataSource.modelMesh->getVertexNormals(row)[0]];
			}
			else if ([[tableColumn identifier] isEqualToString:@"Y"]) {
				return [NSNumber numberWithFloat:dataSource.modelMesh->getVertexNormals(row)[1]];
			}
			else if ([[tableColumn identifier] isEqualToString:@"Z"]) {
				return [NSNumber numberWithFloat:dataSource.modelMesh->getVertexNormals(row)[2]];
			}
		}
		
		else if ([[tableInputPopUpButton titleOfSelectedItem] isEqualToString:TEXTURE_COORDINATES_KEY]) {
			if ([[tableColumn identifier] isEqualToString:@"X"]) {
				return [NSNumber numberWithFloat:dataSource.modelMesh->getVertexTextureCoordinates(row)[0]];
			}
			else if ([[tableColumn identifier] isEqualToString:@"Y"]) {
				return [NSNumber numberWithFloat:dataSource.modelMesh->getVertexTextureCoordinates(row)[0]];
			}
		}
		
		else if ([[tableInputPopUpButton titleOfSelectedItem] isEqualToString:COLORS_KEY]) {
			if ([[tableColumn identifier] isEqualToString:@"R"]) {
				return [NSNumber numberWithFloat:dataSource.modelMesh->getVertexColors(row)[0]];
			}
			else if ([[tableColumn identifier] isEqualToString:@"G"]) {
				return [NSNumber numberWithFloat:dataSource.modelMesh->getVertexColors(row)[1]];
			}
			else if ([[tableColumn identifier] isEqualToString:@"B"]) {
				return [NSNumber numberWithFloat:dataSource.modelMesh->getVertexColors(row)[2]];
			}
			else if ([[tableColumn identifier] isEqualToString:@"A"]) {
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
			else if ([[tableColumn identifier] isEqualToString:@"Node 3 id"]) {
				return [NSString stringWithFormat:@"%d", n3];
			}
		}
		

	}
	
	return 0;
}

@end
