//
//  VSCOBSceneElementListController.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/28/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCOSXOBSceneElementListController.h"

#import "VSCOSXOBSceneElementCell.h"

NSString* const CellIdentifier = @"VSCOSXOBSceneElementCell";

@implementation VSCOSXOBSceneElementListController

#pragma mark - List View Delegate Methods

- (NSUInteger)numberOfRowsInListView: (PXListView*)aListView
{
    VSC::OB::Scene::SPtr s = self.scene.lock();
    VSC::OB::Scene::Elements& elems = s->getElements();
    
	return (NSUInteger)(elems.size());
}

- (PXListViewCell*)listView:(PXListView*)aListView cellForRow:(NSUInteger)row
{
	VSCOSXOBSceneElementCell *cell = (VSCOSXOBSceneElementCell*)[self.listView dequeueCellWithReusableIdentifier:CellIdentifier];
	
	if(!cell)
    {
		cell = [VSCOSXOBSceneElementCell cellLoadedFromNibNamed:@"VSCOSXOBSceneElementCell" reusableIdentifier:CellIdentifier];
	}
    
    cell.environmentController = self.environmentController;
    
    VSC::OB::Scene::SPtr s = self.scene.lock();
    VSC::OB::Scene::Elements& elems = s->getElements();
    
    VSC::OB::Scene::Element::SPtr elem = elems.at(row);
    VSC::OB::Scene::Element::WPtr weakElem = VSC::OB::Scene::Element::WPtr(elem);
	
	// Set up the new cell:
	cell.element = weakElem;
	
	return cell;
}

- (CGFloat)listView:(PXListView*)aListView heightOfRow:(NSUInteger)row
{
	return 60;
}

- (void)listViewSelectionDidChange:(NSNotification*)aNotification
{
    NSLog(@"Selection changed");
}

@end
