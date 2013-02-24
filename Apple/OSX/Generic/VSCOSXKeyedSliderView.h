//
//  VSCOSXKeyedSliderView.h
//  VSCApplication
//
//  Created by Jonathan Thorpe on 1/18/13.
//  Copyright (c) 2013 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface VSCOSXKeyedSliderView : NSView

@property (nonatomic, weak) IBOutlet NSSlider* slider;
@property (nonatomic, weak) IBOutlet NSTextField* valueTextField;
@property (nonatomic, weak) IBOutlet NSTextField* labelTextField;

@end
