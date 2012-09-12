
#include "VSCOgreKeyboardManager.h"
#include "VSCUserInput.h"
#include "VSCKeyBindings.h"
#include "VSCOgreKeyboardAction.h"

#include <boost/assert.hpp>

VSCOgreKeyboardManager::VSCOgreKeyboardManager()
{
    loadDefaultBindings();
}

VSCOgreKeyboardManager::~VSCOgreKeyboardManager()
{
    
}

VSCOgreKeyboardAction::Key VSCOgreKeyboardManager::getActionKeyForCombination(const VSCKeyboard::Combination& comb) const 
{
    VSCKeyboard::Action action = mKeyBindings.getActionForCombination(comb);
    if (action == VSCKeyboard::NullAction) return VSCOgreKeyboardAction::None;
    BOOST_ASSERT_MSG(action.domain == VSCKeyboard::ActionDomainOgre, "Expected action.domain to be VSCKeyboard::ActionDomainOgre");
    return (VSCOgreKeyboardAction::Key)action.key;
}

VSCKeyboard::Combination VSCOgreKeyboardManager::getCombinationForActionKey(const VSCOgreKeyboardAction::Key& key) const 
{
    VSCKeyboard::Action action(key, VSCKeyboard::ActionDomainOgre);
    VSCKeyboard::Combination comb = mKeyBindings.getCombinationForAction(action);
    return comb;
}

void VSCOgreKeyboardManager::setBinding(const VSCOgreKeyboardAction::Key& key, const VSCKeyboard::Combination& comb)
{
    VSCKeyboard::Action action(key, VSCKeyboard::ActionDomainOgre);
    mKeyBindings.setBinding(action, comb);
}

void VSCOgreKeyboardManager::loadDefaultBindings(void)
{
    using namespace OIS;
    
    setBinding(VSCOgreKeyboardAction::Quit,                         VSCKeyboard::Combination(KC_ESCAPE));
    setBinding(VSCOgreKeyboardAction::SaveScreenShot,               VSCKeyboard::Combination(KC_SYSRQ));
    
    setBinding(VSCOgreKeyboardAction::ToggleDisplayWireFrame,       VSCKeyboard::Combination(KC_T));
    setBinding(VSCOgreKeyboardAction::ToggleDisplayAABB,            VSCKeyboard::Combination(KC_1));
    setBinding(VSCOgreKeyboardAction::ToggleDisplayContactPoints,   VSCKeyboard::Combination(KC_3));
    setBinding(VSCOgreKeyboardAction::ToggleDeactivation,           VSCKeyboard::Combination(KC_4));
    setBinding(VSCOgreKeyboardAction::ToggleHelpText,               VSCKeyboard::Combination(KC_5));
    setBinding(VSCOgreKeyboardAction::ToggleDrawText,               VSCKeyboard::Combination(KC_6));
    setBinding(VSCOgreKeyboardAction::ToggleProfileTimings,         VSCKeyboard::Combination(KC_7));
    setBinding(VSCOgreKeyboardAction::ToggleSatComparison,          VSCKeyboard::Combination(KC_8));
    setBinding(VSCOgreKeyboardAction::ToggleBulletLCP,              VSCKeyboard::Combination(KC_9));
    setBinding(VSCOgreKeyboardAction::ToggleCCD,                    VSCKeyboard::Combination(KC_0));
    
    setBinding(VSCOgreKeyboardAction::ToggleSimulationPause,        VSCKeyboard::Combination(KC_P));
    setBinding(VSCOgreKeyboardAction::SimulationStep,               VSCKeyboard::Combination(KC_M));
    
    setBinding(VSCOgreKeyboardAction::IncrementShootSpeed,          VSCKeyboard::Combination(KC_ADD));
    setBinding(VSCOgreKeyboardAction::DecrementShootSpeed,          VSCKeyboard::Combination(KC_MINUS));
    
    setBinding(VSCOgreKeyboardAction::ShootCube,                    VSCKeyboard::Combination(KC_B));
    setBinding(VSCOgreKeyboardAction::ShootSphere,                  VSCKeyboard::Combination(KC_N));
    setBinding(VSCOgreKeyboardAction::ShootCylinder,                VSCKeyboard::Combination(KC_H));
    setBinding(VSCOgreKeyboardAction::ShootCone,                    VSCKeyboard::Combination(KC_G));
    setBinding(VSCOgreKeyboardAction::DropCube,                     VSCKeyboard::Combination(KC_J));
    setBinding(VSCOgreKeyboardAction::DropSphere,                   VSCKeyboard::Combination(KC_K));
    setBinding(VSCOgreKeyboardAction::DropCylinder,                 VSCKeyboard::Combination(KC_U));
    setBinding(VSCOgreKeyboardAction::DropCone,                     VSCKeyboard::Combination(KC_I));
    
    setBinding(VSCOgreKeyboardAction::MoveCameraForward,            VSCKeyboard::Combination(KC_W));
    setBinding(VSCOgreKeyboardAction::MoveCameraBackward,           VSCKeyboard::Combination(KC_S));
    setBinding(VSCOgreKeyboardAction::MoveCameraRight,              VSCKeyboard::Combination(KC_A));
    setBinding(VSCOgreKeyboardAction::MoveCameraLeft,               VSCKeyboard::Combination(KC_D));
    
    
}


