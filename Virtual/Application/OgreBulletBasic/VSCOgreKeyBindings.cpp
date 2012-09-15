
#include "VSCOgreKeyBindings.h"

#include <boost/pointer_cast.hpp>

void VSCOgreKeyBound::setOgreKeyBindings(VSCOgreKeyBindings::SPtr keyBindings)
{
    BaseBindings::SPtr baseKeyBinding = boost::dynamic_pointer_cast<BaseBindings>(keyBindings);
    BaseBindings::setBindings(baseKeyBinding);
}

VSCOgreKeyBindings::SPtr VSCOgreKeyBound::getOgreKeyBindings(void)
{
    BaseBindings::SPtr baseKeyBinding = BaseBindings::getBindings();
    return boost::dynamic_pointer_cast<VSCOgreKeyBindings>(baseKeyBinding);
}



