
#ifndef _VSC_OGRE_KEY_BINDINGS_H_
#define _VSC_OGRE_KEY_BINDINGS_H_

#include "VSCUI.h"  
#include "VSCBindings.h"
#include "VSCBound.h"
#include "VSCOgreKeyboardAction.h"

#include <boost/shared_ptr.hpp>

#include <set>


class VSCOgreKeyBindings : public VSCBindings<VSCOgreKeyboardAction::Key, VSCKeyboard::Combination>
{
    
public:
    
    typedef boost::shared_ptr<VSCOgreKeyBindings> SPtr;
    
    VSCOgreKeyBindings() {}
    virtual ~VSCOgreKeyBindings() {}
    
private:
    
    typedef VSCBindings<VSCOgreKeyboardAction::Key, VSCKeyboard::Combination> BaseBindings;
    
}


class VSCOgreKeyBound : private VSCBound<VSCOgreKeyboardAction::Key, VSCKeyboard::Combination> 
{
    
public:
    
    VSCOgreKeyBound() {}
    virtual ~VSCOgreKeyBound() {}
    
    void                        setOgreKeyBindings(VSCOgreKeyBindings::SPtr keyBindings);
    VSCOgreKeyBindings::SPtr    getOgreKeyBindings(void);
    
};
 
 

#endif//_VSC_OGRE_KEY_BINDINGS_H_

