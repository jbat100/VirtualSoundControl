//
//  LightControlView.m
//  MeshFrames3
//
//  Created by Jonathan Thorpe on 05/04/2010.
//  Copyright 2010 JBAT. All rights reserved.
//

#import "LightControlViewController.h"
#import <iostream>


@implementation LightControlViewController

@synthesize dataSource;

#pragma mark -
#pragma mark View Controller Methods

- (void)awakeFromNib {
	
	NSLog(@"In LightControlViewController awakeFromNib");
	
	/*
	 
	 Resizing masks
	
	NSViewNotSizable     = 0,
	NSViewMinXMargin     = 1,
	NSViewWidthSizable   = 2,
	NSViewMaxXMargin     = 4,
	NSViewMinYMargin     = 8,
	NSViewHeightSizable  = 16,
	NSViewMaxYMargin     = 32
	 
	 */
	
	[[self view] setAutoresizesSubviews:YES];
	
	//[directionBox setAutoresizingMask:NSViewMinXMargin |
	
	//NSUInteger labelsResizingMask = NSViewMinXMargin | NSViewMinYMargin;
	//NSUInteger textFieldResizingMask = 
	
	
	
	[spotlightAngleSlider setMaxValue:360.0];
	[spotlightAngleSlider setMinValue:0.0];
	
	// Set Text field delegates
	id d = self;
	
	[xCoordinateTextField setDelegate:d];
	[yCoordinateTextField setDelegate:d];
	[zCoordinateTextField setDelegate:d];
	[xDirectionTextField setDelegate:d];
	[yDirectionTextField setDelegate:d];
	[zDirectionTextField setDelegate:d];
	
}

#pragma mark -
#pragma mark Interface update Methods

- (void) updateInterface {
	
	NSLog(@"Updating light interface");
	
	JBAT_Light *light = [dataSource light];
	
	if (light) {
		
		JBAT_Light *light = [dataSource light];
		
		//NSLog(@"Updating light interface, light is: ");
		
		std::cout << *light;
	
		[self enableInterface:light->isOn() ];
		
		[spotlightAngleSlider setFloatValue:light->getSpotlightAngle() ];
		
		if (light->isSpotlight()) {
			[spotlightButton setState:1];
		}
		else {
			[spotlightButton setState:0];
			[spotlightAngleSlider setEnabled:NO];
		}
		
		float* position = light->getPosition();
		float* direction = light->getDirection();

		[xCoordinateTextField setStringValue:[NSString stringWithFormat:@"%f", position[0]]];
		[yCoordinateTextField setStringValue:[NSString stringWithFormat:@"%f", position[1]]];
		[zCoordinateTextField setStringValue:[NSString stringWithFormat:@"%f", position[2]]];
		
		[xDirectionTextField setStringValue:[NSString stringWithFormat:@"%f", direction[0]]];
		[yDirectionTextField setStringValue:[NSString stringWithFormat:@"%f", direction[1]]];
		[zDirectionTextField setStringValue:[NSString stringWithFormat:@"%f", direction[2]]];
		
		
		NSLog(@"Setting ambientColorButton cell background color to %@", [self ambientColor]);
		[[ambientColorButton cell] setBackgroundColor:[self ambientColor]];
		[[diffuseColorButton cell] setBackgroundColor:[self diffuseColor]];
		[[specularColorButton cell] setBackgroundColor:[self specularColor]];
		
		if (light->isOn()) [enabledButton setState:1];
		else [enabledButton setState:0];
		
	
	}
	
	else {
		
		NSLog(@"Updating light interface, light is NULL");
		
		[self enableInterface:NO];
	}

	
}

