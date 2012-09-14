
#ifndef _VSC_KEY_BINDINGS_H_
#define _VSC_KEY_BINDINGS_H_

#include "VSCUI.h"
#include "VSCBindings.h"

#include <boost/shared_ptr.hpp>

template <typename Action, typename Input>
class VSCBound 
{
    
public:
    
    VSCBound();
    virtual ~VSCBound();
    
    VSCBindings<Action,Input>::SPtr         getBindings() {return mBindings;}
    void                                    setBindings(VSCBindings<Action,Input>::SPtr bindings) {mBindings = bindings;}
    
private:
    
    VSCBindings<Action,Input>::SPtr         mBindings;
    
};


#endif//_VSC_KEY_BINDINGS_H_

