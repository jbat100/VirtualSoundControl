/***************************************************************************
This source file is part of OGREBULLET
(Object-oriented Graphics Rendering Engine Bullet Wrapper)
For the latest info, see http://www.ogre3d.org/phpBB2addons/viewforum.php?f=10
Copyright (c) 2007 tuan.kuranes@gmail.com (Use it Freely, even Statically, but have to contribute any changes)
This source file is not LGPL, it's public source code that you can reuse.
-----------------------------------------------------------------------------*/

#ifndef _VSC_OGREBULLET_RAGDOLL_DEMO_H_
#define _VSC_OGREBULLET_RAGDOLL_DEMO_H_

#include "OgreBulletDynamics.h"

#include "VSCOBScene.h"

namespace VSC {
    
    namespace OB {

        // -------------------------------------------------------------------------
        class RagdollDemo : public Scene
        {
        public:
            
            RagdollDemo() : Scene() {setName("Ragdoll Demo");}
            virtual ~RagdollDemo(){}
            
            void init(Ogre::Root *root, Ogre::RenderWindow *win);
            
            bool keyPressed(Ogre::RenderWindow* renderWindow, OIS::KeyCode key);
            
            void shootToKill();
            
            void button0Pressed();
            
            bool frameStarted(Ogre::Real elapsedTime);
            bool frameEnded(Ogre::Real elapsedTime);
            
        };
        
    }
}

#endif //_VSC_OGREBULLET_RAGDOLL_DEMO_H_
