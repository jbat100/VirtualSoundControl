//
//  VSCOSXEnvelopeViewController.m
//  EnvelopeEditor
//
//  Created by Jonathan Thorpe on 12/16/12.
//
//

#import "VSCOSXEnvelopeViewController.h"
#import "NSString+VSCAdditions.h"

#import "VSCOSXEnvelopeView.h"

using namespace VSC;

@interface VSCOSXEnvelopeViewController ()
{
    Envelopes _backgroundEnvelopes;
}

@end

@implementation VSCOSXEnvelopeViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self)
    {
        // Initialization code here.
    }
    
    return self;
}

-(void) setCurrentEnvelope:(Envelope_SPtr)envelope
{
    _currentEnvelope = envelope;
}

-(void) addBackgroundEnvelope:(Envelope_SPtr)envelope
{
    if (std::find(_backgroundEnvelopes.begin(), _backgroundEnvelopes.end(), envelope) == _backgroundEnvelopes.end())
    {
        _backgroundEnvelopes.push_back(envelope);
    }
}

-(void) removeBackgroundEnvelope:(Envelope_SPtr)envelope
{
    // http://en.wikipedia.org/wiki/Erase-remove_idiom
    _backgroundEnvelopes.erase(std::remove(_backgroundEnvelopes.begin(), _backgroundEnvelopes.end(), envelope), _backgroundEnvelopes.end());
}

-(void) clearBackgroundEnvelopes
{
    _backgroundEnvelopes.clear();
}

-(const VSC::Envelopes&) backgroundEnvelopes
{
    return _backgroundEnvelopes;
}

#pragma mark - EnvelopeViewDataSource Methods

-(Envelope_SPtr) mainEnvelopeForEnvelopeView:(VSCOSXEnvelopeView*)envelopeView
{
    return _currentEnvelope;
}

-(Envelopes) backgroundEnvelopesForEnvelopeView:(VSCOSXEnvelopeView*)envelopeView
{
    return _backgroundEnvelopes;
}


#pragma mark - MenuCallbacks Callbacks

-(IBAction) loadEnvelope:(id)sender
{
	NSLog(@"Load Envelope");
	
    NSArray *fileTypes = [NSArray arrayWithObject:[NSString stringWithStdString:Envelope::FileExtension]];
    NSOpenPanel *oPanel = [NSOpenPanel openPanel];
    [oPanel setAllowsMultipleSelection:NO];
    
    int result = [oPanel runModalForDirectory:self.defaultEnvelopePath file:nil types:fileTypes];
    if (result == NSOKButton)
    {
        NSArray *filesToOpen = [oPanel filenames];
		NSAssert([filesToOpen count] < 2, @"Cannot open multiple files");
        if ([filesToOpen count] == 1)
        {
			Envelope_SPtr newEnvelope = Envelope::createFromXMLFile([[filesToOpen objectAtIndex:0] UTF8String]);
			newEnvelope->setFilePath([[filesToOpen objectAtIndex:0] UTF8String]);
			[self setCurrentEnvelope:newEnvelope];
		}
    }
}

-(IBAction) saveEnvelope:(id)sender
{
	NSLog(@"Save Envelope");
	
	NSString* filePath = [NSString stringWithCString:(self.currentEnvelope->getFilePath()).c_str() encoding:NSUTF8StringEncoding];
	
	if ([filePath length] == 0)
    {
		[self saveEnvelopeAs:sender];
		return;
	}
	
	NSLog(@"Saving envelope to %@", filePath);
	
	if ([[NSFileManager defaultManager] isWritableFileAtPath:filePath])
    {
		self.currentEnvelope->saveToXMLFile((const char *)[filePath UTF8String]);
	}
	else
    {
		[self saveEnvelopeAs:sender];
		return;
	}
}

-(IBAction) saveEnvelopeAs:(id)sender
{
	NSLog(@"Save Envelope As");
	
	NSAssert(self.currentEnvelope, @"Should't receive save command when currentEnvelope is nil");
	
	if (!self.currentEnvelope)
    {
		return;
	}
	
	/* create or get the shared instance of NSSavePanel */
	NSSavePanel* sp = [NSSavePanel savePanel];
    
    sp.allowedFileTypes = [NSArray arrayWithObject:[NSString stringWithStdString:Envelope::FileExtension]];
    
    sp.directoryURL = [NSURL fileURLWithPath:@""];
    sp.nameFieldStringValue = @"Envelope";
	
	/* display the NSSavePanel */
	int runResult = [sp runModal];
	
	/* if successful, save file under designated name */
	if (runResult == NSOKButton)
    {
		self.currentEnvelope->saveToXMLFile((const char *)[[[sp directoryURL] path] UTF8String]);
	}
}

@end
