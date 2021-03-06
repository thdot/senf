// $Id$
//
// Copyright (C) 2008
// Fraunhofer Institute for Open Communication Systems (FOKUS)
//
// The contents of this file are subject to the Fraunhofer FOKUS Public License
// Version 1.0 (the "License"); you may not use this file except in compliance
// with the License. You may obtain a copy of the License at 
// http://senf.fokus.fraunhofer.de.de/license.html
//
// The Fraunhofer FOKUS Public License Version 1.0 is based on, 
// but modifies the Mozilla Public License Version 1.1.
// See the full license text for the amendments.
//
// Software distributed under the License is distributed on an "AS IS" basis, 
// WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License 
// for the specific language governing rights and limitations under the License.
//
// The Original Code is Fraunhofer FOKUS code.
//
// The Initial Developer of the Original Code is Fraunhofer-Gesellschaft e.V. 
// (registered association), Hansastraße 27 c, 80686 Munich, Germany.
// All Rights Reserved.
//
// Contributor(s):
//   Stefan Bund <senf@g0dil.de>

/** \file
    \brief Traits non-inline non-template implementation */

#include "Traits.hh"

// Custom includes
#include <senf/Utils/String.hh>

#define prefix_
//-/////////////////////////////////////////////////////////////////////////////////////////////////

prefix_ long senf::console::detail::parseEnum(EnumTable const & table,
                                              ParseCommandInfo::TokensRange const & tokens)
{
    if (tokens.size() != 1)
        throw SyntaxErrorException("parameter syntax error");

    std::string sym (tokens.begin()[0].value());
    boost::algorithm::to_lower(sym);
    EnumTable::left_map::const_iterator i1 (table.left.lower_bound(sym));
    EnumTable::left_map::const_iterator i2 (table.left.lower_bound(sym+"\xff"));
    if (i1 == i2)
        throw SyntaxErrorException("parameter syntax error: invalid enum value: ")
            << tokens.begin()[0].value();
    long v (i1->second);
    if (boost::algorithm::to_lower_copy(i1->first) == sym)
        return v;
    ++i1;
    if (i1 != i2)
        throw SyntaxErrorException("parameter syntax error: ambiguous enum value: ")
            << tokens.begin()[0].value();
    return v;
}

prefix_ std::string senf::console::detail::formatEnum(EnumTable const & table, long value)
{
    EnumTable::right_map::const_iterator i (table.right.find(value));
    if (i == table.right.end())
        return "invalid enum value: " + senf::str(value);
    return i->second;
}

//-/////////////////////////////////////////////////////////////////////////////////////////////////
#undef prefix_


// Local Variables:
// mode: c++
// fill-column: 100
// comment-column: 40
// c-file-style: "senf"
// indent-tabs-mode: nil
// ispell-local-dictionary: "american"
// compile-command: "scons -u test"
// End:
