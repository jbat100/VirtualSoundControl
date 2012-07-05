/***************************************************************************
This source file is part of OGREBULLET
(Object-oriented Graphics Rendering Engine Bullet Wrapper)
For the latest info, see http://www.ogre3d.org/phpBB2addons/viewforum.php?f=10
Copyright (c) 2007 tuan.kuranes@gmail.com (Use it Freely, even Statically, but have to contribute any changes)
This source file is not LGPL, it's public source code that you can reuse.
-----------------------------------------------------------------------------*/
/***************************************************************************
 File modified for VSC project
 -----------------------------------------------------------------------------*/

#ifndef _VSC_OGRE_BULLET_COCOA_INPUT_LISTENER_H_
#define _VSC_OGRE_BULLET_COCOA_INPUT_LISTENER_H_

#include <Ogre/Ogre.h>
#include "OIS.h"
#include "VSCOgreInputListener.h"

class VSCOgreBulletCocoaInputAdapter;

class VSCOgreBulletCocoaInputListener : public VSCOgreInputListener
{
public:

	// Constructor/destructor
    VSCOgreBulletCocoaInputListener(VSCOgreBulletScene *ogreBulletListener, Ogre::RenderWindow *window);
    virtual ~VSCOgreBulletCocoaInputListener(){};
    
    void setInputAdapter(VSCOgreBulletCocoaInputAdapter* adapter) {mCocoaInputAdapter = adapter;}
    VSCOgreBulletCocoaInputAdapter* getInputAdapter(void) {return mCocoaInputAdapter;}

private:

    VSCOgreBulletCocoaInputAdapter* mCocoaInputAdapter;
    
};

#endif//_VSC_OGRE_BULLET_COCOA_INPUT_LISTENER_H_
