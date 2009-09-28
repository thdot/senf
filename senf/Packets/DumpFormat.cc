// $Id$
//
// Copyright (C) 2009 
// Fraunhofer Institute for Open Communication Systems (FOKUS)
// Competence Center NETwork research (NET), St. Augustin, GERMANY
//     Stefan Bund <g0dil@berlios.de>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the
// Free Software Foundation, Inc.,
// 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

/** \file
    \brief DumpFormat non-inline non-template implementation */

#include "Packets.hh"
#include "DumpFormat.ih"

// Custom includes
#include <iostream>
#include <algorithm>

//#include "DumpFormat.mpp"
#define prefix_
///////////////////////////////cc.p////////////////////////////////////////

#ifndef SENF_PACKET_DUMP_COLON_COLUMN
#define SENF_PACKET_DUMP_COLON_COLUMN 27
#endif

prefix_ std::string senf::fieldName(std::string const & s)
{
    std::string t (std::max(std::string::size_type(SENF_PACKET_DUMP_COLON_COLUMN+1), s.size()+5), ' ');
    std::copy(s.begin(), s.end(), t.begin()+2);
    t[t.size()-2] = ':';
    return t;
}

prefix_ std::string senf::detail::prettySignedNumber(long long v, unsigned bits)
{
    if (v<0) return prettyUnsignedNumber(-v,bits,-1);
    else     return prettyUnsignedNumber(v,bits,+1);
}

prefix_ std::string senf::detail::prettyUnsignedNumber(unsigned long long v, unsigned bits,
                                                       int sign)
{
    int bytes ((bits+7)/8);
    int digs (int(2.4*bytes)+1);
    std::stringstream ss;
    ss << (sign ? (sign<0 ? "-" : " ") : "") 
       << "0x" << std::setw(2*bytes) << std::setfill('0') << std::hex 
       << 1u*v
       << " (" << std::setw(digs+(sign ? 1 : 0)) << std::setfill(' ') << std::dec;
    if (sign)
        ss << sign*static_cast<long long>(v);
    else
        ss << 1u*v;
    ss << ") (";
    for (int i (bytes-1); i>=0; --i) {
        char c ((v>>(8*i))&0xff);
        ss << ((c>=32 && c<=127) ? c : '.');
    }
    ss << ')';
    return ss.str();
}


///////////////////////////////cc.e////////////////////////////////////////
#undef prefix_
//#include "DumpFormat.mpp"


// Local Variables:
// mode: c++
// fill-column: 100
// comment-column: 40
// c-file-style: "senf"
// indent-tabs-mode: nil
// ispell-local-dictionary: "american"
// compile-command: "scons -u test"
// End:
