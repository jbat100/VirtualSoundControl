
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
    
    typedef std::map<std::string, std::string>      InfoMap;
    typedef std::pair<std::string, std::string>     InfoKeyValuePair;
    
    VSCBaseException();
    VSCBaseException(std::string additionalInfo);
    VSCBaseException(std::string additionalInfo, std::string recoveryInfo);
    
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


#pragma mark Bad State

/*------------------------------------------------------------------------------------
 *	Used when call is made in unexpected state
 */
class VSCBadStateException : public VSCBaseException
{
    
public:
    
    VSCBadStateException(std::string additionalInfo) : VSCBaseException(additionalInfo) {}
    
    virtual const char* what() const throw()
    {
        return "Bad State";
    }
};

#pragma mark Internal Inconsistency Exception

/*------------------------------------------------------------------------------------
 *	Used when unexpected internal states arise
 */
class VSCInternalInconsistencyException : public VSCBaseException
{
    
public:
    
    VSCInternalInconsistencyException(std::string additionalInfo) : VSCBaseException(additionalInfo) {}
    
    virtual const char* what() const throw()
    {
        return "Internal Inconsistency";
    }
};

#pragma mark Argument Exception

/*------------------------------------------------------------------------------------
 *	Generic parameter checking
 */

class VSCInvalidArgumentException : public VSCBaseException
{
    
public:
    
    VSCInvalidArgumentException(std::string additionalInfo) : VSCBaseException(additionalInfo) {}
    
    virtual const char* what() const throw()
    {
        return "Invalid Argument";
    }
};

typedef VSCInvalidArgumentException VSCSInvalidArgumentException;

#pragma mark Out Of Bounds Exception

class VSCSOutOfBoundsException : public VSCBaseException
{
    virtual const char* what() const throw()
    {
        return "Out Of Bounds";
    }
};

#pragma mark Property/Parameter Exception

class VSCBadParameterException : public VSCBaseException
{
    virtual const char* what() const throw()
    {
        return "Bad Parameter";
    }
};

class VSCBadPropertyException : public VSCBaseException
{
    virtual const char* what() const throw()
    {
        return "Bad Property";
    }
};

typedef VSCBadParameterException VSCSBadParameterException;
typedef VSCBadPropertyException VSCSBadPropertyException;

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


#pragma mark MIDI Specific Exceptions

class VSCMIDIException : public VSCBaseException
{
    
public:
    
    VSCMIDIException(std::string additionalInfo) : VSCBaseException(additionalInfo) {}
    
    virtual const char* what() const throw()
    {
        return "Operation requires non-empty VSC::Envelope";
    }
};

#pragma mark Envelope Specific Exceptions

/*------------------------------------------------------------------------------------
 *	Envelope specific 
 */

class VSC::EnvelopeEmptyException : public VSCBaseException
{
    virtual const char* what() const throw()
    {
        return "Operation requires non-empty VSC::Envelope";
    }
};

#pragma mark UI Specific Exceptions

class VSCUIException : public VSCBaseException
{
    
public:
    
    VSCUIException(std::string additionalInfo) : VSCBaseException(additionalInfo) {}
    
    virtual const char* what() const throw()
    {
        return "UI exception";
    }
};


#endif