- (void) enableInterface:(BOOL)enable {
	
	if (!enable) {
		
		[xCoordinateTextField setStringValue:@""];
		[yCoordinateTextField setStringValue:@""];
		[zCoordinateTextField setStringValue:@""];
		
		[xDirectionTextField setStringValue:@""];
		[yDirectionTextField setStringValue:@""];
		[zDirectionTextField setStringValue:@""];
		
		[[ambientColorButton cell] setBackgroundColor:[NSColor whiteColor]];
		[[diffuseColorButton cell] setBackgroundColor:[NSColor whiteColor]];
		[[specularColorButton cell] setBackgroundColor:[NSColor whiteColor]];
	}
	
	[enabledButton setEnabled:YES];
	[spotlightButton setEnabled:enable];
	[spotlightAngleSlider setEnabled:enable];
	
	[xCoordinateTextField setEnabled:enable];
	[yCoordinateTextField setEnabled:enable];
	[zCoordinateTextField setEnabled:enable];
	
	[xDirectionTextField setEnabled:enable];
	[yDirectionTextField setEnabled:enable];
	[zDirectionTextField setEnabled:enable];
	
	[ambientColorButton setEnabled:enable];
	[diffuseColorButton setEnabled:enable];
	[specularColorButton setEnabled:enable];
	
}

#pragma mark -
#pragma mark Interface Callback Methods

- (IBAction) buttonClicked:(id)sender {
	
	JBAT_Light *light = [dataSource light];
	
	if (light) {
	
		if (sender == enabledButton) {
			if ([enabledButton state]) light->enable();
			else light->disable();
			light->setOnNeedsUpdate();
		}
		
		else if (sender == spotlightButton) {
			if ([spotlightButton state]) light->enableSpotlight();
			else light->disableSpotlight();
			light->setSpotLightNeedsUpdate();
		}

		else if (sender == ambientColorButton) {
			NSColorPanel *colorPanel;
			colorPanel = [NSColorPanel sharedColorPanel];
			[colorPanel setAction:@selector(colorPanelAmbientSelection:)];
			[colorPanel setTarget:self];
			[colorPanel setColor:[self ambientColor]];
			[colorPanel makeKeyAndOrderFront:nil];
		}
		
		else if (sender == diffuseColorButton) {
			NSColorPanel *colorPanel;
			colorPanel = [NSColorPanel sharedColorPanel];
			[colorPanel setAction:@selector(colorPanelDiffuseSelection:)];
			[colorPanel setTarget:self];
			[colorPanel setColor:[self diffuseColor]];
			[colorPanel makeKeyAndOrderFront:nil];
		}
		
		else if (sender == specularColorButton) {
			NSColorPanel *colorPanel;
			colorPanel = [NSColorPanel sharedColorPanel];
			[colorPanel setAction:@selector(colorPanelSpecularSelection:)];
			[colorPanel setTarget:self];
			[colorPanel setColor:[self specularColor]];
			[colorPanel makeKeyAndOrderFront:nil];
		}
		
	}
	
	[self updateInterface];

	
}

-(IBAction) sliderMoved:(id)sender {
	
	NSLog(@"Slider moved to value %f", [spotlightAngleSlider floatValue]);
	
	JBAT_Light *light = [dataSource light]; 
	
	if (light) {
		light->setSpotlightAngle([spotlightAngleSlider floatValue]);
		light->setSpotLightAngleNeedsUpdate();
	}
	
	[self updateInterface];
	
}

-(void) colorPanelAmbientSelection:(id)sender {
	
	JBAT_Light *light = [dataSource light];
	
	if (light) {
		
		float* ambientColor = light->getAmbientColor();
	
		ambientColor[0] = [[sender color] redComponent];
		ambientColor[1] = [[sender color] greenComponent];
		ambientColor[2] = [[sender color] blueComponent];
		ambientColor[3] = [[sender color] alphaComponent];
		light->setAmbientColorNeedsUpdate();
		
		[[ambientColorButton cell] setBackgroundColor:[self ambientColor]];
		
	}
	
	[self updateInterface];
	
}

-(void) colorPanelDiffuseSelection:(id)sender {
	
	JBAT_Light *light = [dataSource light];
	
	if (light) {
		
		float* diffuseColor = light->getDiffuseColor();
	
		diffuseColor[0] = [[sender color] redComponent];
		diffuseColor[1] = [[sender color] greenComponent];
		diffuseColor[2] = [[sender color] blueComponent];
		diffuseColor[3] = [[sender color] alphaComponent];
		light->setDiffuseColorNeedsUpdate();
		
		[[diffuseColorButton cell] setBackgroundColor:[self diffuseColor]];
		
	}
}

