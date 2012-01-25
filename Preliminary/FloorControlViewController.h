//
//  FloorViewController.h
//  MeshFrames3
//
//  Created by Jonathan Thorpe on 13/05/2010.
//  Copyright 2010 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface FloorControlViewController : NSViewController <NSTextFieldDelegate> {
	
	
	IBOutlet NSButton *enabledButton;
	
	
	IBOutlet NSTextField *lengthTextField;
	IBOutlet NSTextField *detailTextField;
	IBOutlet NSTextField *lengthLabel;
	IBOutlet NSTextField *detailLabel;
	
	IBOutlet NSTextField *xCoordinateTextField;
	IBOutlet NSTextField *yCoordinateTextField;
	IBOutlet NSTextField *zCoordinateTextField;
	IBOutlet NSTextField *xCoordinateLabel;
	IBOutlet NSTextField *yCoordinateLabel;
	IBOutlet NSTextField *zCoordinateLabel;
	
	IBOutlet NSTextField *xNormalVectorTextField;
	IBOutlet NSTextField *yNormalVectorTextField;
	IBOutlet NSTextField *zNormalVectorTextField;
	IBOutlet NSTextField *xNormalVectorLabel;
	IBOutlet NSTextField *yNormalVectorLabel;
	IBOutlet NSTextField *zNormalVectorLabel;
	

}

@end
