//
//  VCSEnveloppeEditorWindowController.m
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 11/09/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import "VSCEnveloppeEditorWindowController.h"
#import "VSCEnveloppeEditorDocument.h"

#import <string>
#import <list>
#include <boost/serialization/shared_ptr.hpp>

#define FILE_MENU_TITLE								@"File"
#define LOAD_ENVELOPPE_MENU_ITEM_TITLE				@"Load..."
#define LOAD_RECENT_ENVELOPPE_MENU_ITEM_TITLE		@"Load Recent"
#define SAVE_ENVELOPPE_MENU_ITEM_TITLE				@"Save"
#define SAVE_ENVELOPPE_AS_MENU_ITEM_TITLE			@"Save As..."

@interface VSCEnveloppeEditorWindowController ()

@property (nonatomic) NSMenu* fileMenu;
@property (nonatomic) NSMenuItem* fileTitleMenuItem;

-(void) updateEnveloppePopUpMenu;
-(void) checkInterface;

-(void) saveEnveloppeAs:(id)sender;

@end


@implementation VSCEnveloppeEditorWindowController

//@synthesize enveloppeView, enveloppeEditorView;
@synthesize enveloppeEditorView;
@synthesize loadedTextField, enveloppePopUpButton, testButton, fileMenu, fileTitleMenuItem;

#pragma mark - NSWindowController Window Callbacks

