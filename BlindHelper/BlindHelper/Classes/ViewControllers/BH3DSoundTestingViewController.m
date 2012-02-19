//
//  BH3DSoundTestingViewController.m
//  BlindHelper
//
//  Created by Jonathan Thorpe on 2/18/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "BH3DSoundTestingViewController.h"

#import "BH3DTestingSpatialView.h"
#import "BHOpenALPlayback.h"

@implementation BH3DSoundTestingViewController
@synthesize playPauseButton;
@synthesize musicSwitch;
@synthesize tiltListenerSwitch;


@synthesize playback, spatialView;

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)didReceiveMemoryWarning
{
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc that aren't in use.
}

#pragma mark - View lifecycle

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
}

- (void)viewDidUnload
{
    [self setPlayPauseButton:nil];
    [self setMusicSwitch:nil];
    [self setTiltListenerSwitch:nil];
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}

#pragma mark - Button Callbacks

- (IBAction)playPause:(UIButton *)sender
{
	// Toggle the playback
	
	if (self.playback.isPlaying) [self.playback stopSound];
	else [self.playback startSound];
	sender.selected = self.playback.isPlaying;
}


- (IBAction)musicSwitchValueChanged:(UISwitch*)sender {
    playback.usingMusic = !(playback.usingMusic);
}

- (IBAction)tiltSwitchValueChanged:(UISwitch*)sender {
    
    // Toggle the accelerometer
	// Note: With the accelerometer on, the device should be held vertically, not laid down flat.
	// As the device is rotated, the orientation of the listener will adjust so as as to be looking upward.
	if (sender.on)
	{
		[[UIAccelerometer sharedAccelerometer] setDelegate:self];
	} 
    else 
    {
		[[UIAccelerometer sharedAccelerometer] setDelegate:nil];
	}
    
}

#pragma mark - Accelerometer callbacks

- (void)accelerometer:(UIAccelerometer *)accelerometer didAccelerate:(UIAcceleration *)acceleration
{
	CGFloat zRot;
	
	// Find out the Z rotation of the device by doing some trig on the accelerometer values for X and Y
	zRot = (atan2(acceleration.x, acceleration.y) + M_PI);
	
	// Set our listener's rotation
	playback.listenerRotation = zRot;
}



@end
