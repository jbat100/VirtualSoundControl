
#import <Cocoa/Cocoa.h>

@protocol VSCOSXOBSceneElementController;

@interface VSCOSXOBSceneElementDetailView : NSView

@property (assign) IBOutlet id<VSCOSXOBSceneElementController> elementController;

@property (weak) IBOutlet NSTextField* nameTextField;
@property (weak) IBOutlet NSTextField* idTextField;

/*
 *  Position/Rotation
 */

@property (weak) IBOutlet NSTextField* xPosTextField;
@property (weak) IBOutlet NSTextField* yPosTextField;
@property (weak) IBOutlet NSTextField* zPosTextField;

@property (weak) IBOutlet NSTextField* xVelTextField;
@property (weak) IBOutlet NSTextField* yVelTextField;
@property (weak) IBOutlet NSTextField* zVelTextField;

@property (weak) IBOutlet NSTextField* wRotTextField;
@property (weak) IBOutlet NSTextField* xRotTextField;
@property (weak) IBOutlet NSTextField* yRotTextField;
@property (weak) IBOutlet NSTextField* zRotTextField;

-(void) reloadInterface;

@end
