

#include "VSCException.h"

const std::string VSCBaseExceptionAdditionalInfoKey = "VSCBaseExceptionAdditionalInfoKey";
const std::string VSCBaseExceptionRecoveryInfoKey = "VSCBaseExceptionRecoveryInfoKey";

VSCBaseException::VSCBaseException() {
    
}

VSCBaseException::VSCBaseException(std::string additionalInfo) {
    _userInfo.insert(InfoKeyValuePair(VSCBaseExceptionAdditionalInfoKey, additionalInfo));
}

VSCBaseException::VSCBaseException(std::string additionalInfo, std::string recoveryInfo) {
    _userInfo.insert(InfoKeyValuePair(VSCBaseExceptionAdditionalInfoKey, additionalInfo));
    _userInfo.insert(InfoKeyValuePair(VSCBaseExceptionRecoveryInfoKey, recoveryInfo));
}

void VSCBaseException::setValueForKey(std::string value, std::string key)
{
    _userInfo[key] = value;
}

std::string VSCBaseException::getValueForKey(std::string key)
{
    return _userInfo[key];
}