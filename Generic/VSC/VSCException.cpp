//
//  VSCException.cpp
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 8/30/11.
//  Copyright 2011 NXP. All rights reserved.
//

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