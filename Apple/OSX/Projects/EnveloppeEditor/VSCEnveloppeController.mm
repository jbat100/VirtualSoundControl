//
//  VSCEnveloppeController.m
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 4/2/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCEnveloppeController.h"

#import "NSString+VSCAdditions.h"

@implementation VSCEnveloppeController

@synthesize enveloppeView = _enveloppeView;
@synthesize horizontalScaleSlider = _horizontalScaleSlider;

@synthesize defaultEnveloppePath = _defaultEnvelppePath;

-(id) init {
    
    if ((self = [super init])) {
        
        _currentEnveloppe = VSCEnveloppePtr();
        
    }
    
    return self;
    
}


-(VSCEnveloppePtr) getCurrentEnveloppe {
    return _currentEnveloppe;
}

-(void) setCurrentEnveloppe:(VSCEnveloppePtr)enveloppe {
    _currentEnveloppe = enveloppe;
    [self.enveloppeView redrawMainEnveloppe];
}

-(void) addEnveloppe:(VSCEnveloppePtr)enveloppe {
    // not implemented
}

-(void) removeEnveloppe:(VSCEnveloppePtr)enveloppe {
    // not implemented
}

#pragma mark - VSCEnveloppeViewDataSource Methods

-(VSCEnveloppePtr) mainEnveloppeForEnveloppeView:(VSCEnveloppeView*)enveloppeView {
    return _currentEnveloppe;
}

-(VSCEnveloppe::List) backgroundEnveloppesForEnveloppeView:(VSCEnveloppeView*)enveloppeView {
    //return VSCEnveloppe::List();
    return _backgroundEnveloppes;
}


#pragma mark - MenuCallbacks Callbacks

-(IBAction) loadEnveloppe:(id)sender {
	NSLog(@"Load Enveloppe");
	
    NSArray *fileTypes = [NSArray arrayWithObject:[NSString stringWithStdString:VSCEnveloppe::FileExtension]];
    NSOpenPanel *oPanel = [NSOpenPanel openPanel];
    [oPanel setAllowsMultipleSelection:NO];
	
    
    int result = [oPanel runModalForDirectory:self.defaultEnveloppePath file:nil types:fileTypes];
    if (result == NSOKButton) {
        NSArray *filesToOpen = [oPanel filenames];
		NSAssert([filesToOpen count] < 2, @"Cannot open multiple files");
        if ([filesToOpen count] == 1) {
			VSCEnveloppePtr newEnveloppe = VSCEnveloppe::createFromXMLFile([[filesToOpen objectAtIndex:0] UTF8String]);
			newEnveloppe->setFilePath([[filesToOpen objectAtIndex:0] UTF8String]);
			[self setCurrentEnveloppe:newEnveloppe];
		}
    }
}

-(IBAction) saveEnveloppe:(id)sender {
	NSLog(@"Save Enveloppe");
	
	NSString* filePath = [NSString stringWithCString:([self getCurrentEnveloppe]->getFilePath()).c_str() encoding:NSUTF8StringEncoding];
	
	if ([filePath length] == 0) {
		[self saveEnveloppeAs:sender];
		return;
	}
	
	NSLog(@"Saving enveloppe to %@", filePath);
	
	if ([[NSFileManager defaultManager] isWritableFileAtPath:filePath]) {
		[self getCurrentEnveloppe]->saveToXMLFile((const char *)[filePath UTF8String]);
	}
	else {
		[self saveEnveloppeAs:sender];
		return;
	}
	
	
}

-(IBAction) saveEnveloppeAs:(id)sender {
	NSLog(@"Save Enveloppe As");
	
	NSAssert([self getCurrentEnveloppe], @"Should't receive save command when currentEnveloppe is nil");
	
	if (![self getCurrentEnveloppe]) {
		return;
	}
	
	/* create or get the shared instance of NSSavePanel */
	NSSavePanel* sp = [NSSavePanel savePanel];
    
    sp.allowedFileTypes = [NSArray arrayWithObject:[NSString stringWithStdString:VSCEnveloppe::FileExtension]];
    
    sp.directoryURL = [NSURL fileURLWithPath:@""];
    sp.nameFieldStringValue = @"Enveloppe";
	
	/* display the NSSavePanel */
	int runResult = [sp runModal];
	
	/* if successful, save file under designated name */
	if (runResult == NSOKButton) {
		[self getCurrentEnveloppe]->saveToXMLFile((const char *)[[[sp directoryURL] path] UTF8String]);
	}
	
}

@end
