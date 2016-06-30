// $Id:$
//
// Copyright (C) 2009
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
//       Christian Niephaus <cni@berlios.de>

/** \file
    \brief TAPEthernetInterface non-inline non-template implementation */

#include <linux/sockios.h>
#include <linux/filter.h>
#include "TAPEthernetInterface.hh"

// Custom includes

#define prefix_
//-/////////////////////////////////////////////////////////////////////////////////////////////////

//-/////////////////////////////////////////////////////////////////////////////////////////////////
// senf::emu::detail::TAPEthernetInterfaceNet

prefix_ senf::emu::detail::TAPEthernetInterfaceNet::TAPEthernetInterfaceNet()
    : socket (senf::noinit), source (socket), sink (socket),
      netOutput (annotator_.output), netInput (sink.input)
{
    senf::ppi::connect(source.output, annotator_.input);
}

prefix_ void senf::emu::detail::TAPEthernetInterfaceNet::assignSockets(TapSocketHandle & socket_)
{
    socket = socket_;
    source.handle(socket);
    sink.handle(socket);
}


//-/////////////////////////////////////////////////////////////////////////////////////////////////
// senf::emu::TAPEthernetInterface

namespace {
    struct DisableInterfaceGuard
    {
        DisableInterfaceGuard(senf::emu::Interface & iface)
            : iface_ (iface), enabled_ (iface_.enabled())
            { if (enabled_) iface_.disable(); }

        ~DisableInterfaceGuard()
            { if (enabled_) iface_.enable(); }

        senf::emu::Interface & iface_;
        bool enabled_;
    };
}

prefix_ senf::emu::TAPEthernetInterface::TAPEthernetInterface(std::string const & device)
    : BidirectionalWiredInterface (netOutput, netInput), dev_ (device), ctrl_ (dev_)
{
    BidirectionalWiredInterface::init();
    HardwareInterface::init();

    namespace fty = console::factory;

    consoleDir()
        .add("device",
             fty::Command(SENF_MEMBINDFNP(std::string, TAPEthernetInterface, device, () const))
             .overloadDoc("Get Ethernet network device name.") );
    consoleDir()
        .add("maxBurst", fty::Command(
                 SENF_MEMBINDFNP(void, TAPEthernetInterface, maxBurst, (unsigned)))
             .doc("set max burst rate"));
    consoleDir()
        .add("maxBurst", fty::Command(
                 SENF_MEMBINDFNP(unsigned, TAPEthernetInterface, maxBurst, () const))
             .doc("get max burst rate"));


    console::provideDirectory(interfaceDir(),"by-device").add(dev_, fty::Link(consoleDir()));

    annotator_.id(id());

    if (enabled())
        init_sockets();
}

prefix_ std::string senf::emu::TAPEthernetInterface::device()
    const
{
    return dev_;
}

prefix_ void senf::emu::TAPEthernetInterface::init_sockets()
{
    TapSocketHandle socket_ (device());
    socket_.blocking(false);

    TAPEthernetInterfaceNet::assignSockets(socket_);
}

prefix_ void senf::emu::TAPEthernetInterface::close_sockets()
{
    if (TAPEthernetInterfaceNet::socket.valid())
        TAPEthernetInterfaceNet::socket.close();
    
    TAPEthernetInterfaceNet::assignSockets(socket);
}

prefix_ void senf::emu::TAPEthernetInterface::v_enable()
{
    if (! enabled()) {
        ctrl_.up();
        init_sockets();
    }
}

prefix_ void senf::emu::TAPEthernetInterface::v_disable()
{
    if (enabled()) {
        close_sockets();
        ctrl_.down();
    }
}

prefix_ bool senf::emu::TAPEthernetInterface::v_enabled()
    const
{
    return ctrl_.isUp();
}

prefix_ void senf::emu::TAPEthernetInterface::v_id(MACAddress const & mac)
{
    DisableInterfaceGuard guard (*this);
    ctrl_.hardwareAddress(mac);
    annotator_.id(id());
}

prefix_ senf::MACAddress senf::emu::TAPEthernetInterface::v_id()
    const
{
    return ctrl_.hardwareAddress();
}

prefix_ bool senf::emu::TAPEthernetInterface::v_promisc()
    const
{
    return ctrl_.promisc();
}

prefix_ void senf::emu::TAPEthernetInterface::v_promisc(bool v)
{
    close_sockets();
    ctrl_.promisc(v);
    init_sockets();
    // inform the annotator about our promisc state (if promisc is on, all frames will be prepended with an AnnotationsPacket)
    annotator_.rawMode(v);
}

prefix_ unsigned senf::emu::TAPEthernetInterface::v_mtu()
    const
{
    return ctrl_.mtu();
}

prefix_ void senf::emu::TAPEthernetInterface::v_mtu(unsigned v)
{
    ctrl_.mtu(v);
}

prefix_ void senf::emu::TAPEthernetInterface::v_flushRxQueues()
{
    TAPEthernetInterfaceNet::source.flush();
}

prefix_ unsigned senf::emu::TAPEthernetInterface::maxBurst()
    const
{
    return source.maxBurst();
}

prefix_ void senf::emu::TAPEthernetInterface::maxBurst(unsigned maxBurst)
{
    source.maxBurst(maxBurst);
}

prefix_ void senf::emu::TAPEthernetInterface::dumpMmapStats(std::ostream & os)
{
    if (TAPEthernetInterfaceNet::socket.valid()) {
        os << "Socket active, but no stats collection implemented (yet)" << std::endl;
    } else {
        os << "Socket closed. Not stats available." << std::endl;
    }
}

#ifdef SENF_DEBUG

prefix_ unsigned senf::emu::TAPEthernetInterface::burstMax()
{
    return source.burstMax();
}

prefix_ unsigned senf::emu::TAPEthernetInterface::sharedPackets()
{
    return source.sharedPackets();
}

#endif

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
