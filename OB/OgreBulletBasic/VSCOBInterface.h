
#ifndef _VSC_OB_INTERFACE_H_
#define _VSC_OB_INTERFACE_H_

#include "VSCInterfaceAdapter.h"

#include <Ogre/Ogre.h>
#include "OIS.h"

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

namespace VSC {
    
    namespace OB {
        
        class VSC::InterfaceAdapter<Ogre::RenderWindow, Ogre::Vector2> InterfaceAdapter;

        class VSC::InterfaceAdapter<Ogre::RenderWindow, Ogre::Vector2>::Responder Responder;
        
    }
}

#endif //_VSC_OB_INTERFACE_H_

