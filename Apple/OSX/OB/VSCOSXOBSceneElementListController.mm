//
//  VSCOBSceneElementListController.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/28/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCOSXOBSceneElementListController.h"

#import "VSCOSXOBSceneElementListView.h"
#import "VSCOSXOBSceneElementCell.h"

NSString* const VSCOSXOBSceneElementCellReuseIdentifier = @"VSCOSXOBSceneElementCellReuseIdentifier";

@implementation VSCOSXOBSceneElementListController

@synthesize environmentController = _environmentController;

#pragma mark - List View Delegate Methods

- (NSUInteger)numberOfRowsInListView: (PXListView*)aListView
{
    VSC::OB::Scene::SPtr s = self.scene.lock();
    VSC::OB::Scene::Elements& elems = s->getElements();
    
	return (NSUInteger)(elems.size());
}

- (PXListViewCell*)listView:(PXListView*)aListView cellForRow:(NSUInteger)row
{
    BOOST_ASSERT(aListView == self.elementListView.listView);
    if (aListView != self.elementListView.listView) return nil;
    
    PXListViewCell* cell = [aListView dequeueCellWithReusableIdentifier:VSCOSXOBSceneElementCellReuseIdentifier];
    
    BOOST_ASSERT([cell isKindOfClass:[VSCOSXOBSceneElementCell class]]);
    
	VSCOSXOBSceneElementCell *elementCell = (VSCOSXOBSceneElementCell*)cell;
	
	if(!elementCell)
    {
		elementCell = [VSCOSXOBSceneElementCell cellLoadedFromNibNamed:@"VSCOSXOBSceneElementCell"
                                                    reusableIdentifier:VSCOSXOBSceneElementCellReuseIdentifier];
	}
    
    elementCell.environmentController = self.environmentController;
    
    VSC::OB::Scene::SPtr s = self.scene.lock();
    VSC::OB::Scene::Elements& elems = s->getElements();
    
    VSC::OB::Scene::Element::SPtr elem = elems.at(row);
    VSC::OB::Scene::Element::WPtr weakElem = VSC::OB::Scene::Element::WPtr(elem);
	
	// Set up the new cell:
	elementCell.element = weakElem;
	
	return elementCell;
}

- (CGFloat)listView:(PXListView*)aListView heightOfRow:(NSUInteger)row
{
	return [VSCOSXOBSceneElementCell defaultViewHeight];
}

- (void)listViewSelectionDidChange:(NSNotification*)aNotification
{
    NSLog(@"%@ selection changed %@", self, aNotification);
}

@end
