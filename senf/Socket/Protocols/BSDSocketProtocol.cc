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
    \brief BSDSocketProtocol non-inline non-template implementation */

#include <senf/Utils/String.hh>
#include "BSDSocketProtocol.hh"
//#include "BSDSocketProtocol.ih"

// Custom includes
#include <linux/filter.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/ip.h>


//#include "BSDSocketProtocol.mpp"
#define prefix_
//-/////////////////////////////////////////////////////////////////////////////////////////////////

prefix_ std::pair<bool,unsigned> senf::BSDSocketProtocol::linger()
    const
{
    struct linger ling;
    socklen_t len = sizeof(ling);
    ::memset(&ling, 0, sizeof(ling));
    if (::getsockopt(fd(),SOL_SOCKET,SO_LINGER,&ling,&len) < 0)
        SENF_THROW_SYSTEM_EXCEPTION("could not get socketopt SO_LINGER");
    return std::make_pair(ling.l_onoff, ling.l_linger);
}

prefix_ void senf::BSDSocketProtocol::linger(bool enable, unsigned timeout)
    const
{
    struct linger ling;
    ling.l_onoff = enable;
    ling.l_linger = timeout;
    if (::setsockopt(fd(),SOL_SOCKET,SO_LINGER,&ling,sizeof(ling)) < 0)
        SENF_THROW_SYSTEM_EXCEPTION("could not set socketopt SO_LINGER");
}

prefix_ boost::uint8_t senf::BSDSocketProtocol::priority()
    const
{
    int value;
    socklen_t len (sizeof(value));
    if (::getsockopt(fd(),SOL_SOCKET,SO_PRIORITY,&value,&len) < 0)
        SENF_THROW_SYSTEM_EXCEPTION("could not get socketopt SO_PRIORITY");
    return value;
}

prefix_ void senf::BSDSocketProtocol::priority(boost::uint8_t value)
    const
{
    int ivalue (value);
    if (::setsockopt(fd(),SOL_SOCKET,SO_PRIORITY,&ivalue,sizeof(ivalue)) < 0)
        SENF_THROW_SYSTEM_EXCEPTION("could not set socketopt SO_PRIORITY to "  + senf::str(value));
}

prefix_ int senf::BSDSocketProtocol::error()
    const
{
    int err;
    socklen_t len (sizeof(err));
    if (::getsockopt(fd(),SOL_SOCKET,SO_ERROR,&err,&len) < 0)
        SENF_THROW_SYSTEM_EXCEPTION("could not get socketopt SO_ERROR");
    return err;
}

prefix_ unsigned senf::BSDSocketProtocol::rcvbuf()
    const
{
    unsigned size;
    socklen_t len (sizeof(size));
    if (::getsockopt(fd(),SOL_SOCKET,SO_RCVBUF,&size,&len) < 0)
        SENF_THROW_SYSTEM_EXCEPTION("could not get socketopt SO_RCVBUF");
    // Linux doubles the bufer size on setting the RCVBUF to cater for internal
    // headers. We fix this up here .. (see lkml FAQ)
    return size/2;
}

prefix_ void senf::BSDSocketProtocol::rcvbuf(unsigned size)
    const
{
    if (::setsockopt(fd(),SOL_SOCKET,SO_RCVBUF,&size,sizeof(size)) < 0)
        SENF_THROW_SYSTEM_EXCEPTION("could not set socketopt SO_RCVBUF to" + senf::str(size));
}

prefix_ unsigned senf::BSDSocketProtocol::sndbuf()
    const
{
    unsigned size;
    socklen_t len (sizeof(size));
    if (::getsockopt(fd(),SOL_SOCKET,SO_SNDBUF,&size,&len) < 0)
        SENF_THROW_SYSTEM_EXCEPTION("could not get socketopt SO_SNDBUF");
    // Linux doubles the bufer size on setting the SNDBUF to cater for internal
    // headers. We fix this up here .. (see lkml FAQ)
    return size/2;
}

prefix_ void senf::BSDSocketProtocol::sndbuf(unsigned size)
    const
{
    if (::setsockopt(fd(),SOL_SOCKET,SO_SNDBUF,&size,sizeof(size)) < 0)
        SENF_THROW_SYSTEM_EXCEPTION("could not set socketopt SO_SNDBUF to " + senf::str(size));
}

prefix_ void senf::BSDSocketProtocol::detachSocketFilter()
{
    int dummy;
    if (::setsockopt(fd(), SOL_SOCKET, SO_DETACH_FILTER, &dummy, sizeof(dummy)) < 0)
        SENF_THROW_SYSTEM_EXCEPTION("could not detach socket filter");
}

prefix_ void senf::BSDSocketProtocol::do_attachSocketFilter(::sock_filter * filter, unsigned short len)
{
    struct sock_fprog sockArg;
    sockArg.filter = filter;
    sockArg.len = len;
    if (::setsockopt(fd(), SOL_SOCKET, SO_ATTACH_FILTER, &sockArg, sizeof(sockArg)) < 0)
        SENF_THROW_SYSTEM_EXCEPTION("could not attach socket filter");
}

prefix_ void senf::BSDSocketProtocol::mtuDiscovery(int mode) 
    const
{
    if (::setsockopt(fd(), IPPROTO_IP, IP_MTU_DISCOVER, &mode, sizeof(mode)) < 0)
        SENF_THROW_SYSTEM_EXCEPTION("could not set IP_MTU_DISCOVER mode to " + senf::str(mode));
}

prefix_ void senf::BSDSocketProtocol::sndLowat(unsigned lowat)
    const
{
    if (::setsockopt(fd(), SOL_SOCKET, SO_SNDLOWAT, &lowat, sizeof(lowat)) < 0)
        SENF_THROW_SYSTEM_EXCEPTION("could not set SO_SNDLOWAT to " + senf::str(lowat));
}

prefix_ void senf::BSDSocketProtocol::rcvLowat(unsigned lowat)
    const
{
    if (::setsockopt(fd(), SOL_SOCKET, SO_RCVLOWAT, &lowat, sizeof(lowat)) < 0)
        SENF_THROW_SYSTEM_EXCEPTION("could not set SO_RCVLOWAT to " + senf::str(lowat));
}

//-/////////////////////////////////////////////////////////////////////////////////////////////////

prefix_ bool senf::AddressableBSDSocketProtocol::reuseaddr()
    const
{
    int value;
    socklen_t len (sizeof(value));
    if (::getsockopt(fd(),SOL_SOCKET,SO_REUSEADDR,&value,&len) < 0)
        SENF_THROW_SYSTEM_EXCEPTION("could not get socketopt SO_REUSEADDR");
    return value;
}

prefix_ void senf::AddressableBSDSocketProtocol::reuseaddr(bool value)
    const
{
    int ivalue (value);
    if (::setsockopt(fd(),SOL_SOCKET,SO_REUSEADDR,&ivalue,sizeof(ivalue)) < 0)
        SENF_THROW_SYSTEM_EXCEPTION("could not set socketopt SO_REUSEADDR");
}

//-/////////////////////////////////////////////////////////////////////////////////////////////////
#undef prefix_
//#include "BSDSocketProtocol.mpp"


// Local Variables:
// mode: c++
// fill-column: 100
// c-file-style: "senf"
// indent-tabs-mode: nil
// ispell-local-dictionary: "american"
// compile-command: "scons -u test"
// comment-column: 40
// End:
