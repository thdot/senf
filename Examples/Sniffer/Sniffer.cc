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


// Definition of non-inline non-template functions

//#include "Sniffer.hh"
//#include "Sniffer.ih"

// Custom includes
#include <string>
#include <iostream>
#include <iomanip>
#include <senf/Socket/Protocols/Raw.hh>
#include <senf/Utils/membind.hh>
#include <senf/Utils/hexdump.hh>
#include <senf/Packets/DefaultBundle/EthernetPacket.hh>
#include <senf/Scheduler/Scheduler.hh>

//#include "Sniffer.mpp"
#define prefix_
//-/////////////////////////////////////////////////////////////////////////////////////////////////

int loop_main (int argc, char const * argv[])
{
    try {
        senf::PacketSocketHandle sock;
        sock.bind(senf::LLSocketAddress(argv[2]));
        // sock.protocol().promisc("eth0",senf::PacketProtocol::Promiscuous);

        while (true) { // forever
            senf::EthernetPacket packet (senf::EthernetPacket::create(
                                             senf::noinit));
            sock.read(packet.data(),0);
            packet.dump(std::cout);
            senf::hexdump(
                    packet.last().data().begin(),
                    packet.last().data().end(),
                    std::cout);
            std::cout << "\n\n";
        }
    }
    catch (std::exception const & ex) {
        std::cerr << senf::prettyName(typeid(ex)) << ": " << ex.what() << "\n";
    }
    return 0;
}

class Sniffer
{
    senf::PacketSocketHandle sock;
    senf::scheduler::FdEvent event;

public:
    Sniffer(std::string const & interface)
        : event ("Sniffer", senf::membind(&Sniffer::dumpPacket, this),
                 sock, senf::scheduler::FdEvent::EV_READ)
    {
        sock.bind(senf::LLSocketAddress(interface));
    }

    void run()
    {
        senf::scheduler::process();
    }

private:
    void dumpPacket(int event)
    {
        senf::EthernetPacket packet (
            senf::EthernetPacket::create(senf::noinit));
        sock.read(packet.data(),0);
        packet.dump(std::cout);
        senf::hexdump(
                packet.last().data().begin(),
                packet.last().data().end(),
                std::cout);
        std::cout << "\n\n";
    }
};

int scheduler_main(int argc, char const * argv[])
{
    try {
        Sniffer sniffer (argv[2]);
        sniffer.run();
    }
    catch (std::exception const & ex) {
        std::cerr << senf::prettyName(typeid(ex)) << ": " << ex.what() << "\n";
    }
    return 0;
}

int main(int argc, char const * argv[])
{
    std::cout << "Registered packets:\n\n";
    senf::dumpPacketRegistries(std::cout);

    if (argc >= 3) {
        if (std::string(argv[1]) == "loop")
            return loop_main(argc,argv);
        else if (std::string(argv[1]) == "scheduler")
            return scheduler_main(argc,argv);
    }

    std::cerr << "Usage: sniffer { loop | scheduler } [interface]" << std::endl;
    return 1;
}

//-/////////////////////////////////////////////////////////////////////////////////////////////////
#undef prefix_
//#include "Sniffer.mpp"


// Local Variables:
// mode: c++
// fill-column: 100
// c-file-style: "senf"
// indent-tabs-mode: nil
// ispell-local-dictionary: "american"
// compile-command: "scons -u"
// comment-column: 40
// End:
