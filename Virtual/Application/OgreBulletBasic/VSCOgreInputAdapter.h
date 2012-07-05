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
    
    VSCOgreInputAdapter(void);
    
    /*
     *  Add/Remove input listeners
     */
    void addInputListener(VSCOgreInputListener* listener);
    void removeInputListener(VSCOgreInputListener* listener);
    const std::set<VSCOgreInputListener*>& getInputListeners(void) {return mInputListeners;}
    
    bool areCoordinatesNormalized(void) {return mNormalizedCoordinates;}
    void normalizeCoordinates(bool norm) {mNormalizedCoordinates = norm;}
    
protected:

    std::set<VSCOgreInputListener*>     mInputListeners;
    bool                                mNormalizedCoordinates;
    
};

#endif//_VSC_OGRE_BULLET_INPUT_ADAPTER_H_

