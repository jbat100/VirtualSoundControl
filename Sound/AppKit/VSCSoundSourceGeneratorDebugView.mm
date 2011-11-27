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

- (void)drawRect:(NSRect)dirtyRect {
    // Drawing code here.
}

-(void) customInit {
	
	[self createChannelLevelParameterView];
	[self createGeneratorParameterView];
	
}

-(void) createChannelLevelParameterView {
	
	NSAssert(channelLevelParameterParentView, @"Expected non nil channelLevelParameterParentView");
	self.channelLevelParameterView = [[[VSCSoundChannelLevelParameterView alloc] 
									initWithFrame:channelLevelParameterParentView.bounds] autorelease];
	
	channelLevelParameterView.delegate = self;
	
	[channelLevelParameterParentView addSubview:channelLevelParameterView];
	
	
	
}

-(void) createGeneratorParameterView {
	// to subclass ...
}


/*
 *	C++ Setters/Getters
 */
-(VSCSynthSourceGeneratorPtr) getSourceGenerator {
	return sourceGenerator;
}

-(void) setSourceGenerator:(VSCSynthSourceGeneratorPtr)_sourceGenerator {
	sourceGenerator = _sourceGenerator;
	tickFrames.resize((size_t)tickDepth, (unsigned int)sourceGenerator->getNumberOfChannels());
}




#pragma mark - VSCSParameterAppleListener 

-(BOOL) interestedInParameterId:(VSCSParameterId)paramId {
	if (paramId.element == sourceGenerator.get()) {
		return YES;
	}
	return NO;
}

-(void) parameterWithKey:(VSCSParameter::Key)key 
			   changedTo:(double)value 
			  forElement:(VSCSoundParameterizedElement*)element {
	
	if (element != sourceGenerator.get()) return;
	
	[channelLevelParameterView setDoubleValue:value forParameterAtIndex:]
	
}


#pragma mark - VSCSPropertyAppleListener 

-(BOOL) interestedInPropertyId:(VSCSPropertyId)propId {
	if (propId.element == sourceGenerator.get()) {
		return YES;
	}
}

-(void) propertyWithKey:(VSCSProperty::Key)key 
			  changedTo:(std::string)value 
			 forElement:(VSCSoundPropertizedElement*)element {
	
	if (element != sourceGenerator.get()) return;
	
	[channelLevelParameterView ]
	
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
