//
//  VSCOSXKeyedSliderView.h
//  VSCApplication
//
//  Created by Jonathan Thorpe on 1/18/13.
//  Copyright (c) 2013 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface VSCOSXKeyedSliderView : NSView

@property (nonatomic, weak) NSSlider* slider;
@property (nonatomic, weak) NSTextField* valueTextField;
@property (nonatomic, weak) NSTextField* labelTextField;

@end
