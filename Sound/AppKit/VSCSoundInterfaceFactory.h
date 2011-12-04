//
//  VSCSoundInterfaceFactory.h
//  SynthStation
//
//  Created by Jonathan Thorpe on 28/11/2011.
//  Copyright 2011 NXP. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#import "VSCSoundElement.h"
#import "VSCSoundElementView.h"

#import "VSCMatrixParameterControlView.h"

typedef enum _VSCSoundInterfaceFactoryTheme {
	VSCSoundInterfaceFactoryThemeNone,
	VSCSoundInterfaceFactoryThemeDefault
} VSCSoundInterfaceFactoryTheme;

typedef enum _VSCSoundInterfaceFactoryOptions {
	VSCSoundInterfaceFactoryOptionNone = 0
} VSCSoundInterfaceFactoryOptions;

@interface VSCSoundInterfaceFactory : NSObject {
	
	VSCSoundInterfaceFactoryTheme theme;
	VSCSoundInterfaceFactoryOptions options;

}

@property (nonatomic, assign) VSCSoundInterfaceFactoryTheme theme;
@property (nonatomic, assign) VSCSoundInterfaceFactoryOptions options;

+(VSCSoundInterfaceFactory*) defaultFactory;

/*
 *	Main sound element view builder functions
 */
-(VSCSoundElementView*) soundElementViewForSoundElement:(VSCSoundElementPtr)soundElement;
-(void) configureSoundElementView:(VSCSoundElementView*)v 
				  forSoundElement:(VSCSoundElementPtr)soundElement;

/*
 *	Intermediate helper functions
 */
-(VSCMatrixParameterControlView*) matrixParameterControlViewForParameterKeys:(VSCSParameter::KeySet)keys
																   withFrame:(NSRect)f;
-(VSCMatrixParameterControlView*) matrixParameterControlViewForParameterDomain:(VSCSParameter::Domain)domain
																	 withFrame:(NSRect)frame;

@end
