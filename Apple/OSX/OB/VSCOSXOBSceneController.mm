//
//  VSCOBSceneElementListController.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/28/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCOSXOBSceneController.h"

#import "VSCOBOSXSceneDisplayView.h"
#import "VSCOSXOBSceneElementListView.h"
#import "VSCOSXOBSceneElementCell.h"

NSString* const VSCOSXOBSceneElementCellReuseIdentifier = @"VSCOSXOBSceneElementCellReuseIdentifier";

@implementation VSCOSXOBSceneController

@synthesize scene = _scene;
@synthesize environmentController = _environmentController;

-(void) dealoc
{
    NSLog(@"%@ DEALLOC", self);
}

#pragma mark - Custom Setter

-(void) setScene:(VSC::OB::Scene::WPtr)s
{
    BOOST_ASSERT(self.sceneListener);
    
    VSC::OB::Scene::SPtr oldScene = _scene.lock();
    if (oldScene && self.sceneListener)
    {
        oldScene->removeListener(self.sceneListener);
    }
    
    _scene = s;
    
    [self.sceneView setupControlChain];
    
    VSC::OB::Scene::SPtr newScene = _scene.lock();
    if (newScene && self.sceneListener)
    {
        oldScene->addListener(self.sceneListener);
    }
    
    [self.elementListView.listView reloadData];
}

#pragma mark - List View Delegate Methods

- (NSUInteger)numberOfRowsInListView: (PXListView*)aListView
{
    VSC::OB::Scene::SPtr s = self.scene.lock();
    
    if (s)
    {
        VSC::OB::Scene::Elements& elems = s->getElements();
        NSLog(@"%@ numberOfRowsInListView %ld", self, (NSUInteger)(elems.size()));
        return (NSUInteger)(elems.size());
    }
 
    NSLog(@"%@ numberOfRowsInListView NO SCENE", self);
    
	return 0;
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
    
    NSLog(@"%@ aListView %@ cellForRow %ld: %@", self, aListView, row, elementCell);
	
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
