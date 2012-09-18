
#ifndef _VSC_KEY_BINDINGS_H_
#define _VSC_KEY_BINDINGS_H_

#include "VSCUI.h"
#include "VSCBindings.hpp"

#include <boost/shared_ptr.hpp>

namespace VSC {

    template <typename Action, typename Input>
    class Bound
    {
        
    public:
        
        Bound() {}
        virtual ~Bound() {}
        
        typename Bindings<Action,Input>::SPtr    getBindings() {return mBindings;}
        void                                        setBindings(typename Bindings<Action,Input>::SPtr bindings) {mBindings = bindings;}
        
    private:
        
        typename Bindings<Action,Input>::SPtr    mBindings;
        
    };
    
}


#endif//_VSC_KEY_BINDINGS_H_

