//
//  iOSControllerAppDelegate.h
//  iOSController
//
//  Created by Jonathan Thorpe on 15/01/2011.
//  Copyright JBAT 2011. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface iOSControllerAppDelegate : NSObject <UIApplicationDelegate, UITabBarControllerDelegate> {
    UIWindow *window;
    UITabBarController *tabBarController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet UITabBarController *tabBarController;

+(iOSControllerAppDelegate*) sharedAppDelegate;

-(NSString*) settingsFilePath;

@end
