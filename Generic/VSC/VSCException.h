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


/*------------------------------------------------------------------------------------
 *	Base Exception 
 */

class VSCBaseException : public std::exception
{
    virtual const char* what() const throw()
    {
        return "Base VSC Exception";
    }
};


/*------------------------------------------------------------------------------------
 *	Envelope specific 
 */

class VSCEnveloppeEmptyException : public VSCBaseException
{
    virtual const char* what() const throw()
    {
        return "Operation requires non-empty VSCEnveloppe";
    }
};

/*------------------------------------------------------------------------------------
 *	Generic parameter checking
 */

class VSCSInvalidArgumentException : public VSCBaseException
{
    virtual const char* what() const throw()
    {
        return "Invalid Argument";
    }
};

class VSCSOutOfBoundsException : public VSCBaseException
{
    virtual const char* what() const throw()
    {
        return "Out Of Bounds";
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

/*------------------------------------------------------------------------------------
 *	Used for objective c where pure virtual cannot actually be enforeced (like in C++)
 */
class VSCSCalledPureVirtualFunctionException : public VSCBaseException
{
    virtual const char* what() const throw()
    {
        return "Called Pure Virtual Function";
    }
};


#endif
