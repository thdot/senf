// $Id$
//
// Copyright (C) 2006
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
    \brief ConnectedUDPv4SocketHandle and ConnectedUDPv6SocketHandle non-inline non-template implementation
 */

#include "ConnectedUDPSocketHandle.hh"
//#include "ConnectedUDPSocketHandle.ih"

// Custom includes
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

#include <senf/Utils/Exception.hh>

//#include "ConnectedUDPSocketHandle.mpp"
#define prefix_
//-/////////////////////////////////////////////////////////////////////////////////////////////////

//-/////////////////////////////////////////////////////////////////////////////////////////////////
// senf::ConnectedUDPv4SocketProtocol

prefix_ void senf::ConnectedUDPv4SocketProtocol::init_client()
    const
{
    int sock = ::socket(PF_INET,SOCK_DGRAM,0);
    if (sock < 0)
        SENF_THROW_SYSTEM_EXCEPTION("::socket(PF_INET,SOCK_DGRAM,0) failed.");
    fd(sock);
}

prefix_ void
senf::ConnectedUDPv4SocketProtocol::init_client(INet4SocketAddress const & address)
    const
{
    init_client();
    clientHandle().connect(address);
}

//-/////////////////////////////////////////////////////////////////////////////////////////////////
// senf::ConnectedUDPv6SocketProtocol::

prefix_ void senf::ConnectedUDPv6SocketProtocol::init_client()
    const
{
    int sock = ::socket(PF_INET6,SOCK_DGRAM,0);
    if (sock < 0)
        SENF_THROW_SYSTEM_EXCEPTION("::socket(PF_INET6,SOCK_DGRAM,0) failed.");
    fd(sock);
}

prefix_ void
senf::ConnectedUDPv6SocketProtocol::init_client(INet6SocketAddress const & address)
    const
{
    init_client();
    clientHandle().connect(address);
}

//-/////////////////////////////////////////////////////////////////////////////////////////////////
#undef prefix_
//#include "ConnectedUDPSocketHandle.mpp"


// Local Variables:
// mode: c++
// fill-column: 100
// c-file-style: "senf"
// indent-tabs-mode: nil
// ispell-local-dictionary: "american"
// compile-command: "scons -u test"
// comment-column: 40
// End:
