//
//  LightsControlView.m
//  MeshFrames3
//
//  Created by Jonathan Thorpe on 05/04/2010.
//  Copyright 2010 JBAT. All rights reserved.
//

#import "LightsControlViewController.h"


@implementation LightsControlViewController

@synthesize dataSource, lightsTable;

- (void)awakeFromNib {
	
	NSLog(@"In LightsControlViewController awakeFromNib");
	
	activeLightIndex = -1;
	
	if (!lightControlViewController) {
		lightControlViewController = [[LightControlViewController alloc] initWithNibName:@"LightControlView" 
																				   bundle:[NSBundle mainBundle]];
	}
	
	[[lightControlViewController view] setFrame:[lightControlView frame]];
	[lightControlView addSubview:[lightControlViewController view]];
	[lightControlViewController setDataSource:self];
	[lightsTable setAllowsMultipleSelection:NO];
	 
//	NSLog(@"lightControlView frame is %@", NSStringFromRect(lightControlView.frame));
//	NSLog(@"[lightControlViewController view] frame is %@", NSStringFromRect([lightControlViewController view].frame));
	
}

-(void) setup {
	
	[lightsTable setDataSource:self];
	[lightsTable setDelegate:self];
	NSIndexSet *selectedSet = [NSIndexSet indexSetWithIndex:0];
	[lightsTable selectRowIndexes:selectedSet byExtendingSelection:NO];

}

-(void) updateInterface {
	
	[lightsTable reloadData];
	
	if ([self numberOfLights] < 1) {
		[addLightButton setEnabled:YES];
		[removeLightButton setEnabled:NO];
	}
	else if ([self numberOfLights] > 7) {
		[addLightButton setEnabled:NO];
		[removeLightButton setEnabled:YES];
	}
	else {
		[addLightButton setEnabled:YES];
		[removeLightButton setEnabled:YES];
	}

	
	if ([lightsTable selectedRow] < 0) {
		[removeLightButton setEnabled:NO];
	}
	
	[lightControlViewController updateInterface];

	
}


- (JBAT_Light*) light {
	
//	NSLog(@"Getting Light at selected row: %d, number of lights is %d", [lightsTable selectedRow], [dataSource numberOfLights]);
	
	if ([lightsTable selectedRow] < 0) {
		return NULL;
	}
	
	if ([lightsTable selectedRow] >= [self numberOfLights]) {
		NSLog(@"Tried to access out of bounds light");
		return NULL;
	}
	
	JBAT_Light *l = [self lightAtIndex:[lightsTable selectedRow]];
	
	return l;
	
}

-(void) addLightClicked:(id)sender {
	
	[self addLight];
	//NSLog(@"Added light, lights are:");
	//[dataSource printLightsToConsole];
	[lightsTable reloadData];
	
	// select the last (just added light)
	NSIndexSet *selectedSet = [NSIndexSet indexSetWithIndex:[self numberOfLights]-1];
	
	//NSLog(@"Setting selected row to %@", selectedSet);
	[lightsTable selectRowIndexes:selectedSet byExtendingSelection:NO];
	
	//NSLog(@"selected row is %d", [lightsTable selectedRow]);
	//NSLog(@"selected row indexes %@", [lightsTable selectedRowIndexes]);
	
	[self updateInterface];
	


}

-(void) removeLightClicked:(id)sender {
	NSInteger selectedRow = [lightsTable selectedRow];
	BOOL succeeded = [self removeLightAtIndex:selectedRow];
	
	[lightsTable reloadData];
	
	if (succeeded) {
		int nbOfLights = [self numberOfLights];
		if (selectedRow == 0 && nbOfLights > 0)
			[lightsTable selectRowIndexes:[NSIndexSet indexSetWithIndex:0] byExtendingSelection:NO];
		else if (selectedRow > 0 && nbOfLights > 0)
			[lightsTable selectRowIndexes:[NSIndexSet indexSetWithIndex:selectedRow-1] byExtendingSelection:NO];
	}
	
	[self updateInterface];
	
}

-(IBAction) printLightsButtonClicked:(id)sender {
	[self printLightsToConsole];
}

#pragma mark -
#pragma mark View Controller Methods
	 
- (NSInteger)numberOfRowsInTableView:(NSTableView *)aTableView {
	int n = [self numberOfLights];
	return n;
}

- (id)tableView:(NSTableView *)tableView objectValueForTableColumn:(NSTableColumn *)tableColumn row:(int)row {
	return [NSString stringWithFormat:@"Light %d", row+1];
}

- (BOOL)tableView:(NSTableView *)aTableView shouldSelectRow:(NSInteger)rowIndex {
	//NSLog(@" In - (BOOL)tableView:(NSTableView *)aTableView shouldSelectRow:(NSInteger)rowIndex, returning yes");
	return YES;
}

- (void)tableViewSelectionDidChange:(NSNotification *)aNotification {
	NSLog(@"Selected row");
	[lightControlViewController updateInterface];
}

#pragma mark -
#pragma mark Light Methods


-(JBAT_Light *) lightAtIndex:(NSInteger)index {
	return [dataSource lightSet]->getLightAtIndex(index);
}

-(NSInteger) numberOfLights {
	return [dataSource lightSet]->numberOfLights();
}

-(BOOL) addLight {
	bool success = [dataSource lightSet]->addLight();
	if (!success) {
		return NO;
	}
	return YES;
}

-(BOOL) removeLightAtIndex:(NSInteger)index {
	[dataSource lightSet]->removeLightAtIndex(index);
	return YES;
}

-(void) printLightsToConsole {
	std::cout << [dataSource lightSet];
}

	 
	 


@end
