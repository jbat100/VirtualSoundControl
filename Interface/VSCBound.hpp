
#ifndef _VSC_KEY_BINDINGS_H_
#define _VSC_KEY_BINDINGS_H_

#include "VSCUI.h"
#include "VSCBindings.hpp"

#include <boost/shared_ptr.hpp>

template <typename Action, typename Input>
class VSC::Bound 
{
    
public:
    
    VSC::Bound() {}
    virtual ~VSC::Bound() {}
    
    typename VSC::Bindings<Action,Input>::SPtr    getBindings() {return mBindings;}
    void                                        setBindings(typename VSC::Bindings<Action,Input>::SPtr bindings) {mBindings = bindings;}
    
private:
    
    typename VSC::Bindings<Action,Input>::SPtr    mBindings;
    
};


#endif//_VSC_KEY_BINDINGS_H_

