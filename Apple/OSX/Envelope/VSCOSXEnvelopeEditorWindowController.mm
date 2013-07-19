//
//  VCSEnvelopeEditorWindowController.m
//  EnvelopeEditor
//
//  Created by Jonathan Thorpe on 11/09/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import "VSCOSXEnvelopeEditorWindowController.h"
#import "VSC::EnvelopeEditorDocument.h"

#import <string>
#import <list>
#include <boost/serialization/shared_ptr.hpp>

/*
#define FILE_MENU_TITLE								@"File"
#define LOAD_ENVELOPE_MENU_ITEM_TITLE				@"Load..."
#define LOAD_RECENT_ENVELOPE_MENU_ITEM_TITLE		@"Load Recent"
#define SAVE_ENVELOPE_MENU_ITEM_TITLE				@"Save"
#define SAVE_ENVELOPE_AS_MENU_ITEM_TITLE			@"Save As..."
*/
 
@interface VSCOSXEnvelopeEditorWindowController ()

/*
@property (nonatomic) NSMenu* fileMenu;
@property (nonatomic) NSMenuItem* fileTitleMenuItem;

-(void) updateEnvelopePopUpMenu;
-(void) checkInterface;

-(void) saveEnvelopeAs:(id)sender; 
 */

@end


@implementation VSCOSXEnvelopeEditorWindowController

//@synthesize envelopeView, envelopeEditorView;
@synthesize envelopeController = _envelopeController;
//@synthesize loadedTextField, envelopePopUpButton, testButton, fileMenu, fileTitleMenuItem;

#pragma mark - NSWindowController Window Callbacks

-(void) windowDidLoad {
    
	NSLog(@"%@ window did load", self);
    
    [self.window setDelegate:self];
    
    
    /*
    CALayer *viewLayer = [CALayer layer];
    [viewLayer setBackgroundColor:CGColorCreateGenericRGB(0.0, 0.5, 0.0, 0.4)]; //RGB plus Alpha Channel
    [viewLayer setDelegate:envelopeView];
    */
     
    /*
     * We don't want to call [self setWantsLayer:YES]; BEFORE setLayer as we want a layer hosting view, not a layer backed view
     
     APPLE DOC:
     
     A layer-backed view is a view that is backed by a Core Animation layer. 
     Any drawing done by the view is cached in the backing layer. You configure a layer-backed view by invoking setWantsLayer: 
     with a value of YES. The view class automatically creates a backing layer for you (using makeBackingLayer if overridden), and 
     you must use the view class’s drawing mechanisms. When using layer-backed views you should never interact directly with the layer. 
     Instead you must use the standard view programming practices.
     
     */
    
    //[self setWantsLayer:YES]; // view's backing store is using a Core Animation Layer
    
    
    //[envelopeView setLayer:viewLayer];
    
    /*
     * We DO want to call [self setWantsLayer:YES]; AFTER setLayer
     *
     
     APPLE DOC:
     
     A layer-hosting view is a view that contains a Core Animation layer that you intend to manipulate directly. 
     You create a layer-hosting view by instantiating a Core Animation layer class and supplying that layer to the 
     view’s setLayer: method. After doing so, you then invoke setWantsLayer: with a value of YES. This method order 
     is crucial. When using a layer-hosting view you should not rely on the view for drawing, nor should you add 
     subviews to the layer-hosting view. The root layer (the layer set using setLayer:) should be treated as the 
     root layer of the layer tree and you should only use Core Animation drawing and animation methods. You still 
     use the view for handling mouse and keyboard events, but any resulting drawing must be handled by Core Animation.
     
     */
    
    //[envelopeView setWantsLayer:YES];
    //[envelopeView setupEnvelopeLayer];
    //[viewLayer setNeedsDisplay];
    
    
}

-(void) windowWillLoad {
	NSLog(@"%@ window will load", self);
}

- (void)windowDidResize:(NSNotification *)notification {
    NSLog(@"windowDidResize %@", notification);

}


#pragma mark - Convenience 

-(VSC::EnvelopeEditorDocument*) envelopeEditorDocument {
	return (VSC::EnvelopeEditorDocument*)[self document];
}

/*

-(void) checkInterface {
	
	if (!currentEnvelope) {
		[[fileMenu itemWithTitle:SAVE_ENVELOPE_MENU_ITEM_TITLE] setEnabled:NO];
		[[fileMenu itemWithTitle:SAVE_ENVELOPE_AS_MENU_ITEM_TITLE] setEnabled:NO];
	}
	else {
		[[fileMenu itemWithTitle:SAVE_ENVELOPE_MENU_ITEM_TITLE] setEnabled:YES];
		[[fileMenu itemWithTitle:SAVE_ENVELOPE_AS_MENU_ITEM_TITLE] setEnabled:YES];
	}
	
	[envelopePopUpButton selectItem:fileTitleMenuItem];
	
}
 
 */


#pragma mark - Interface Update 

/*

-(void) updateEnvelopePopUpMenu {
	
	self.fileMenu = [[NSMenu alloc] initWithTitle:FILE_MENU_TITLE];
	
	self.fileTitleMenuItem = [[NSMenuItem alloc] initWithTitle:FILE_MENU_TITLE
														action:NULL 
												 keyEquivalent:@""];
	[fileMenu addItem:fileTitleMenuItem];
	
	NSMenuItem* loadItem = [[NSMenuItem alloc] initWithTitle:LOAD_ENVELOPE_MENU_ITEM_TITLE
													  action:@selector(loadEnvelope:) 
											   keyEquivalent:@""];
	[fileMenu addItem:loadItem];
	
	NSMenuItem* loadRecentItem = [[NSMenuItem alloc] initWithTitle:LOAD_RECENT_ENVELOPE_MENU_ITEM_TITLE
													  action:nil
											   keyEquivalent:@""];
	[fileMenu addItem:loadRecentItem];
	
	
	[fileMenu addItem:[NSMenuItem separatorItem]];
	
	NSMenuItem* saveItem = [[NSMenuItem alloc] initWithTitle:SAVE_ENVELOPE_MENU_ITEM_TITLE
													  action:@selector(saveEnvelope:) 
											   keyEquivalent:@""];
	
	[fileMenu addItem:saveItem];
	
	NSMenuItem* saveAsItem = [[NSMenuItem alloc] initWithTitle:SAVE_ENVELOPE_AS_MENU_ITEM_TITLE
													  action:@selector(saveEnvelopeAs:) 
											   keyEquivalent:@""];
	
	[fileMenu addItem:saveAsItem];
	
	[envelopePopUpButton setMenu:fileMenu];
	
}
 
 */

/*

- (void)launchFileOrFolder:(id)sender
{
	NSString *filePath = [sender representedObject];
	[[NSWorkspace sharedWorkspace] openFile:filePath];
}
 
 */


@end
