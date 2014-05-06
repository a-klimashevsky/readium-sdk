//
//  encryption_key.h
//  ePub3
//
//  Created by Alexander Zarembovsky on 20.06.13.
//  Copyright (c) 2013 The Readium Foundation and contributors. All rights reserved.
//

#ifndef __ePub3__encryption_key__
#define __ePub3__encryption_key__

#include <ePub3/epub3.h>

EPUB3_BEGIN_NAMESPACE

class EncryptionKeyInfo
{
protected:
    
    ///
    /// We should use http://www.w3.org/2001/04/xmlenc#rsa-1_5 only
    constexpr static const char * const  EncryptedKeyAlgorithmID = "http://www.w3.org/2001/04/xmlenc#rsa-1_5";

public:
    ///
    /// Encryption algorithms are URIs compared as strings.
    typedef string                  key_algorithm_type;
    typedef string                  key_location;
    
public:
    ///
    /// Creates a new EncryptionKeyInfo with no details filled in.
    EncryptionKeyInfo()                            = default;
    /**
     Creates a new EncryptionInfo from an EncryptionData XML element node.
     @param node An XML node, which *must* be an `EncryptionData` node as defined in
     XML-ENC.
     @see http://www.w3.org/TR/xmlenc-core1/#sec-EncryptedData
     */
    EncryptionKeyInfo(xmlNodePtr node);
    ///
    /// Copy constructor.
    EncryptionKeyInfo(const EncryptionKeyInfo& o) : _algorithm(o._algorithm), _location(o._location), _name(o._name), _cipher(o._cipher) {}
    ///
    /// Move constructor.
    EncryptionKeyInfo(EncryptionKeyInfo&& o) : _algorithm(std::move(o._algorithm)), _location(std::move(o._location)), _name(std::move(o._name)), _cipher(std::move(o._cipher)) {}
    virtual     ~EncryptionKeyInfo() {}
    
    ///
    /// Returns an algorithm URI as defined in XML-ENC or OCF.
    /// @see http://www.w3.org/TR/xmlenc-core1/#sec-Table-of-Algorithms
    /// @see http://www.idpf.org/epub/30/spec/epub30-ocf.html#fobfus-specifying
    virtual const key_algorithm_type&   Algorithm()                          const   { return _algorithm; }
    ///
    /// Assigns an algorithm identifier.
    /// @see Algorithm()const
    virtual void                    SetAlgorithm(const key_algorithm_type& alg)      { _algorithm = alg; }
    virtual void                    SetAlgorithm(key_algorithm_type&& alg)           { _algorithm = alg; }
    
    ///
    /// Returns key location (ID)
    virtual const key_location&     Location()                               const   { return _location; }
    ///
    /// Assigns a location identifier.
    /// @see Location()const
    virtual void                    SetLocation(const key_location& loc)             { _location = loc; }
    virtual void                    SetLocation(key_location&& loc)                  { _location = loc; }
    
    /// Returns key name
    virtual const string&           Name()                                   const   { return _name; }
    ///
    /// Assigns a name identifier.
    /// @see Name()const
    virtual void                    SetName(const string& n)                         { _name = n; }
    virtual void                    SetName(string&& n)                              { _name = n; }
    
    ///
    /// Returns the key value
    virtual const string&           Cipher()                                 const   { return _cipher; }
    ///
    /// Assigns a cipher identifier.
    /// @see Cipher()const
    virtual void                    SetCipher(const string& c)                       { _cipher = c; }
    virtual void                    SetCipher(string&& c)                            { _cipher = c; }
    
protected:
    key_algorithm_type  _algorithm; ///< The algorithm identifier, as per XML-ENC or OCF.
    key_location        _location;  ///< EncryptedKey Location (Id)
    string              _name;      ///< Key name
    string              _cipher;    ///< Key value
    
};

EPUB3_END_NAMESPACE

#endif /* defined(__ePub3__encryption_key__) */
