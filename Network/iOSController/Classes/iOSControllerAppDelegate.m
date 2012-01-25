//
//  iOSControllerAppDelegate.m
//  iOSController
//
//  Created by Jonathan Thorpe on 15/01/2011.
//  Copyright JBAT 2011. All rights reserved.
//

#import "iOSControllerAppDelegate.h"
#import "SetupManager.h"
#import "SetupsViewController.h"
#import "ServerController.h"
#import "ControlStation.h"

@implementation iOSControllerAppDelegate

@synthesize window, tabBarController;
@synthesize setupManager;

+(iOSControllerAppDelegate*) sharedAppDelegate {
	return (iOSControllerAppDelegate*)[UIApplication sharedApplication].delegate;
}


- (void)applicationDidFinishLaunching:(UIApplication *)application {
    
    // Add the tab bar controller's current view as a subview of the window
    [window addSubview:tabBarController.view];
	
	[self loadSetupManager];
	[self getSetupsViewController].setupManager = setupManager;
	
	[[ServerController instance] startService];
	
	[ControlStation instance].updateInterval = 0.01;
	[[ControlStation instance] startBroadcasting];
	
}

-(void) applicationWillEnterForeground:(UIApplication *)application {
	
	[[ControlStation instance] startBroadcasting];
	
}


-(void) applicationDidEnterBackground:(UIApplication *)application {
	
	[self saveSetupManager];
	
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

-(SetupsViewController*) getSetupsViewController {
	
	NSInteger setupsViewTabIndex = 1;
	
	UINavigationController* navigationController = (UINavigationController*)[[tabBarController viewControllers] 
																			 objectAtIndex:setupsViewTabIndex];
	
	return [[[navigationController objectAtIndex:setupsViewTabIndex] viewControllers] objectAtIndex:0];
	
}



-(NSString*) settingsFilePath {
	
	NSArray *searchPaths = NSSearchPathForDirectoriesInDomains (NSDocumentDirectory, NSUserDomainMask, YES);
	NSString* folderPath = [searchPaths objectAtIndex: 0];
	return [folderPath stringByAppendingPathComponent:@"Settings.arc"];
	
}

-(void) loadSetupManager {
	
	if ([[NSFileManager defaultManager] fileExistsAtPath:[self setupManagerFilePath]])
		self.setupManager = [NSKeyedUnarchiver unarchiveObjectWithFile:[self setupManagerFilePath]];
	else 
		self.setupManager = [[[SetupManager alloc] init] autorelease];
	
}

-(void) saveSetupManager {
	
	if (setupManager)
		[NSKeyedArchiver archiveRootObject:setupManager toFile:[self setupManagerFilePath]];
	else 
		[[NSFileManager defaultManager] removeItemAtPath:[self setupManagerFilePath] error:nil];
	
}

-(NSString*) setupManagerFilePath {
	
	NSArray *searchPaths = NSSearchPathForDirectoriesInDomains (NSDocumentDirectory, NSUserDomainMask, YES);
	NSString* folderPath = [searchPaths objectAtIndex: 0];
	return [folderPath stringByAppendingPathComponent:@"SetupManager.arc"];
	
}



@end

