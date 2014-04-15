//
//  encryption_key.cpp
//  ePub3
//
//  Created by Alexander Zarembovsky on 20.06.13.
//  Copyright (c) 2013 The Readium Foundation and contributors. All rights reserved.
//

#include "encryption_key.h"
#include "xpath_wrangler.h"

EPUB3_BEGIN_NAMESPACE

EncryptionKeyInfo::EncryptionKeyInfo(std::shared_ptr<ePub3::xml::Node> node)
{
    XPathWrangler xpath(node->Document(), {{"enc", XMLENCNamespaceURI}, {"dsig", XMLDSigNamespaceURI}});
    
    auto strings = xpath.Strings("./@Id", node);
    if (strings.empty())
    {
        fprintf(stderr, "Container::LoadEncryption() error: Node does not contain /@Id \n");
    }
    _location = strings[0];
    
    strings = xpath.Strings("./enc:EncryptionMethod/@Algorithm", node);
    if (strings.empty())
    {
        fprintf(stderr, "Container::LoadEncryption() error: Node does not contain /enc:EncryptionMethod/@Algorithm \n");
    }
    if (strings[0] != EncryptedKeyAlgorithmID)
    {
        fprintf(stderr, "Container::LoadEncryption() error: EncryptionMethod Algorithm %s does not match RSA (http://www.w3.org/2001/04/xmlenc#rsa-1_5) \n", strings[0].c_str());
    }
    _algorithm = strings[0];
    
    strings = xpath.Strings("./dsig:KeyInfo/dsig:KeyName", node);
    if (strings.empty())
    {
        fprintf(stderr, "Container::LoadEncryption() error: Node does not contain /dsig:KeyInfo/dsig:KeyName \n");
    }
    _name = strings[0];
    
    strings = xpath.Strings("./enc:CipherData/enc:CipherValue", node);
    if (strings.empty())
    {
        fprintf(stderr, "Container::LoadEncryption() error: Node does not contain /enc:CipherData/enc:CipherValue \n");
    }
    _cipher = strings[0];
    
}


EPUB3_END_NAMESPACE
