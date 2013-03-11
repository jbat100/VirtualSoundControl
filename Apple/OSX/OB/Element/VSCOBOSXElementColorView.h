//
//  VSCOBOSXElementColorView.h
//  VSCApplication
//
//  Created by Jonathan Thorpe on 3/11/13.
//  Copyright (c) 2013 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@protocol VSCOBOSXElementController;

@interface VSCOBOSXElementColorView : NSView

@property (assign) IBOutlet id<VSCOBOSXElementController> elementController;

@property (nonatomic, strong) NSButton* randomizeVertexDiffuseColorsButton;
@property (nonatomic, strong) NSButton* printVertexDiffuseColorsButton;
@property (nonatomic, strong) NSButton* selectVertexDiffuseColorButton;

@end
