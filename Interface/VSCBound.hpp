
#ifndef _VSC_KEY_BINDINGS_H_
#define _VSC_KEY_BINDINGS_H_

#include "VSCUI.h"
#include "VSCBindings.hpp"

#include <boost/shared_ptr.hpp>

template <typename Action, typename Input>
class VSCBound 
{
    
public:
    
    VSCBound() {}
    virtual ~VSCBound() {}
    
    typename VSCBindings<Action,Input>::SPtr    getBindings() {return mBindings;}
    void                                        setBindings(typename VSCBindings<Action,Input>::SPtr bindings) {mBindings = bindings;}
    
private:
    
    typename VSCBindings<Action,Input>::SPtr    mBindings;
    
};


#endif//_VSC_KEY_BINDINGS_H_

