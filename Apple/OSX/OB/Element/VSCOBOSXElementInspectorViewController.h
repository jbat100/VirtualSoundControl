//
//  VSCOBOSXElementInspectorViewController.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 11/27/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#import "VSCOBOSXElementController.h"
#import "VSCOBOSXSceneListener.h"

@class DMTabBar;
@class VSCOBOSXElementDetailView;
@class VSCOBOSXElementEventChainsView;
@class VSCOBOSXElementColorView;
@class VSCIMOSXEventChainViewController;

@interface VSCOBOSXElementInspectorViewController : NSViewController
<VSCOBOSXElementController, VSCOBOSXSceneListenerTarget, NSTableViewDataSource, NSTableViewDelegate>

@property (strong) IBOutlet VSCOBOSXElementDetailView* elementDetailView;
@property (strong) IBOutlet VSCOBOSXElementEventChainsView* elementEventChainsView;
@property (strong) IBOutlet VSCOBOSXElementColorView* elementColorView;

@property (weak) IBOutlet DMTabBar* tabBar;

-(void) setupIfNecessary;

-(void) showElementDetailView;
-(void) showElementEventChainsView;
-(void) showElementColorView;

-(IBAction) immobilized:(id)sender;

-(IBAction) printVertexDeclarations:(id)sender;
-(IBAction) printIndeces:(id)sender;
-(IBAction) printVertices:(id)sender;
-(IBAction) printNormals:(id)sender;
-(IBAction) printDiffuseColors:(id)sender;
-(IBAction) printSpecularColors:(id)sender;
-(IBAction) printTextureCoordinates:(id)sender;
-(IBAction) randomizeDiffuseVertexColors:(id)sender;
-(IBAction) selectDiffuseColor:(id)sender;

-(void) reloadImmobilizedInterface;

@end
