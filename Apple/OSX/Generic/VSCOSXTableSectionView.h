//
//  VSCOSXTableSectionView.h
//  VSCApplication
//
//  Created by Jonathan Thorpe on 2/10/13.
//  Copyright (c) 2013 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface VSCOSXTableSectionView : NSView

+(CGFloat) defaultViewHeight;

@property (nonatomic, strong) IBOutlet NSTextField* nameTextField;

@end
