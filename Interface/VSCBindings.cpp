

#include "VSCException.h"
#include <boost/foreach.hpp>

template<typename Action, typename Input>
const VSCBindings::ActionSet& VSCBindings::getActionsForInput(const Input& input)
{
    InputActionsMap::iterator it = mInputActionsMap.find(input);
    
    if (it == mInputActionsMap.end())
    {
        /*
         *  If we don't have anything, then create an empty set and return that.
         *  NOTE: DON'T just create a local and return it as it would go out of
         *  scope and be destroyed (potentially) as soon as the function returns.
         */
        
        ActionSet actionSet;
        mInputActionsMap[input] = actionSet;
        
        it = mInputActionsMap.find(input);
    }
    
    return it->second;
}

template<typename Action, typename Input>
const VSCBindings::InputSet& VSCBindings::getInputsForAction(const Action& action)
{
    ActionInputsMap::iterator it = mActionInputsMap.find(action);
    
    if (it == mInputActionsMap.end())
    {
        /*
         *  If we don't have anything, then create an empty set and return that.
         *  NOTE: DON'T just create a local and return it as it would go out of
         *  scope and be destroyed (potentially) as soon as the function returns.
         */
        
        InputSet inputSet;
        mActionInputsMap[action] = inputSet;
        
        it = mActionInputsMap.find(action);
    }
    
    return it->second;
}

template<typename Action, typename Input>
void VSCBindings::eraseBinding(const Action& action, const Input& input)
{
    ActionInputsMap::iterator it = mActionInputsMap.find(action);
    
    if (it != mActionInputsMap.end())
    {
        it->second.erase(input);
    }
    
    InputActionsMap::iterator it = mInputActionsMap.find(input);
    
    if (it != mInputActionsMap.end())
    {
        it->second.erase(action);
    }    
}

template<typename Action, typename Input>
void VSCBindings::eraseInputBindings(const Input& input)
{
    /*
     *  Have to cycle through all the input sets to remove the input. A bit expensive but
     *  not a time critical operation...    
     */
    
    mInputActionsMap.erase(input);
    
    BOOST_FOREACH (ActionInputsMap::value_type& pair, mActionInputsMap) 
    {
        pair->second.erase(input);
    }
}

template<typename Action, typename Input>
void VSCBindings::eraseActionBindings(const Action& action)
{
    mActionInputsMap.erase(action);
    
    BOOST_FOREACH (InputActionsMap::value_type& pair, mInputActionsMap) 
    {
        pair->second.erase(action);
    }
}

template<typename Action, typename Input>
void VSCBindings::setBinding(const Action& action, const Input& input) 
{
    /*
     *  Update input set for action
     */
    
    {
        ActionInputsMap::iterator it = mActionInputsMap.find(action);
        if (it == mActionInputsMap.end()) 
        {
            InputSet inputSet;
            mActionInputsMap[action] = inputSet;
            it = mActionInputsMap.find(action);
        }
        it->second.insert(input);
    }
    
    /*
     *  Update action set for input
     */
    
    {
        InputActionsMap::iterator it = mInputActionsMap.find(input);
        if (it == mInputActionsMap.end()) 
        {
            ActionSet actionSet;
            mInputActionsMap[input] = actionSet;
            it = mInputActionsMap.find(input);
        }
        it->second.insert(action);
    }
}



