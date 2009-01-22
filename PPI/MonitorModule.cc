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
    \brief MonitorModule non-inline non-template implementation */

#include "MonitorModule.hh"
//#include "MonitorModule.ih"

// Custom includes

//#include "MonitorModule.mpp"
#define prefix_
///////////////////////////////cc.p////////////////////////////////////////

prefix_ void senf::ppi::module::MonitorModule::request()
{
    Packet p (input());
    v_handlePacket(p);
    if (output.connected())
        output(p);
}

prefix_ void senf::ppi::module::MonitorModule::throttle()
{
    if (output.connected())
        input.throttle();
}

prefix_ void senf::ppi::module::MonitorModule::unthrottle()
{
    if (output.connected())
        input.unthrottle();
}

///////////////////////////////cc.e////////////////////////////////////////
#undef prefix_
//#include "MonitorModule.mpp"


// Local Variables:
// mode: c++
// fill-column: 100
// comment-column: 40
// c-file-style: "senf"
// indent-tabs-mode: nil
// ispell-local-dictionary: "american"
// compile-command: "scons -u test"
// End:
