//
//  VSCSoundSourceSineWaveView.h
//  SynthStation
//
//  Created by Jonathan Thorpe on 11/11/2011.
//  Copyright 2011 NXP. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "VSCSoundSourceGeneratorDebugView.h"
#import "VSCSynthSourceSine.h"

@class VSCSynthSourceSineWaveParameterView;

@interface VSCSoundSourceSineWaveDebugView : VSCSoundSourceGeneratorDebugView {
	
	/*
	 *	http://codeblog.bsdninjas.co.uk/index.php?/archives/118-Using-polymorphism-with-boostshared_ptr.html
	 *	Should create a cast of the super (VSCSoundSourceGeneratorDebugView) sourceGenerator
	 */
	//VSCSynthSourceSinePtr sourceSine;

}

-(void) setSourceSine:(VSCSynthSourceSinePtr)sourceSine;
-(VSCSynthSourceSinePtr) getSourceSine;

/*
 *	Cast of VSCSynthSourceGeneratorControlView controlView property of
 *  VSCSoundSourceGeneratorDebugView superclass
 */
-(VSCSynthSourceSineWaveParameterView*) sineWaveParameterView;

@end
