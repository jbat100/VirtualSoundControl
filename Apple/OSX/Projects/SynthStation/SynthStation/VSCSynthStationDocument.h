//
//  SynthStationDocument.h
//  SynthStation
//
//  Created by Jonathan Thorpe on 24/10/2011.
//  Copyright 2011 NXP. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@class VSCSynthStationWindowController;


@interface VSCSynthStationDocument : NSDocument {
	
	VSCSynthStationWindowController* synthStationWindowController;

}

@end
