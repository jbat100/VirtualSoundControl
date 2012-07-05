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

/*
InputListener.h
-------------
A basic test framework that minimize code in each test scene listener.
*/
#ifndef _VSC_OGRE_BULLET_INPUT_ADAPTER_H_
#define _VSC_OGRE_BULLET_INPUT_ADAPTER_H_

#include <set>

class VSCOgreInputListener;

class VSCOgreInputAdapter
{
    
public:
    
    /*
     *  Add/Remove input listeners
     */
    void addOgreBulletInputListener(VSCOgreInputListener* listener);
    void removeOgreBulletInputListener(VSCOgreInputListener* listener);
    const std::set<VSCOgreInputListener*>& getOgreBulletInputListeners(void) {return mInputListeners;}
    
private:

    std::set<VSCOgreInputListener*>   mInputListeners;
    
};

#endif//_VSC_OGRE_BULLET_INPUT_ADAPTER_H_