-(void) windowDidLoad {
    
	NSLog(@"%@ window did load", self);
    
    [self.window setDelegate:self];
    
    if (![self getCurrentEnveloppe]) {
        
        [self setCurrentEnveloppe:VSCEnveloppe::createADSREnveloppe(0.5, 0.5, 1.0, 1.0)];
        
    }
    
    /*
    CALayer *viewLayer = [CALayer layer];
    [viewLayer setBackgroundColor:CGColorCreateGenericRGB(0.0, 0.5, 0.0, 0.4)]; //RGB plus Alpha Channel
    [viewLayer setDelegate:enveloppeView];
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
    
    
    //[enveloppeView setLayer:viewLayer];
    
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
    
    //[enveloppeView setWantsLayer:YES];
    //[enveloppeView setupEnveloppeLayer];
    //[viewLayer setNeedsDisplay];
    
    [self updateEnveloppePopUpMenu];
	[self checkInterface];
    
    assert(enveloppeEditorView);
    
    [enveloppeEditorView setupGraph];
    
    [self.enveloppeEditorView.mainEnveloppeView setEnveloppe:[self getCurrentEnveloppe]];
    
}

-(void) windowWillLoad {
	NSLog(@"%@ window will load", self);
}

- (void)windowDidResize:(NSNotification *)notification {
    
    NSLog(@"windowDidResize %@", notification);
    
    [self.enveloppeEditorView layoutEnveloppeView];
    
    //[self.enveloppeView redrawEnveloppeLayer];
    
}

#pragma mark - C++ Setters / Getters

-(VSCEnveloppePtr) getCurrentEnveloppe {
	return currentEnveloppe;
}

-(void) setCurrentEnveloppe:(VSCEnveloppePtr)enveloppe {
	currentEnveloppe = enveloppe;
	if (enveloppe)
		std::cout << "Current enveloppe set to " << *currentEnveloppe;
	else 
		std::cout << "Current enveloppe set none";
    
	//[enveloppeView setEnveloppe:enveloppe];
	[enveloppeEditorView.mainEnveloppeView setEnveloppe:enveloppe];
    
	[self checkInterface];
}

#pragma mark - Convenience 

-(VSCEnveloppeEditorDocument*) enveloppeEditorDocument {
	return (VSCEnveloppeEditorDocument*)[self document];
}

-(void) checkInterface {
	
	if (!currentEnveloppe) {
		[[fileMenu itemWithTitle:SAVE_ENVELOPPE_MENU_ITEM_TITLE] setEnabled:NO];
		[[fileMenu itemWithTitle:SAVE_ENVELOPPE_AS_MENU_ITEM_TITLE] setEnabled:NO];
	}
	else {
		[[fileMenu itemWithTitle:SAVE_ENVELOPPE_MENU_ITEM_TITLE] setEnabled:YES];
		[[fileMenu itemWithTitle:SAVE_ENVELOPPE_AS_MENU_ITEM_TITLE] setEnabled:YES];
	}
	
	[enveloppePopUpButton selectItem:fileTitleMenuItem];
	
	//[enveloppeView setNeedsDisplay:YES];
    [enveloppeEditorView setNeedsDisplay:YES];
	
}

#pragma mark - Button Callbacks

-(IBAction) testButtonClicked:(id)sender {
	NSLog(@"%@ window test button clicked", self);
    
	[self setCurrentEnveloppe:[[self enveloppeEditorDocument] defaultEnveloppe]];
    
    
    
}

#pragma mark - Interface Update 

-(void) updateEnveloppePopUpMenu {
	
	self.fileMenu = [[NSMenu alloc] initWithTitle:FILE_MENU_TITLE];
	
	self.fileTitleMenuItem = [[NSMenuItem alloc] initWithTitle:FILE_MENU_TITLE
														action:NULL 
												 keyEquivalent:@""];
	[fileMenu addItem:fileTitleMenuItem];
	
	NSMenuItem* loadItem = [[NSMenuItem alloc] initWithTitle:LOAD_ENVELOPPE_MENU_ITEM_TITLE
													  action:@selector(loadEnveloppe:) 
											   keyEquivalent:@""];
	[fileMenu addItem:loadItem];
	
	NSMenuItem* loadRecentItem = [[NSMenuItem alloc] initWithTitle:LOAD_RECENT_ENVELOPPE_MENU_ITEM_TITLE
													  action:nil
											   keyEquivalent:@""];
	[fileMenu addItem:loadRecentItem];
	
	
	[fileMenu addItem:[NSMenuItem separatorItem]];
	
	NSMenuItem* saveItem = [[NSMenuItem alloc] initWithTitle:SAVE_ENVELOPPE_MENU_ITEM_TITLE
													  action:@selector(saveEnveloppe:) 
											   keyEquivalent:@""];
	
	[fileMenu addItem:saveItem];
	
	NSMenuItem* saveAsItem = [[NSMenuItem alloc] initWithTitle:SAVE_ENVELOPPE_AS_MENU_ITEM_TITLE
													  action:@selector(saveEnveloppeAs:) 
											   keyEquivalent:@""];
	
	[fileMenu addItem:saveAsItem];
	
	[enveloppePopUpButton setMenu:fileMenu];
	
}

/*

- (void)launchFileOrFolder:(id)sender
{
	NSString *filePath = [sender representedObject];
	[[NSWorkspace sharedWorkspace] openFile:filePath];
}
 
 */

#pragma mark - MenuCallbacks Callbacks

-(void) loadEnveloppe:(id)sender {
	NSLog(@"Load Enveloppe");
	
	[enveloppePopUpButton selectItem:fileTitleMenuItem];
	
    NSArray *fileTypes = [NSArray arrayWithObject:
						  [NSString stringWithCString:ENVELOPPE_FILE_EXTENSION encoding:NSUTF8StringEncoding]];
    NSOpenPanel *oPanel = [NSOpenPanel openPanel];
    [oPanel setAllowsMultipleSelection:NO];
	
	NSString* defaultFilePath = @"";
	
	NSAssert([[self enveloppeEditorDocument] enveloppeBaseFilePath] > 0, @"Should have default enveloppe path");
	
	if ([[self enveloppeEditorDocument] enveloppeBaseFilePath] > 0) {
		defaultFilePath = [[self enveloppeEditorDocument] enveloppeBaseFilePath];
		//NSString* envPath = [dirPath stringByAppendingPathComponent:@"Enveloppe.vscxenv"];
	}
	
    int result = [oPanel runModalForDirectory:defaultFilePath file:nil types:fileTypes];
    if (result == NSOKButton) {
        NSArray *filesToOpen = [oPanel filenames];
		NSAssert([filesToOpen count] < 2, @"Cannot open multiple files");
        if ([filesToOpen count] == 1) {
			VSCEnveloppePtr newEnveloppe = VSCEnveloppePtr(new VSCEnveloppe());
			loadVSCEnveloppeFromXML(*newEnveloppe, [[filesToOpen objectAtIndex:0] UTF8String]);
			newEnveloppe->setFilePath([[filesToOpen objectAtIndex:0] UTF8String]);
			[self setCurrentEnveloppe:newEnveloppe];
		}
    }
}

-(void) saveEnveloppe:(id)sender {
	NSLog(@"Save Enveloppe");
	
	[enveloppePopUpButton selectItem:fileTitleMenuItem];
	
	NSAssert(currentEnveloppe, @"Should't receive save as command when currentEnveloppe is nil");
	
	NSString* filePath = [NSString stringWithCString:(currentEnveloppe->getFilePath()).c_str() encoding:NSUTF8StringEncoding];
	
	if ([filePath length] == 0) {
		[self saveEnveloppeAs:sender];
		return;
	}
	
	NSAssert([[filePath pathExtension] isEqualToString:
			  [NSString stringWithCString:ENVELOPPE_FILE_EXTENSION encoding:NSUTF8StringEncoding]], 
			 @"Wrong file path extension");
	
	NSLog(@"Saving enveloppe to %@", filePath);
	
	if ([[NSFileManager defaultManager] isWritableFileAtPath:filePath]) {
		saveVSCEnveloppeToXML(*currentEnveloppe, (const char *)[filePath UTF8String]);
	}
	else {
		[self saveEnveloppeAs:sender];
		return;
	}
	
	
}

-(void) saveEnveloppeAs:(id)sender {
	NSLog(@"Save Enveloppe As");
	
	[enveloppePopUpButton selectItem:fileTitleMenuItem];

	NSAssert(currentEnveloppe, @"Should't receive save command when currentEnveloppe is nil");
	
	if (!currentEnveloppe) {
		return;
	}
	
	/* create or get the shared instance of NSSavePanel */
	NSSavePanel* sp = [NSSavePanel savePanel];
	
	/* set up new attributes */
	//[sp setAccessoryView:newView];
	
    //[sp setRequiredFileType:[NSString stringWithCString:ENVELOPPE_FILE_EXTENSION encoding:NSUTF8StringEncoding]];
    
    sp.allowedFileTypes = [NSArray arrayWithObject:[NSString stringWithCString:ENVELOPPE_FILE_EXTENSION encoding:NSUTF8StringEncoding]];
	
	NSString* defaultFilePath = @"";
	
	NSAssert([[self enveloppeEditorDocument] enveloppeBaseFilePath] > 0, @"Should have default enveloppe path");
	
	if ([[self enveloppeEditorDocument] enveloppeBaseFilePath] > 0) {
		defaultFilePath = [[self enveloppeEditorDocument] enveloppeBaseFilePath];
		//NSString* envPath = [dirPath stringByAppendingPathComponent:@"Enveloppe.vscxenv"];
	}
    
    sp.directoryURL = [NSURL fileURLWithPath:@""];
    sp.nameFieldStringValue = @"Enveloppe";
	
	/* display the NSSavePanel */
	int runResult = [sp runModal];
	
	/* if successful, save file under designated name */
	if (runResult == NSOKButton) {
		saveVSCEnveloppeToXML(*currentEnveloppe, (const char *)[[[sp directoryURL] path] UTF8String]);
	}
	
}

@end
