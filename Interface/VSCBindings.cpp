
#include "VSCBindings.h"
#include "VSCException.h"

#include <boost/foreach.hpp>

template<typename Action, typename Input>
const std::set<Action>& VSCBindings::getActionsForCombination(const Input& input)
{
    ActionInputsMap::iterator it = mInputActionsMap.find(input);
    
    if (it == mInputActionsMap.end())
    {
        /*
         *  If we don't have anything, then create an empty set and return that.
         *  NOTE: DON'T just create a local and return it as it would go out of
         *  scope and be destroyed (potentially) as soon as the function returns.
         */
        
        std::set<Action> actionSet;
        mInputActionsMap[input] = actionSet;
        
        it = mInputActionsMap.find(input);
    }
    
    return it->second;
}

template<typename Action, typename Input>
const std::set<Input>& VSCBindings::getInputsForAction(const Action& action)
{
    InputActionsMap::iterator it = mActionInputsMap.find(action);
    
    if (it == mInputActionsMap.end())
    {
        /*
         *  If we don't have anything, then create an empty set and return that.
         *  NOTE: DON'T just create a local and return it as it would go out of
         *  scope and be destroyed (potentially) as soon as the function returns.
         */
        
        std::set<Input> inputSet;
        mActionInputsMap[action] = inputSet;
        
        it = mActionInputsMap.find(action);
    }
    
    return it->second;
}

template<typename Action, typename Input>
void VSCBindings::eraseBinding(const Action& action, const Input& input)
{
    InputActionsMap::iterator it = mActionInputsMap.find(action);
    
    if (it != mActionInputsMap.end())
    {
        it->second.erase(input);
    }
    
    ActionInputsMap::iterator it = mInputActionsMap.find(input);
    
    if (it != mInputActionsMap.end())
    {
        it->second.erase(action);
    }    
}

template<typename Action, typename Input>
void VSCBindings::eraseInputBindings(const Input& input)
{
    mInputActionsMap.erase(input);
    
    BOOST_FOREACH (std::set<Action> inputListener, this->getInputListeners()) 
    {
        inputListener->mouseMoved(position, movement);
    }
}

template<typename Action, typename Input>
void VSCBindings::eraseActionBindings(const Action& action)
{
    BOOST_FOREACH (VSCOgreInputListener* inputListener, this->getInputListeners()) 
    {
        inputListener->mouseMoved(position, movement);
    }
}

template<typename Action, typename Input>
void VSCBindings::setBinding(const Action& action, const Input& input) 
{
    
}



