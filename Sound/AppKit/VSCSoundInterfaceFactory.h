//
//  VSCSoundInterfaceFactory.h
//  SynthStation
//
//  Created by Jonathan Thorpe on 28/11/2011.
//  Copyright 2011 NXP. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#import "VSCSoundParameters.h"
#import "VSCSoundElement.h"
#import "VSCSoundElementView.h"

@class VSCParameterSliderControlView;

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

-(VSCParameterSliderControlView*) parameterSliderControlViewForParameterKeys:(VSCSParameter::KeyList)keys withFrame:(NSRect)frame;

@end
