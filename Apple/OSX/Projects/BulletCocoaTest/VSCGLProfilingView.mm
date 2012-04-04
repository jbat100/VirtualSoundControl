//
//  VSCGLProfilingView.mm
//  BulletCocoaTest
//
//  Created by Jonathan Thorpe on 13/06/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import "VSCGLProfilingView.h"


@implementation VSCGLProfilingView

@synthesize textView, delegate;

- (id)initWithFrame:(NSRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code here.
    }
    return self;
}

-(void) updateProfiling {
	
	if ([delegate rootApplication]) {
		
	}
	else {
		[textView setString:@""];
	}

	
}

@end
