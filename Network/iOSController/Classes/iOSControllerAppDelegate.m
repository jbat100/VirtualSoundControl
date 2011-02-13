//
//  iOSControllerAppDelegate.m
//  iOSController
//
//  Created by Jonathan Thorpe on 15/01/2011.
//  Copyright JBAT 2011. All rights reserved.
//

#import "iOSControllerAppDelegate.h"
#import "ServerController.h"
#import "ControlStation.h"

@implementation iOSControllerAppDelegate

@synthesize window;
@synthesize tabBarController;

+(iOSControllerAppDelegate*) sharedAppDelegate {
	return (iOSControllerAppDelegate*)[UIApplication sharedApplication].delegate;
}


- (void)applicationDidFinishLaunching:(UIApplication *)application {
    
    // Add the tab bar controller's current view as a subview of the window
    [window addSubview:tabBarController.view];
	
	[[ServerController instance] startService];
	
	[ControlStation instance].updateInterval = 0.01;
	[[ControlStation instance] startBroadcasting];
	
}

-(void) applicationWillEnterForeground:(UIApplication *)application {
	
	[[ControlStation instance] startBroadcasting];
	
}


-(void) applicationDidEnterBackground:(UIApplication *)application {
	
	//[[ServerController instance] stopService];
	
	[[ControlStation instance] stopBroadcasting];
	
}


-(void) applicationDidBecomeActive:(UIApplication *)application {
	
	[[ControlStation instance] startBroadcasting];
	
}


-(void) applicationWillResignActive:(UIApplication *)application {
	
	//[[ServerController instance] stopService];
	
	[[ControlStation instance] stopBroadcasting];
	
}


-(void) applicationWillTerminate:(UIApplication *)application {
	
	[[ControlStation instance] stopBroadcasting];
	
	[[ServerController instance] stopService];
	
}

/*
// Optional UITabBarControllerDelegate method
- (void)tabBarController:(UITabBarController *)tabBarController didSelectViewController:(UIViewController *)viewController {
}
*/

/*
// Optional UITabBarControllerDelegate method
- (void)tabBarController:(UITabBarController *)tabBarController didEndCustomizingViewControllers:(NSArray *)viewControllers changed:(BOOL)changed {
}
*/


- (void)dealloc {
    [tabBarController release];
    [window release];
    [super dealloc];
}

-(NSString*) settingsFilePath {
	
	NSArray *searchPaths = NSSearchPathForDirectoriesInDomains (NSDocumentDirectory, NSUserDomainMask, YES);
	NSString* folderPath = [searchPaths objectAtIndex: 0];
	return [folderPath stringByAppendingPathComponent:@"Settings.dic"];
	
}

@end

