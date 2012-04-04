//
//  SoundStationDocument.m
//  VSoundStation
//
//  Created by Jonathan Thorpe on 31/01/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import "SoundStationDocument.h"
#import "NetworkDefinitions.h"
#import "SBJSON.h"
#import "Touch.h"
#import "TouchDisplayView.h"
#import "MultiDisplayView.h"



@implementation SoundStationDocument

@synthesize testButton;

- (NSString *)windowNibName {
    // Implement this to return a nib to load OR implement -makeWindowControllers to manually create your controllers.
    return @"SoundStationDocument";
}

- (NSData *)dataOfType:(NSString *)typeName error:(NSError **)outError
{
    // Insert code here to write your document to data of the specified type. If the given outError != NULL, ensure that you set *outError when returning nil.
    // You can also choose to override -fileWrapperOfType:error:, -writeToURL:ofType:error:, or -writeToURL:ofType:forSaveOperation:originalContentsURL:error: instead.
    // For applications targeted for Panther or earlier systems, you should use the deprecated API -dataRepresentationOfType:. In this case you can also choose to override -fileWrapperRepresentationOfType: or -writeToFile:ofType: instead.

    return nil;
}

- (BOOL)readFromData:(NSData *)data ofType:(NSString *)typeName error:(NSError **)outError
{
    // Insert code here to read your document from the given data of the specified type.  If the given outError != NULL, ensure that you set *outError when returning NO.
    // You can also choose to override -readFromFileWrapper:ofType:error: or -readFromURL:ofType:error: instead. 
    // For applications targeted for Panther or earlier systems, you should use the deprecated API -loadDataRepresentation:ofType. In this case you can also choose to override -readFromFile:ofType: or -loadFileWrapperRepresentation:ofType: instead.
    
    return YES;
}

-(IBAction) buttonTouched:(id)sender {
	
	if (sender == testButton) {
		NSLog(@"Yo bitch, you're pressing the testButton, ... beyaaaatch");
	}
	
	NSSliderCell* cell0 = [accelerationSliderMatrix cellAtRow:0 column:0];
	NSSliderCell* cell1 = [accelerationSliderMatrix cellAtRow:1 column:0];
	NSSliderCell* cell2 = [accelerationSliderMatrix cellAtRow:2 column:0];
	
	[cell0 setFloatValue:1.2];
	[cell1 setFloatValue:0.0];
	[cell2 setFloatValue:-1.2];
	
}

#pragma mark -
#pragma mark Interface Methods

-(void) updateViewWithStateString:(NSString*)stateString {
	
	if (!jsonParser)
		jsonParser = [SBJSON new];
	
	NSError *errorMsg;	
	NSDictionary* jsonDict = [jsonParser objectWithString:stateString error:&errorMsg];
	
	//NSLog(@"Updating with jsonDict: %@", jsonDict);
	
	
	NSDictionary* accelDict = [jsonDict objectForKey:[NSString stringWithUTF8String:JSON_ACCELERATION_DICTIONARY_KEY]];
	
	NSString* xString = [accelDict objectForKey:[NSString stringWithUTF8String:JSON_ACCELERATION_X_KEY]];
	NSString* yString = [accelDict objectForKey:[NSString stringWithUTF8String:JSON_ACCELERATION_Y_KEY]];
	NSString* zString = [accelDict objectForKey:[NSString stringWithUTF8String:JSON_ACCELERATION_Z_KEY]];
	
	NSScanner* scanner = nil;
	float xf = 0.0; 
	float yf = 0.0; 
	float zf = 0.0;
	
	scanner = [NSScanner scannerWithString:xString];
	[scanner scanFloat:&xf];
	scanner = [NSScanner scannerWithString:yString];
	[scanner scanFloat:&yf];
	scanner = [NSScanner scannerWithString:zString];
	[scanner scanFloat:&zf];
	
	NSSliderCell* cell0 = [accelerationSliderMatrix cellAtRow:0 column:0];
	NSSliderCell* cell1 = [accelerationSliderMatrix cellAtRow:1 column:0];
	NSSliderCell* cell2 = [accelerationSliderMatrix cellAtRow:2 column:0];
	
	[cell0 setFloatValue:xf];
	[cell1 setFloatValue:yf];
	[cell2 setFloatValue:zf];
	
	NSArray* controlViewArray = [jsonDict objectForKey:[NSString stringWithUTF8String:JSON_CTRL_VIEW_ARRAY_KEY]];
	
	//NSLog(@"View dictionary is %@", viewDictionary);
	
	BOOL displayViewChanged = NO;
	
	for (NSDictionary* controlViewDict in controlViewArray) {
		
		//NSLog(@"Updating view for controlViewDict: %@", controlViewDict);
		
		NSString* viewId = [controlViewDict objectForKey:[NSString stringWithUTF8String:JSON_CTRL_VIEW_ID_KEY]];
		
		//NSLog(@"viewId is %@, objectForKey %@", viewId, [NSString stringWithUTF8String:JSON_CTRL_VIEW_ID_KEY]);
		
		DisplayView* displayView = [multiDisplayView displayViewWithId:viewId];
		
		//NSLog(@"displayView is %@", displayView);
		
		
		
		NSString* type = [controlViewDict objectForKey:[NSString stringWithUTF8String:JSON_CTRL_VIEW_TYPE_KEY]];
		
		if (!displayView) {
			
			displayViewChanged = YES;
			
			if ([type isEqualToString:[NSString stringWithUTF8String:JSON_CTRL_VIEW_TOUCH_TYPE]]) {
				displayView = [[TouchDisplayView alloc] init];
				displayView.viewId = viewId;
				[multiDisplayView addDisplayView:displayView];
			}
			
		}
		
		else {
			
			if ([type isEqualToString:[NSString stringWithUTF8String:JSON_CTRL_VIEW_TOUCH_TYPE]]) {
				
				if (![displayView isKindOfClass:[TouchDisplayView class]]) {
					displayViewChanged = YES;
					[multiDisplayView removeDisplayView:displayView];
					displayView = [[TouchDisplayView alloc] init];
					displayView.viewId = viewId;
					[multiDisplayView addDisplayView:displayView];
				}
				
			}
				
		}
		
		//NSLog(@"multiDisplayView.displayViews are %@", multiDisplayView.displayViews);
		
		
		if ([type isEqualToString:[NSString stringWithUTF8String:JSON_CTRL_VIEW_TOUCH_TYPE]]) {

			NSArray* touchArray = [controlViewDict objectForKey:[NSString stringWithUTF8String:JSON_TOUCH_ARRAY_KEY]];
			[(TouchDisplayView*)displayView updateTouchesWithJSONArray:touchArray];
				
		}
		
		[displayView setNeedsDisplay:YES];
	}
	
	if (displayViewChanged) {
		[multiDisplayView layoutDisplayViews];
	}
	
	//NSLog(@"Ended: multiDisplayView.displayViews are %@", multiDisplayView.displayViews);
	
	[multiDisplayView setNeedsDisplay:YES];
	
}


@end
