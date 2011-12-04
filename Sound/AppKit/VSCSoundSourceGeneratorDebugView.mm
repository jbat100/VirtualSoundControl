//
//  VSCSoundSourceGenerator.m
//  SynthStation
//
//  Created by Jonathan Thorpe on 06/11/2011.
//  Copyright 2011 NXP. All rights reserved.
//

#import "VSCSoundSourceGeneratorDebugView.h"
#import "VSCSoundGenerator.h"
#import "VSCSoundChannelLevelParameterView.h"
#import "VSCSoundParameters.h"

#import <boost/pointer_cast.hpp>


@implementation VSCSoundSourceGeneratorDebugView

@synthesize generatorParameterParentView, generatorParameterView;
@synthesize channelLevelParameterView, channelLevelParameterParentView;
@synthesize tickTableView, tickButton;
@synthesize generatorTitleTextField, tickCountTextField;


- (id)initWithFrame:(NSRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code here.
		[self customInit];
    }
    return self;
}

/*
- (void)drawRect:(NSRect)dirtyRect {
    // Drawing code here.
}
 */

-(void) customInit {
	
	
}

/*
 *	Which cast to use? See interesting discussion at:
 *	http://stackoverflow.com/questions/28002/regular-cast-vs-static-cast-vs-dynamic-cast
 *
 *	Gist of it is:
 *
 *	- static_cast is used for cases where you basically want to reverse an implicit conversion, 
 *	with a few restrictions and additions. static_cast performs no runtime checks. This should 
 *	be used if you know that you refer to an object of a specific type, and thus a check would be 
 *	unnecessary.
 *
 *	- dynamic_cast is used for cases where you don't know what the dynamic type of the object is. 
 *	You cannot use dynamic_cast if you downcast and the argument type is not polymorphic. 
 *	dynamic_cast returns a null pointer if the object referred to doesn't contain the type casted 
 *	to as a base class (when you cast to a reference, a bad_cast exception is thrown in that case). 
 *
 *	In this case we do not know what the class will be...
 */

-(void) setSynthSourceGenerator:(VSCSoundGeneratorPtr)generator {
	VSCSoundElementPtr element = boost::dynamic_pointer_cast<VSCSoundElement>(generator);
	if (generator && !element) 
		throw VSCSInvalidArgumentException();
	[self setSoundElement:element];
}

-(VSCSoundGeneratorPtr) getSoundElement {
	return boost::dynamic_pointer_cast<VSCSoundGenerator>(soundElement);
}


#pragma mark - NSTableViewDelegate

- (NSInteger)numberOfRowsInTableView:(NSTableView *)aTableView {
	
	if (sourceGenerator && sourceGenerator->fillPastFrames) {
		return sourceGenerator->numberOfPastFrames;
	}
	
}

- (id)tableView:(NSTableView *)aTableView objectValueForTableColumn:(NSTableColumn *)aTableColumn 
			row:(NSInteger)rowIndex {
	
	
	
}


@end
