
#ifndef _VSC_OGRE_KEY_BINDINGS_H_
#define _VSC_OGRE_KEY_BINDINGS_H_

#include "VSCUI.h"  
#include "VSCBindings.hpp"
#include "VSCBound.hpp"
#include "VSCOgreKeyboardAction.h"

#include <boost/shared_ptr.hpp>

#include <set>


class VSC::OB::KeyBindings : public VSC::Bindings<VSC::OB::KeyboardAction::Key, VSC::Keyboard::Combination>
{
    
public:
    
    typedef boost::shared_ptr<VSC::OB::KeyBindings> SPtr;
    
    VSC::OB::KeyBindings() {}
    virtual ~VSC::OB::KeyBindings() {}
    
};


class VSC::OB::KeyBound : private VSC::Bound<VSC::OB::KeyboardAction::Key, VSC::Keyboard::Combination> 
{
    
public:
    
    VSC::OB::KeyBound() {}
    virtual ~VSC::OB::KeyBound() {}
    
    void                        setOgreKeyBindings(VSC::OB::KeyBindings::SPtr keyBindings);
    VSC::OB::KeyBindings::SPtr    getOgreKeyBindings(void);
    
private:
    
    typedef VSC::Bindings<VSC::OB::KeyboardAction::Key, VSC::Keyboard::Combination> BaseBindings;
    
};
 
 

#endif//_VSC_OGRE_KEY_BINDINGS_H_

