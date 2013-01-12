
#ifndef _VSC_OB_INTERFACE_H_
#define _VSC_OB_INTERFACE_H_

#include "VSCInterfaceAdapter.h"
#include <Ogre/Ogre.h>
#include <vector>

namespace VSC {
    
    namespace OB {
        
        typedef VSC::InterfaceAdapter<Ogre::RenderWindow, Ogre::Vector2> InterfaceAdapter;

        typedef VSC::InterfaceAdapter<Ogre::RenderWindow, Ogre::Vector2>::Responder InterfaceResponder;
        
        typedef VSC::InterfaceAdapter<Ogre::RenderWindow, Ogre::Vector2>::ResponderChain InterfaceResponderChain;
        
        typedef std::vector<InterfaceResponder::SPtr> InterfaceResponders;
    }
}

#endif //_VSC_OB_INTERFACE_H_

