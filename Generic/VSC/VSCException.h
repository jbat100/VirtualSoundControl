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
#include <map>
#include <string>

#include <boost/exception/all.hpp>

#pragma mark Exception User Info Keys

extern const std::string VSCBaseExceptionAdditionalInfoKey;
extern const std::string VSCBaseExceptionRecoveryInfoKey;

#pragma mark Base Exception

/*------------------------------------------------------------------------------------
 *	Base Exception 
 */

class VSCBaseException : public boost::exception, public std::exception
{

    /*
     *  Interestig compile error : Exception specification of overriding function os more lax than base version
     *  http://stackoverflow.com/questions/8618060/c-exception-subclass-string-member-variable
     *
     *  The destructor of std::string is not no-throw, which causes the implicit destructor of FileException not no-throw either. 
     *  But the destructor of std::exception is no-throw, thus there's a compiler error.
     *  You could declare an explicit no-throw destructor:
     *
     *  virtual ~FileException() throw() {}
     *
     *  or just inherit from std::runtime_error instead of std::exception, which has a constructor that takes std::string input.
     *
     *  OR CONSIDER USING THE BOOST STUFF WHICH WILL NOT REQUIRE THIS COMPROMISE
     */
     
public:
    
    typedef std::map<std::string, std::string> InfoMap;
    
    virtual ~VSCBaseException() throw() {}
    
    virtual const char* what() const throw()
    {
        return "Base VSC Exception";
    }
    
    void setValueForKey(std::string value, std::string key);
    std::string getValueForKey(std::string key);
    
    const InfoMap& getInfoMap(void);
    
private:
    
    std::map<std::string, std::string> _userInfo;
 
};

#pragma mark Implementation Exception

/*------------------------------------------------------------------------------------
 *	Implementation
 */

class VSCNotImplementedException : public VSCBaseException
{
    virtual const char* what() const throw()
    {
        return "Functionality is not implemented";
    }
};

#pragma mark Internal Inconsistency Exception

/*------------------------------------------------------------------------------------
 *	Used when unexpected internal states arise
 */
class VSCInternalInconsistencyException : public VSCBaseException
{
    virtual const char* what() const throw()
    {
        return "Called Pure Virtual Function";
    }
};

#pragma mark Enveloppe Specific Exception

#pragma mark Enveloppe Specific Exception

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

#pragma mark Argument Exception

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

#pragma mark Out Of Bounds Exception

class VSCSOutOfBoundsException : public VSCBaseException
{
    virtual const char* what() const throw()
    {
        return "Out Of Bounds";
    }
};

#pragma mark Property/Parameter Exception

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

#pragma mark Virtual Function Exception (Used For Obj-C)

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
