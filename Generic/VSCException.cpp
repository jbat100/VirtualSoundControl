

#include "VSCException.h"

const std::string VSCExceptionAdditionalInfoKey     = "VSCExceptionAdditionalInfoKey";
const std::string VSCExceptionRecoveryInfoKey       = "VSCExceptionRecoveryInfoKey";

VSCBaseException::VSCBaseException() {
    
}

VSCBaseException::VSCBaseException(std::string additionalInfo) {
    _userInfo.insert(InfoKeyValuePair(VSCExceptionAdditionalInfoKey, additionalInfo));
}

VSCBaseException::VSCBaseException(std::string additionalInfo, std::string recoveryInfo) {
    _userInfo.insert(InfoKeyValuePair(VSCExceptionAdditionalInfoKey, additionalInfo));
    _userInfo.insert(InfoKeyValuePair(VSCExceptionRecoveryInfoKey, recoveryInfo));
}

void VSCBaseException::setValueForKey(std::string value, std::string key)
{
    _userInfo[key] = value;
}

std::string VSCBaseException::getValueForKey(std::string key)
{
    return _userInfo[key];
}