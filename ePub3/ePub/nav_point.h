//
//  nav_point.h
//  ePub3
//
//  Created by Jim Dovey on 2012-12-11.
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

#ifndef __ePub3__nav_point__
#define __ePub3__nav_point__

#include <ePub3/epub3.h>
#include <ePub3/nav_element.h>
#include <ePub3/utilities/utfstring.h>
#include <vector>

EPUB3_BEGIN_NAMESPACE

/**
 @ingroup navigation
 */
class NavigationPoint : public NavigationElement
{
public:
                            NavigationPoint() {};
                            NavigationPoint(const std::string& ident, const std::string& label, const std::string& content) : NavigationElement(), _label(label), _sourceHref(content) {}
                            NavigationPoint(const NavigationPoint&)     = delete;
                            NavigationPoint(NavigationPoint&& o) : NavigationElement(o), _label(std::move(o._label)), _sourceHref(std::move(o._sourceHref)) {}
    virtual                 ~NavigationPoint() {}
    
    virtual const string&   Title()                     const   { return _label; }
    virtual void            SetTitle(const string& str)         { _label = str; }
    virtual void            SetTitle(string&& str)              { _label = str; }
    
    virtual const string&   SourceHref()                const   { return _sourceHref; }
    virtual void            SetSourceHref(const string& str)    { _sourceHref = str; }
    virtual void            SetSourceHref(string&& str)         { _sourceHref = str; }
    
protected:
    string _label;
    string _sourceHref;
};

EPUB3_END_NAMESPACE

#endif /* defined(__ePub3__nav_point__) */
