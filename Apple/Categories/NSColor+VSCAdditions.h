
#import <Cocoa/Cocoa.h>

#import <Ogre/Ogre.h>

@interface NSColor (VSCAdditions)

+(NSColor*) colorWithOgreRGBA:(const Ogre::RGBA &)ogreColor;

-(Ogre::RGBA) ogreRGBA;

@end
