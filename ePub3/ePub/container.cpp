//
//  container.cpp
//  ePub3
//
//  Created by Jim Dovey on 2012-11-28.
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

#include "container.h"
#include "archive.h"
#include "package.h"
#include "archive_xml.h"
#include "xpath_wrangler.h"
#include "byte_stream.h"

EPUB3_BEGIN_NAMESPACE

static const char * gContainerFilePath = "META-INF/container.xml";
static const char * gEncryptionFilePath = "META-INF/encryption.xml";
static const char * gRootfilesXPath = "/ocf:container/ocf:rootfiles/ocf:rootfile";
static const char * gRootfilePathsXPath = "/ocf:container/ocf:rootfiles/ocf:rootfile/@full-path";
static const char * gVersionXPath = "/ocf:container/@version";

Container::Container(const string& path) : _archive(Archive::Open(path.stl_str()))
{
    if ( _archive == nullptr )
        throw std::invalid_argument(_Str("Path does not point to a recognised archive file: '", path, "'"));
    
    // TODO: Initialize lazily? Doing so would make initialization faster, but require
    // PackageLocations() to become non-const, like Packages().
    ArchiveXmlReader reader(_archive->ReaderAtPath(gContainerFilePath));
    _ocf = reader.xmlReadDocument(gContainerFilePath, nullptr, XML_PARSE_RECOVER|XML_PARSE_NOENT|XML_PARSE_DTDATTR);
    if ( _ocf == nullptr )
        throw std::invalid_argument(_Str(__PRETTY_FUNCTION__, ": No container.xml in ", path));
    
    XPathWrangler xpath(_ocf, {{"ocf", "urn:oasis:names:tc:opendocument:xmlns:container"}});
    xmlNodeSetPtr nodes = xpath.Nodes(reinterpret_cast<const xmlChar*>(gRootfilesXPath));
    
    if ( nodes == nullptr || nodes->nodeNr == 0 )
        throw std::invalid_argument(_Str(__PRETTY_FUNCTION__, ": No rootfiles in ", path));
    
    for ( int i = 0; i < nodes->nodeNr; i++ )
    {
        xmlNodePtr n = nodes->nodeTab[i];
        
        const xmlChar * _type = xmlGetProp(n, reinterpret_cast<const xmlChar*>("media-type"));
        std::string type((_type == nullptr ? "" : reinterpret_cast<const char*>(_type)));
        
        const xmlChar * _path = xmlGetProp(n, reinterpret_cast<const xmlChar*>("full-path"));
        if ( _path == nullptr )
            continue;
        
        _packages.push_back(new Package(_archive, _path, type));
    }

    LoadEncryption();
}
Container::Container(Container&& o) : _archive(o._archive), _ocf(o._ocf), _packages(std::move(o._packages))
{
    o._archive = nullptr;
    o._ocf = nullptr;
    o._packages.clear();
}
Container::~Container()
{
    if ( _archive != nullptr )
        delete _archive;
    if ( _ocf != nullptr )
        xmlFreeDoc(_ocf);
}
Container::PathList Container::PackageLocations() const
{
    XPathWrangler xpath(_ocf, {{"ocf", "urn:oasis:names:tc:opendocument:xmlns:container"}});
    
    PathList output;
    for ( string& str : xpath.Strings(gRootfilePathsXPath) )
    {
        output.emplace_back(std::move(str));
    }
    
    return output;
}
const Package* Container::DefaultPackage() const
{
    if ( _packages.empty() )
        return nullptr;
    return _packages[0];
}
string Container::Version() const
{
    XPathWrangler xpath(_ocf, {{"ocf", "urn:oasis:names:tc:opendocument:xmlns:container"}});
    
    std::vector<string> strings = xpath.Strings(gVersionXPath);
    if ( strings.empty() )
        return "1.0";       // guess
    
    return std::move(strings[0]);
}
void Container::LoadEncryption()
{
    ArchiveReader *pZipReader = _archive->ReaderAtPath(gEncryptionFilePath);
    if ( pZipReader == nullptr )
        return;
    
    ArchiveXmlReader reader(pZipReader);
    xmlDocPtr enc = reader.xmlReadDocument(gEncryptionFilePath, nullptr, XML_PARSE_RECOVER|XML_PARSE_NOENT|XML_PARSE_DTDATTR);
    if ( enc == nullptr )
        return;
    
    XPathWrangler xpath(enc, {{"enc", XMLENCNamespaceURI}, {"ocf", OCFNamespaceURI}});
    
    xmlNodeSetPtr nodes = xpath.Nodes("/ocf:encryption/enc:EncryptedData");
    if ( nodes == nullptr || nodes->nodeNr == 0 )
    {
        fprintf(stderr, "Container::LoadEncryption() error: encryption.xml does not contain enc:EncryptedData nodes \n");
        return;     // should be a hard error?
    }
    
    for ( size_t i = 0; i < nodes->nodeNr; i++ )
    {
        _encryption.emplace_back(new EncryptionInfo(nodes->nodeTab[i]));
    }
    
    xmlNodeSetPtr keyNodes = xpath.Nodes("/ocf:encryption/enc:EncryptedKey");
    if (keyNodes == nullptr || keyNodes->nodeNr == 0 || keyNodes->nodeNr > 1)
    {
        fprintf(stderr, "Container::LoadEncryption() error: encryption.xml does not contain enc:EncryptedKey node \n");
        return; // should be a hard error?
    }
    
    _key_info = new EncryptionKeyInfo(keyNodes->nodeTab[0]);

    xmlXPathFreeNodeSet(nodes);
}

bool Container::IsContainerEncrypted() const
{
    return (_encryption.size() != 0);
}

const EncryptionInfo* Container::EncryptionInfoForPath(const string &path) const
{
    for ( auto item : _encryption )
    {
        if ( item->Path() == path)
        {
            if (item->Retrieval_Method() != _key_info->Location())
            {
                fprintf(stderr, "Container::LoadEncryption(): RetrievalMethod URI %s for %s does not exist \n", item->Retrieval_Method().c_str(), item->Path().c_str());
                return nullptr;
            }
            
            return item;
        }
        
    }
    
    return nullptr;
}

bool Container::IsPathEncrypted(const ePub3::string &path) const
{
    for ( auto item : _encryption )
    {
        if ( item->Path() == path)
        {
            if (item->Retrieval_Method() != _key_info->Location())
            {
                fprintf(stderr, "Container::LoadEncryption(): RetrievalMethod URI %s for %s does not exist \n", item->Retrieval_Method().c_str(), item->Path().c_str());
                return false;
            }
            
            return true;
        }
        
    }
    
    return false;
}

Auto<ByteStream> Container::ReadStreamAtPath(const string &path) const
{
    return _archive->ByteStreamAtPath(path.stl_str());
}

EPUB3_END_NAMESPACE
