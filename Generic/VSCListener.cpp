
#include "VSCListener.h"

bool VSC::operator< (const Listener::WPtr& lhs, const Listener::WPtr& rhs)
{
    Listener::SPtr slhs = lhs.lock();
    Listener::SPtr srhs = rhs.lock();
    
    return slhs < srhs;
}

bool VSC::operator== (const Listener::WPtr& lhs, const Listener::WPtr& rhs)
{
    Listener::SPtr slhs = lhs.lock();
    Listener::SPtr srhs = rhs.lock();
    
    return slhs == srhs;
}