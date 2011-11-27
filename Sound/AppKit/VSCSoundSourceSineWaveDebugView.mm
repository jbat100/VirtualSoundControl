//
//  VSCSoundSourceSineWaveView.m
//  SynthStation
//
//  Created by Jonathan Thorpe on 11/11/2011.
//  Copyright 2011 NXP. All rights reserved.
//

#import "VSCSoundSourceSineWaveDebugView.h"
#import "VSCSynthSourceSineWaveParameterView.h"

#import "VSCSynthSourceSine.h"
#import "VSCSynthSourceGenerator.h"

#include <boost/pointer_cast.hpp>

@implementation VSCSoundSourceSineWaveDebugView

/*
- (void)drawRect:(NSRect)dirtyRect {
    // Drawing code here.
}
 */

#pragma mark C++ Setters/Getters

/*
 * Override the source generator C++ setter/Getter
 */
-(VSCSynthSourceGeneratorPtr) getSourceGenerator {
	return [super getSourceGenerator];
}

-(void) setSourceGenerator:(VSCSynthSourceGeneratorPtr)_sourceGenerator {
	
	if (_sourceGenerator) {
		// if we have a non NULL _sourceGenerator, attempt to cast it to a sine source
		VSCSynthSourceSinePtr sourceSine = boost::dynamic_pointer_cast<VSCSynthSourceSine>(_sourceGenerator);
		NSAssert(sourceSine, @"Could not cast generator to sine source");
		if (!sourceSine) {
			_sourceGenerator = VSCSynthSourceGeneratorPtr();
		}
		[[self sineWaveParameterView] setSourceSine:sourceSine];
	}
	
	[super setSourceGenerator:_sourceGenerator];
	
}

/*
 *	Source sine C++ setter/Getter
 */
-(void) setSourceSine:(VSCSynthSourceSinePtr)sourceSine {
	VSCSynthSourceGeneratorPtr generator = boost::dynamic_pointer_cast<VSCSynthSourceGenerator>(sourceSine);
	[self setSourceGenerator:generator];
}

-(VSCSynthSourceSinePtr) getSourceSine {
	VSCSynthSourceGeneratorPtr generator = [self getSourceGenerator];
	VSCSynthSourceSinePtr sourceSine = boost::dynamic_pointer_cast<VSCSynthSourceSine>(generator);
	return sourceSine;
}


#pragma mark Accessor to generatorParameterView


-(VSCSynthSourceSineWaveParameterView*) sineWaveParameterView {
	NSAssert([generatorParameterView isKindOfClass:[VSCSynthSourceSineWaveParameterView class]],
			 @"generatorParameterView should be kind of class VSCSynthSourceSineWaveParameterView");
	return (VSCSynthSourceSineWaveParameterView*) generatorParameterView;
}

-(void) customInit {
	
	[super customInit];
	
	/*
	 
	 APPLE DOCS: loading nibs is different on Mac OS X than on iOS
	 
	 Listing 1-4 shows the process for getting the top-level objects of a nib file in Mac OS X. 
	 This method places a mutable array into the nameTable dictionary and associates it with 
	 the NSNibTopLevelObjects key. The nib-loading code looks for this array object and, if present, 
	 places the top-level objects in it. Because each object starts with a retain count of 1 before
	 it is added to the array, simply releasing the array is not enough to release the objects in the 
	 array as well. As a result, this method sends a release message to each of the objects to 
	 ensure that the array is the only entity holding a reference to them.
	
	
	NSBundle* aBundle = [NSBundle mainBundle];
    NSMutableArray* topLevelObjs = [NSMutableArray array];
    NSDictionary* nameTable = [NSDictionary dictionaryWithObjectsAndKeys:
							   self, NSNibOwner, topLevelObjs, NSNibTopLevelObjects, nil];
	
    if (![aBundle loadNibFile:@"VSCSoundSourceGeneratorDebugView" 
			externalNameTable:nameTable withZone:nil])
    {
        NSLog(@"Warning! Could not load myNib file.\n");
        return nil;
    }
	
    // Release the objects so that they are just owned by the array.
    [topLevelObjs makeObjectsPerformSelector:@selector(release)];
	
    return topLevelObjs;
	 
	 */

	
	
}

-(void) createGeneratorParameterView {
	
	if (generatorParameterView) {
		[generatorParameterView removeFromSuperview];
		self.generatorParameterView = nil;
	}
	
	/*
	 *	Sinewave parameter control
	 */
	NSAssert(generatorParameterParentView, @"Expected non nil generatorParameterParentView");
	self.generatorParameterView = [[[VSCSynthSourceSineWaveParameterView alloc] 
									initWithFrame:generatorParameterParentView.bounds] autorelease];
	[generatorParameterParentView addSubview:generatorParameterView];
	[[self sineWaveControlView] refreshInterface];
	
}

@end
