// $Id: RawINetSocketHandle.cc 597 2008-01-15 09:16:20Z g0dil $
//
// Copyright (C) 2007
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
//   David Wagner <dw6@berlios.de>

#include "RawINetSocketHandle.hh"

// Custom includes
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

#include <senf/Utils/Exception.hh>

//#include "UDPSocketHandle.mpp"
#define prefix_
//-/////////////////////////////////////////////////////////////////////////////////////////////////

//-/////////////////////////////////////////////////////////////////////////////////////////////////
// senf::RawV4SocketProtocol

prefix_ void senf::RawV4SocketProtocol::init_client()
    const
{
    init_client(IPPROTO_RAW);
}

prefix_ void
senf::RawV4SocketProtocol::init_client(int const & protocol)
    const
{
    int sock = ::socket(PF_INET, SOCK_RAW, protocol);
    if (sock < 0)
        SENF_THROW_SYSTEM_EXCEPTION("could not open RawV4Socket");
    fd(sock);
}

prefix_ void
senf::RawV4SocketProtocol::init_client(int const & protocol, INet4SocketAddress const & address)
    const
{
    init_client(protocol);
    clientHandle().bind(address);
}

//-/////////////////////////////////////////////////////////////////////////////////////////////////
// senf::RawV6SocketProtocol::

prefix_ void senf::RawV6SocketProtocol::init_client()
    const
{
    init_client(IPPROTO_RAW);
}

prefix_ void senf::RawV6SocketProtocol::init_client(int const & protocol)
    const
{
    int sock = ::socket(PF_INET6,SOCK_RAW,protocol);
    if (sock < 0)
        SENF_THROW_SYSTEM_EXCEPTION("could not open RawV6Socket");
    fd(sock);
}

prefix_ void
senf::RawV6SocketProtocol::init_client(int const & protocol, INet6SocketAddress const & address)
    const
{
    init_client(protocol);
    clientHandle().bind(address);
}

//-/////////////////////////////////////////////////////////////////////////////////////////////////
#undef prefix_
//#include "RawINetSocketHandle.mpp"


// Local Variables:
// mode: c++
// fill-column: 100
// comment-column: 40
// c-file-style: "senf"
// indent-tabs-mode: nil
// ispell-local-dictionary: "american"
// compile-command: "scons -u test"
// End:
