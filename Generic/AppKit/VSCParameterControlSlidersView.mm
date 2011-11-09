//
//  VSCSlidersControlView.m
//  SynthStation
//
//  Created by Jonathan Thorpe on 06/11/2011.
//  Copyright 2011 NXP. All rights reserved.
//

#import "VSCParameterControlSlidersView.h"


@implementation VSCParameterControlSlidersView

-(void) customInit {
	
	[super customInit];
	
	NSArray* availableFontNames = [NSFontManager sharedFontManager] availableFonts];
	
	NSLog(@"Available fonts are: %@", availableFontNames);
	
	self.controllerCellPrototype = [[[NSSliderCell alloc] init] autorelease];
	[self.labelCellPrototype setFont:[NSFont fontWithName:@"Verdana-Bold" size:13]];
	
	[self createMatrices];
	
}



@end
