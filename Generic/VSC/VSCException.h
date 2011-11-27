//
//  VSCException.h
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 8/30/11.
//  Copyright 2011 NXP. All rights reserved.
//

#ifndef _VSC_EXCEPTION_H_
#define _VSC_EXCEPTION_H_

#include <exception>

class VSCBaseException : public std::exception
{
    virtual const char* what() const throw()
    {
        return "Base VSC Exception";
    }
};

class VSCEnveloppeEmptyException : public VSCBaseException
{
    virtual const char* what() const throw()
    {
        return "Operation requires non-empty VSCEnveloppe";
    }
};

class VSCSBadParameterException : public VSCBaseException
{
    virtual const char* what() const throw()
    {
        return "Bad Parameter";
    }
};

class VSCSBadPropertyException : public VSCBaseException
{
    virtual const char* what() const throw()
    {
        return "Bad Property";
    }
};


#endif