-(void) colorPanelSpecularSelection:(id)sender {
	
	JBAT_Light *light = [dataSource light];
	
	if (light) {
		
		float* specularColor = light->getSpecularColor();
	
		specularColor[0] = [[sender color] redComponent];
		specularColor[1] = [[sender color] greenComponent];
		specularColor[2] = [[sender color] blueComponent];
		specularColor[3] = [[sender color] alphaComponent];
		light->setSpecularColorNeedsUpdate();
		
		[[specularColorButton cell] setBackgroundColor:[self specularColor]];
		
	}
}

#pragma mark -
#pragma mark Text Field Delegate Methods

- (BOOL)control:(NSControl *)control textShouldEndEditing:(NSText *)fieldEditor {
	
	NSLog(@"Control %@ text should end %@", control, fieldEditor);
	
	JBAT_Light *light = [dataSource light];
	
	if (light) {
		NSScanner *scanner;
		BOOL success;
		float floatValue;
		float* position = light->getPosition();
		float* direction = light->getDirection();
		
		if (control == xCoordinateTextField) {
			scanner = [NSScanner scannerWithString:[xCoordinateTextField stringValue]];
			success = [scanner scanFloat:&floatValue];
			if (success) {
				position[0] = floatValue;
				light->setPositionNeedsUpdate();
			}			
		}
		
		else if (control == yCoordinateTextField) {
			scanner = [NSScanner scannerWithString:[yCoordinateTextField stringValue]];
			success = [scanner scanFloat:&floatValue];
			if (success) {
				position[1] = floatValue;
				light->setPositionNeedsUpdate();
			}			
		}
		
		else if (control == zCoordinateTextField) {
			scanner = [NSScanner scannerWithString:[zCoordinateTextField stringValue]];
			success = [scanner scanFloat:&floatValue];
			if (success) {
				position[2] = floatValue;
				light->setPositionNeedsUpdate();
			}			
		}
		
		else if (control == xDirectionTextField) {
			scanner = [NSScanner scannerWithString:[xDirectionTextField stringValue]];
			success = [scanner scanFloat:&floatValue];
			if (success) {
				direction[0] = floatValue;
				light->setDirectionNeedsUpdate();
			}		
		}
		
		else if (control == yDirectionTextField) {
			scanner = [NSScanner scannerWithString:[yDirectionTextField stringValue]];
			success = [scanner scanFloat:&floatValue];
			if (success) {
				direction[1] = floatValue;
				light->setDirectionNeedsUpdate();
			}		
		}
		
		else if (control == zDirectionTextField) {
			scanner = [NSScanner scannerWithString:[zDirectionTextField stringValue]];
			success = [scanner scanFloat:&floatValue];
			if (success) {
				direction[2] = floatValue;
				light->setDirectionNeedsUpdate();
			}			
		}
		
	}
	
	[self updateInterface];
	
	return YES;
}
	

#pragma mark -
#pragma mark Utility Methods

-(NSColor *) ambientColor {
	
	JBAT_Light *light = [dataSource light];
	
	if (light) {
		
		float* ambientColor = light->getAmbientColor();
	
		return [NSColor colorWithCalibratedRed:ambientColor[0] 
										 green:ambientColor[1] 
										  blue:ambientColor[2] 
										 alpha:ambientColor[3]];
		
	}
	
	return nil;
}

-(NSColor *) diffuseColor {
	
	JBAT_Light *light = [dataSource light];
	
	if (light) {
		
		float* diffuseColor = light->getDiffuseColor();
		
		return [NSColor colorWithCalibratedRed:diffuseColor[0] 
										 green:diffuseColor[1] 
										  blue:diffuseColor[2] 
										 alpha:diffuseColor[3]];
		
	}
	
	return nil;
}
	
-(NSColor *) specularColor {
	
	JBAT_Light *light = [dataSource light];
	
	if (light) {
		
		float* specularColor = light->getSpecularColor();
		
		return 	[NSColor colorWithCalibratedRed:specularColor[0] 
										  green:specularColor[1] 
										   blue:specularColor[2] 
										  alpha:specularColor[3]];
	
	}
	
	return nil;
	
}




@end
