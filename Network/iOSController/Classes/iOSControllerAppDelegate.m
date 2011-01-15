//
//  iOSControllerAppDelegate.m
//  iOSController
//
//  Created by Jonathan Thorpe on 15/01/2011.
//  Copyright JBAT 2011. All rights reserved.
//

#import "iOSControllerAppDelegate.h"


@implementation iOSControllerAppDelegate

@synthesize window;
@synthesize tabBarController;

+(iOSControllerAppDelegate*) sharedAppDelegate {
	return (iOSControllerAppDelegate*)[UIApplication sharedApplication].delegate;
}


- (void)applicationDidFinishLaunching:(UIApplication *)application {
    
    // Add the tab bar controller's current view as a subview of the window
    [window addSubview:tabBarController.view];
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

@end

