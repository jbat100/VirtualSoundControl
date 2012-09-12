
#ifndef _VSC_OGRE_KEYBOARD_MANAGED_H_
#define _VSC_OGRE_KEYBOARD_MANAGED_H_


#include "VSCOgreKeyboardAction.h"

/*
 *  Chain of responsability pattern for objects which use a keyboard manager
 */

class VSCOgreKeyboardManager;

class VSCOgreKeyboardManaged
{
    
public:
    
    VSCOgreKeyboardManaged();
    virtual ~VSCOgreKeyboardManaged();
    
    VSCOgreKeyboardManager*     getKeyboardManager(void);
    void                        setKeyboardManager(VSCOgreKeyboardManager* manager);
    
private:
    
    VSCOgreKeyboardManager*     mKeyboardManager;
        
    VSCOgreKeyboardManaged*     mNextKeyboardManaged;
    
};

#endif//_VSC_OGRE_KEYBOARD_MANAGER_H_

