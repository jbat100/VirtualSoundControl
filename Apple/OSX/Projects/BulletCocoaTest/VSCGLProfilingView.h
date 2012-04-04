//
//  VSCGLProfilingView.h
//  BulletCocoaTest
//
//  Created by Jonathan Thorpe on 13/06/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#import "VSCDocumentProtocol.h"


@interface VSCGLProfilingView : NSView {
	
	NSTextView* textView;
	
	id<VSCDocumentProtocol> delegate;

}

@property (assign) id<VSCDocumentProtocol> delegate;

@property (nonatomic, retain) IBOutlet NSTextView* textView;

@end
