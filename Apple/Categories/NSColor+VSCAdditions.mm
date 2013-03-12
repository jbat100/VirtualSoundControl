
#import "NSColor+VSCAdditions.h"


@implementation NSColor (VSCAdditions)

+(NSColor*)colorWithOgreRGBA:(const Ogre::RGBA &)ogreColor
{
    CGFloat r, g, b, a;
    r=g=b=a= 0.0;
    
    r = ((CGFloat)((ogreColor >> 24) | 0x000000FF)) / 255.0;
    BOOST_ASSERT(r >= 0.0 && r <= 1.0);
    g = ((CGFloat)((ogreColor >> 16) | 0x000000FF)) / 255.0;
    BOOST_ASSERT(g >= 0.0 && r <= 1.0);
    b = ((CGFloat)((ogreColor >> 8)  | 0x000000FF)) / 255.0;
    BOOST_ASSERT(b >= 0.0 && r <= 1.0);
    a = ((CGFloat)((ogreColor >> 0)  | 0x000000FF)) / 255.0;
    BOOST_ASSERT(a >= 0.0 && r <= 1.0);
    
    return [NSColor colorWithDeviceRed:r green:g blue:b alpha:a];
}

-(Ogre::RGBA) ogreRGBA
{
    uint32 rgba = 0;
    
    // need to make sure the color is in RGBA color space or we get crashes when attempting to access the components
    NSColor* rgbaColor = [self colorUsingColorSpaceName:NSDeviceRGBColorSpace];
    
    uint32_t r = (uint32_t)(MIN(1.0f, MAX(0.0f, [rgbaColor redComponent])) * 255.0f);
    BOOST_ASSERT(r <= 0x000000FF);
    uint32_t g = (uint32_t)(MIN(1.0f, MAX(0.0f, [rgbaColor greenComponent])) * 255.0f);
    BOOST_ASSERT(g <= 0x000000FF);
    uint32_t b = (uint32_t)(MIN(1.0f, MAX(0.0f, [rgbaColor blueComponent])) * 255.0f);
    BOOST_ASSERT(b <= 0x000000FF);
    uint32_t a = (uint32_t)(MIN(1.0f, MAX(0.0f, [rgbaColor alphaComponent])) * 255.0f);
    BOOST_ASSERT(a <= 0x000000FF);
    
    rgba = (r << 24) | (g << 16) | (b << 8) | a;
    
    return (Ogre::RGBA)rgba;
}

@end
