//
//  encryption.cpp
//  ePub3
//
//  Created by Jim Dovey on 2012-12-28.
//  Copyright (c) 2012-2013 The Readium Foundation and contributors.
//  
//  The Readium SDK is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include "encryption.h"
#include "xpath_wrangler.h"

EPUB3_BEGIN_NAMESPACE

EncryptionInfo::EncryptionInfo(xmlNodePtr node)
{
    XPathWrangler xpath(node->doc, {{"enc", XMLENCNamespaceURI}, {"dsig", XMLDSigNamespaceURI}});

    auto strings = xpath.Strings("./enc:EncryptionMethod/@Algorithm", node);
    if (strings.empty())
    {
        fprintf(stderr, "Container::LoadEncryption() error: Node does not contain /enc:EncryptionMethod/@Algorithm \n");
    }
    if (strings[0] != EncryptedDataAlgorithmID)
    {
        fprintf(stderr, "Container::LoadEncryption() error: EncryptionMethod Algorithm %s does not match AES-128 (http://www.w3.org/2001/04/xmlenc#kw-aes128) \n", strings[0].c_str());
    }
    _algorithm = strings[0];
    
    strings = xpath.Strings("./dsig:KeyInfo/dsig:RetrievalMethod/@URI", node);
    if (strings.empty())
    {
        fprintf(stderr, "Container::LoadEncryption() error: Node does not contain /dsig:KeyInfo/dsig:RetrievalMethod/@URI \n");
    }
    strings[0].erase(0, 1);
    _retrieval_method = strings[0];
    
    strings = xpath.Strings("./dsig:KeyInfo/dsig:KeyIV", node);
    if (strings.empty())
    {
        fprintf(stderr, "Container::LoadEncryption() error: Node does not contain ./dsig:KeyInfo/dsig:KeyIV \n");
    }
    _keyIV = strings[0];
    
    strings = xpath.Strings("./enc:CipherData/enc:CipherReference/@URI", node);
    if (strings.empty())
    {
        fprintf(stderr, "Container::LoadEncryption() error: Node does not contain /enc:CipherData/enc:CipherReference/@URI \n");
    }
    _path = strings[0];
    
}

EPUB3_END_NAMESPACE
