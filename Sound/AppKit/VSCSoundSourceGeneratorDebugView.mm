//
//  VSCSoundSourceGenerator.m
//  SynthStation
//
//  Created by Jonathan Thorpe on 06/11/2011.
//  Copyright 2011 NXP. All rights reserved.
//

#import "VSCSoundSourceGeneratorDebugView.h"
#import "VSCSynthSourceGenerator.h"
#import "VSCSoundChannelLevelParameterView.h"


@implementation VSCSoundSourceGeneratorDebugView

@synthesize generatorParameterParentView, generatorParameterView;
@synthesize channelLevelParameterView, channelLevelParameterParentView;
@synthesize tickTableView, tickButton, tickDepth;
@synthesize generatorTitleTextField, tickCountTextField;


- (id)initWithFrame:(NSRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code here.
		[self customInit];
    }
    return self;
}

- (void)drawRect:(NSRect)dirtyRect {
    // Drawing code here.
}

-(void) customInit {
	
	self.tickDepth = 10;
	
	[self createChannelLevelParameterView];
	[self createGeneratorParameterView];
	
}

-(void) createChannelLevelParameterView {
	
	NSAssert(channelLevelParameterParentView, @"Expected non nil channelLevelParameterParentView");
	self.channelLevelParameterView = [[[VSCSoundChannelLevelParameterView alloc] 
									initWithFrame:channelLevelParameterParentView.bounds] autorelease];
	[channelLevelParameterParentView addSubview:channelLevelParameterView];
	
	
	
}

-(void) createGeneratorParameterView {
	
}

-(void) setTickDepth:(NSInteger)depth {
	
	tickDepth = depth;
	tickFrames.resize(tickDepth, tickFrames.channels());
}

/*
 *	C++ Setters/Getters
 */
-(VSCSynthSourceGeneratorPtr) getSourceGenerator {
	return sourceGenerator;
}

-(void) setSourceGenerator:(VSCSynthSourceGeneratorPtr)_sourceGenerator {
	sourceGenerator = _sourceGenerator;
	tickFrames.resize(tickDepth, sourceGenerator->getNumberOfChannels);
}

/*
 *	Get a copy of the current tickFrames
 */
-(stk::StkFrames) tickFramesCopy {
	// make a copy of the frames (overloaded = operator)
	stk::StkFrames newFrames = tickFrames;
	return newFrames;
}

@end
