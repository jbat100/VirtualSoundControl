//
//  VSCAppDelegate.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 5/1/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCAppDelegate.h"
#import <vector>

#import "VSCOgreBulletApplication.h"

#import "VSCOgreBulletListener.h"
#import "VSCOgreBulletPrimitivesDemo.h"
#import "VSCOgreBulletTriMeshDemo.h"
#import "VSCOgreBulletVehiclesDemo.h"
#import "VSCOgreBulletConstraintsDemo.h"
#import "VSCOgreBulletTerrainDemo.h
#import "VSCOgreBulletRagdollDemo.h"

@implementation VSCAppDelegate

@synthesize window = _window;

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    
    std::vector<VSCOgreBulletListener*> ogrebulletscenes;
    
    ogrebulletscenes.push_back(new VSCOgreBulletPrimitivesDemo());
    ogrebulletscenes.push_back(new VSCOgreBulletTriMeshDemo());
	ogrebulletscenes.push_back(new VSCOgreBulletVehiclesDemo());
	ogrebulletscenes.push_back(new VSCOgreBulletConstraintsDemo());
	ogrebulletscenes.push_back(new VSCOgreBulletTerrainDemo());
	ogrebulletscenes.push_back(new VSCOgreBulletRagdollDemo());
    
    ogrebulletscenes[0]->setNextKey(KC_F1);
    ogrebulletscenes[1]->setNextKey(KC_F2);
	ogrebulletscenes[2]->setNextKey(KC_F3);
	ogrebulletscenes[3]->setNextKey(KC_F4);
	ogrebulletscenes[4]->setNextKey(KC_F5);
	ogrebulletscenes[5]->setNextKey(KC_F6);
    
    // Create the application and try to run it
    VSCOgreBulletApplication app(&ogrebulletscenes);
    
    try
    {
        app.go();
    } 
    catch(Ogre::Exception& e)
    {
        std::cerr << "An exception has occured: " << e.getFullDescription().c_str() << std::endl;
    }
    
    /*
     *  Cleanup
     */
    std::vector<VSCOgreBulletListener*>::iterator it =  ogrebulletscenes.begin();
    while (it != ogrebulletscenes.end())
    {
        delete *it;
        ++it;
    }
    
}

@end
