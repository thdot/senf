// $Id$
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
//   Stefan Bund <senf@g0dil.de>

/** \file
    \brief SocketSink unit tests */

//#include "SocketSink.test.hh"
//#include "SocketSink.test.ih"

// Custom includes
#include <senf/Scheduler/Scheduler.hh>
#include <senf/Socket/Protocols/INet/UDPSocketHandle.hh>
#include <senf/Socket/Protocols/INet/ConnectedUDPSocketHandle.hh>
#include "SocketSource.hh"
#include "DebugModules.hh"
#include "SocketSink.hh"
#include "Setup.hh"

#include <senf/Utils/auto_unit_test.hh>
#include <boost/test/test_tools.hpp>

#define prefix_
//-/////////////////////////////////////////////////////////////////////////////////////////////////
namespace ppi = senf::ppi;
namespace module = ppi::module;
namespace debug = module::debug;

namespace {
    void timeout() {
        senf::scheduler::terminate();
    }

    int base_pid = 0;

    unsigned port(unsigned i)
    {
        if (! base_pid)
            base_pid = ::getpid();
        return 23456u + (((base_pid^(base_pid>>8)^(base_pid>>16)^(base_pid>>24))&0xff)<<2) + i;
    }

    std::string localhost4str(unsigned i)
    {
        return (boost::format("localhost:%d") % port(i)).str();
    }

}

SENF_AUTO_UNIT_TEST(passiveSocketSink)
{
    senf::ConnectedUDPv4ClientSocketHandle outputSocket (
        senf::INet4SocketAddress(localhost4str(0)));
    module::PassiveSocketSink<> udpSink(outputSocket);
    debug::ActiveSource source;
    ppi::connect(source, udpSink);

    std::string data ("TEST");
    senf::Packet p (senf::DataPacket::create(data));

    senf::UDPv4ClientSocketHandle inputSocket;
    inputSocket.bind(senf::INet4SocketAddress(localhost4str(0)));
    senf::ppi::init();
    source.submit(p);

    std::string input (inputSocket.read());
    BOOST_CHECK_EQUAL( data, input );
}

SENF_AUTO_UNIT_TEST(activeSocketSink)
{
    senf::ConnectedUDPv4ClientSocketHandle outputSocket (
        senf::INet4SocketAddress(localhost4str(0)));
    module::ActiveSocketSink<> udpSink(outputSocket);
    debug::PassiveSource source;
    ppi::connect(source, udpSink);

    std::string data ("TEST");
    senf::Packet p (senf::DataPacket::create(data));

    senf::UDPv4ClientSocketHandle inputSocket;
    inputSocket.bind(senf::INet4SocketAddress(localhost4str(0)));
    senf::scheduler::TimerEvent timer (
        "activeSocketSink test timer", &timeout,
        senf::ClockService::now() + senf::ClockService::milliseconds(100));
    source.submit(p);
    senf::ppi::run();

    std::string input (inputSocket.read());
    BOOST_CHECK_EQUAL( data, input );
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
