//
//  VSCSoundSourceGenerator.h
//  SynthStation
//
//  Created by Jonathan Thorpe on 06/11/2011.
//  Copyright 2011 NXP. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#import "Stk.h"
#import "VSCSoundGenerator.h"
#import "VSCSoundElementView.h"


/*
 *	I don't think this view should be subclassed, the sound element contained in the
 *	soundElementView determines the interface and behaviour (accessed by setSoundGenerator)
 *	and getSoundGenerator
 */

@interface VSCSoundSourceGeneratorDebugView : NSView  {
	
	VSCSoundElementView* soundElementView;
    
    NSScrollView* soundElementScrollView;
	
	NSTextField* generatorTitleTextField;
	NSTextField* tickCountTextField;
	NSButton* tickButton;
	
	/*
	 *	This table view is used to display the tick data generated by the 
	 *	generator, the last tickDepth frames will be shown in the NSTableView
	 */
	NSTableView* tickTableView;

}

@property (nonatomic, retain) IBOutlet VSCSoundElementView* soundElementView;

@property (nonatomic, retain) IBOutlet NSScrollView* soundElementScrollView;

@property (nonatomic, retain) IBOutlet NSTextField* generatorTitleTextField;
@property (nonatomic, retain) IBOutlet NSTextField* tickCountTextField;

@property (nonatomic, retain) IBOutlet NSButton* tickButton;
@property (nonatomic, retain) IBOutlet NSTableView* tickTableView;

-(void) setSoundGenerator:(VSCSoundGeneratorPtr)generator;
-(VSCSoundGeneratorPtr) getSoundGenerator;

-(IBAction) tickButtonClicked:(id)sender;

/*
 *	Custom initialization, particularly creating the approproate control subview for
 *	generatorControlParentView.
 */
-(void) customInit;


@end
