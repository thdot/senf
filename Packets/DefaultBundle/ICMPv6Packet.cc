// Copyright (C) 2008
// Fraunhofer Institute for Open Communication Systems (FOKUS)
// Competence Center NETwork research (NET), St. Augustin, GERMANY
//     Philipp Batroff <Philipp.Batroff@fokus.fraunhofer.de>
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

// Custom includes
#include "../../Packets/Packets.hh"
#include "ICMPv6Packet.hh"
#include <boost/io/ios_state.hpp>
#include "../../Packets/DefaultBundle/IPv6Packet.hh"
#include "../../Utils/IpChecksum.hh"

#define prefix_

namespace {
    senf::PacketRegistry<senf::IpTypes>::RegistrationProxy<senf::ICMPv6Packet>
        registerICMPv6Packet (58);
}

prefix_ void senf::ICMPV6PacketParser::calcChecksum() const {

    senf::IpChecksum summer;
    senf::IPv6Packet ipv6 (packet().rfind<senf::IPv6Packet>(senf::nothrow));
    
    summer.feed( ipv6->source().i(), 
                    ipv6->source().i() + senf::IPv6Packet::Parser::source_t::fixed_bytes );
    // The destination used here must be the *final* destination ...
    summer.feed( ipv6->destination().i(), ipv6->destination().i() + senf::IPv6PacketParser::destination_t::fixed_bytes );

    // This is a simplification. The value is really 32bit to support UDP Jumbograms
    // (RFC2147). However, skipping an even number of 0 bytes does not change the checksum
    summer.feed( i() + ipv6->length(), i() + ipv6->length() + 2 );
    // --> http://www.iana.org/assignments/protocol-numbers 
    // need to insert the correct protocol number here, NOT static 17!!
    summer.feed( 0u );
    summer.feed( 58u );
    // since header are 16 / even 32bit aligned we don't have to care for padding. since IpChecksum 
    // cares for padding at the final summing we don't have to care is the payload is 16nbit-aligned, too.
    summer.feed( i(), i()+checksum_offset );
    summer.feed( i()+checksum_offset+2, data().end() );

    boost::uint16_t rv (summer.sum());
    this->checksum() << (rv ? rv : 0xffffu);
}

prefix_ void senf::ICMPV6PacketType::dump(packet p, std::ostream &os)
{
    boost::io::ios_all_saver ias(os);
    os << "ICMPv6 protocol:\n"
        << "Type                    : " << p->type() <<"\n"
        << "Code                    : " << p->code() <<"\n"
        << "Checksum                : " << p->checksumOutput() << "\n";
}

#undef prefix_