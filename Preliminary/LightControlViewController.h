//
//  LightControlView.h
//  MeshFrames3
//
//  Created by Jonathan Thorpe on 05/04/2010.
//  Copyright 2010 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "JBAT_Light.h"


@protocol LightControlViewControllerDataSource

-(JBAT_Light *) light;

@end


@interface LightControlViewController : NSViewController <NSTextFieldDelegate> {
	
	IBOutlet NSButton *enabledButton;
	
	IBOutlet NSBox *coordinateBox;
	IBOutlet NSTextField *xCoordinateTextField;
	IBOutlet NSTextField *yCoordinateTextField;
	IBOutlet NSTextField *zCoordinateTextField;
	IBOutlet NSTextField *xCoordinateLabel;
	IBOutlet NSTextField *yCoordinateLabel;
	IBOutlet NSTextField *zCoordinateLabel;
	
	IBOutlet NSBox *colorBox;
	IBOutlet NSButton *ambientColorButton;
	IBOutlet NSButton *diffuseColorButton;
	IBOutlet NSButton *specularColorButton;
	IBOutlet NSTextField *ambientLabel;
	IBOutlet NSTextField *diffuseLabel;
	IBOutlet NSTextField *specularLabel;
	
	IBOutlet NSBox *directionBox;
	IBOutlet NSButton *spotlightButton;
	IBOutlet NSSlider *spotlightAngleSlider;
	IBOutlet NSTextField *xDirectionTextField;
	IBOutlet NSTextField *yDirectionTextField;
	IBOutlet NSTextField *zDirectionTextField;
	IBOutlet NSTextField *xDirectionLabel;
	IBOutlet NSTextField *yDirectionLabel;
	IBOutlet NSTextField *zDirectionLabel;
	
	id<LightControlViewControllerDataSource> dataSource;

}

@property (assign) id<LightControlViewControllerDataSource> dataSource;

-(void) updateInterface;
-(void) enableInterface:(BOOL)enable;

-(IBAction) buttonClicked:(id)sender;
-(IBAction) sliderMoved:(id)sender;

-(void) colorPanelAmbientSelection:(id)sender;
-(void) colorPanelDiffuseSelection:(id)sender;
-(void) colorPanelSpecularSelection:(id)sender;

-(NSColor *) ambientColor;
-(NSColor *) diffuseColor;
-(NSColor *) specularColor;

@end
