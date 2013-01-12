//
//  VSCOSXEnvelopeViewController.m
//  EnvelopeEditor
//
//  Created by Jonathan Thorpe on 12/16/12.
//
//

#import "VSCOSXEnvelopeViewController.h"
#import "NSString+VSCAdditions.h"

//typedef std::vector<Envelope::WPtr> Envelopes;

@interface VSCOSXEnvelopeViewController ()

@end

@implementation VSCOSXEnvelopeViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Initialization code here.
    }
    
    return self;
}



-(void) setEnvelope:(VSC::Envelope::WPtr)envelope {
    _currentEnvelope = envelope;
    [self.envelopeView redrawMainEnvelope];
}

-(void) addEnvelope:(VSC::Envelope::SPtr)envelope {
    // not implemented
}

-(void) removeEnvelope:(VSC::Envelope::SPtr)envelope {
    // not implemented
}

#pragma mark - VSC::EnvelopeViewDataSource Methods

-(VSC::Envelope::SPtr) mainEnvelopeForEnvelopeView:(VSC::EnvelopeView*)envelopeView {
    return _currentEnvelope;
}

-(VSC::Envelope::List) backgroundEnvelopesForEnvelopeView:(VSC::EnvelopeView*)envelopeView {
    //return VSC::Envelope::List();
    return _backgroundEnvelopes;
}


#pragma mark - MenuCallbacks Callbacks

-(IBAction) loadEnvelope:(id)sender {
	NSLog(@"Load Envelope");
	
    NSArray *fileTypes = [NSArray arrayWithObject:[NSString stringWithStdString:VSC::Envelope::FileExtension]];
    NSOpenPanel *oPanel = [NSOpenPanel openPanel];
    [oPanel setAllowsMultipleSelection:NO];
	
    
    int result = [oPanel runModalForDirectory:self.defaultEnvelopePath file:nil types:fileTypes];
    if (result == NSOKButton) {
        NSArray *filesToOpen = [oPanel filenames];
		NSAssert([filesToOpen count] < 2, @"Cannot open multiple files");
        if ([filesToOpen count] == 1) {
			VSC::Envelope::SPtr newEnvelope = VSC::Envelope::createFromXMLFile([[filesToOpen objectAtIndex:0] UTF8String]);
			newEnvelope->setFilePath([[filesToOpen objectAtIndex:0] UTF8String]);
			[self setCurrentEnvelope:newEnvelope];
		}
    }
}

-(IBAction) saveEnvelope:(id)sender {
	NSLog(@"Save Envelope");
	
	NSString* filePath = [NSString stringWithCString:([self getCurrentEnvelope]->getFilePath()).c_str() encoding:NSUTF8StringEncoding];
	
	if ([filePath length] == 0) {
		[self saveEnvelopeAs:sender];
		return;
	}
	
	NSLog(@"Saving envelope to %@", filePath);
	
	if ([[NSFileManager defaultManager] isWritableFileAtPath:filePath]) {
		[self getCurrentEnvelope]->saveToXMLFile((const char *)[filePath UTF8String]);
	}
	else {
		[self saveEnvelopeAs:sender];
		return;
	}
	
	
}

-(IBAction) saveEnvelopeAs:(id)sender {
	NSLog(@"Save Envelope As");
	
	NSAssert([self getCurrentEnvelope], @"Should't receive save command when currentEnvelope is nil");
	
	if (![self getCurrentEnvelope]) {
		return;
	}
	
	/* create or get the shared instance of NSSavePanel */
	NSSavePanel* sp = [NSSavePanel savePanel];
    
    sp.allowedFileTypes = [NSArray arrayWithObject:[NSString stringWithStdString:VSC::Envelope::FileExtension]];
    
    sp.directoryURL = [NSURL fileURLWithPath:@""];
    sp.nameFieldStringValue = @"Envelope";
	
	/* display the NSSavePanel */
	int runResult = [sp runModal];
	
	/* if successful, save file under designated name */
	if (runResult == NSOKButton) {
		[self getCurrentEnvelope]->saveToXMLFile((const char *)[[[sp directoryURL] path] UTF8String]);
	}
	
}

@end
