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

@property (nonatomic, retain) NSMenu* fileMenu;
@property (nonatomic, retain) NSMenuItem* fileTitleMenuItem;

-(void) updateEnveloppePopUpMenu;
-(void) checkInterface;

-(void) saveEnveloppeAs:(id)sender;

@end


@implementation VSCEnveloppeEditorWindowController

@synthesize enveloppeView, loadedTextField, enveloppePopUpButton, testButton, fileMenu, fileTitleMenuItem;

#pragma mark - NSWindowController Window Callbacks

-(void) windowDidLoad {
	NSLog(@"%@ window did load", self);
	[self updateEnveloppePopUpMenu];
	[self checkInterface];
}

-(void) windowWillLoad {
	NSLog(@"%@ window will load", self);
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
	[enveloppeView setEnveloppe:enveloppe];
	
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
	
	[enveloppeView setNeedsDisplay:YES];
	
}

#pragma mark - Button Callbacks

-(IBAction) testButtonClicked:(id)sender {
	NSLog(@"%@ window test button clicked", self);
	[self setCurrentEnveloppe:[[self enveloppeEditorDocument] defaultEnveloppe]];
}

#pragma mark - Interface Update 

-(void) updateEnveloppePopUpMenu {
	
	self.fileMenu = [[[NSMenu alloc] initWithTitle:FILE_MENU_TITLE] autorelease];
	
	self.fileTitleMenuItem = [[[NSMenuItem alloc] initWithTitle:FILE_MENU_TITLE
														action:NULL 
												 keyEquivalent:@""] autorelease];
	[fileMenu addItem:fileTitleMenuItem];
	
	NSMenuItem* loadItem = [[NSMenuItem alloc] initWithTitle:LOAD_ENVELOPPE_MENU_ITEM_TITLE
													  action:@selector(loadEnveloppe:) 
											   keyEquivalent:@""];
	[fileMenu addItem:loadItem];
	[loadItem release];
	
	NSMenuItem* loadRecentItem = [[NSMenuItem alloc] initWithTitle:LOAD_RECENT_ENVELOPPE_MENU_ITEM_TITLE
													  action:nil
											   keyEquivalent:@""];
	[fileMenu addItem:loadRecentItem];
	[loadRecentItem release];
	
	
	[fileMenu addItem:[NSMenuItem separatorItem]];
	
	NSMenuItem* saveItem = [[NSMenuItem alloc] initWithTitle:SAVE_ENVELOPPE_MENU_ITEM_TITLE
													  action:@selector(saveEnveloppe:) 
											   keyEquivalent:@""];
	
	[fileMenu addItem:saveItem];
	[saveItem release];
	
	NSMenuItem* saveAsItem = [[NSMenuItem alloc] initWithTitle:SAVE_ENVELOPPE_AS_MENU_ITEM_TITLE
													  action:@selector(saveEnveloppeAs:) 
											   keyEquivalent:@""];
	
	[fileMenu addItem:saveAsItem];
	[saveAsItem release];
	
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
			newEnveloppe->setRelativePath([[filesToOpen objectAtIndex:0] UTF8String]);
			[self setCurrentEnveloppe:newEnveloppe];
		}
    }
}

-(void) saveEnveloppe:(id)sender {
	NSLog(@"Save Enveloppe");
	
	[enveloppePopUpButton selectItem:fileTitleMenuItem];
	
	NSAssert(currentEnveloppe, @"Should't receive save as command when currentEnveloppe is nil");
	
	NSString* filePath = [NSString stringWithCString:(currentEnveloppe->getRelativePath()).c_str() encoding:NSUTF8StringEncoding];
	
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
	[sp setRequiredFileType:[NSString stringWithCString:ENVELOPPE_FILE_EXTENSION encoding:NSUTF8StringEncoding]];
	
	NSString* defaultFilePath = @"";
	
	NSAssert([[self enveloppeEditorDocument] enveloppeBaseFilePath] > 0, @"Should have default enveloppe path");
	
	if ([[self enveloppeEditorDocument] enveloppeBaseFilePath] > 0) {
		defaultFilePath = [[self enveloppeEditorDocument] enveloppeBaseFilePath];
		//NSString* envPath = [dirPath stringByAppendingPathComponent:@"Enveloppe.vscxenv"];
	}
	
	/* display the NSSavePanel */
	int runResult = [sp runModalForDirectory:defaultFilePath file:@"Enveloppe"];
	
	/* if successful, save file under designated name */
	if (runResult == NSOKButton) {
		saveVSCEnveloppeToXML(*currentEnveloppe, (const char *)[[sp filename] UTF8String]);
	}
	
}

@end
