// $Id:$
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
//       Thorsten Horstmann <thorsten.horstmann@fokus.fraunhofer.de>

#include "IperfPacket.hh"

// Custom includes

#define prefix_

prefix_ void IperfUDPPacketType::dump(packet p, std::ostream & os)
{
    os << "Iperf UDP Packet\n"
       << "  id:      " << p->id() << std::endl
       << "  tv_sec:  " << p->tv_sec() << std::endl
       << "  tv_usec: " << p->tv_usec() << std::endl;
}

#undef prefix_


// Local Variables:
// mode: c++
// fill-column: 100
// c-file-style: "senf"
// indent-tabs-mode: nil
// ispell-local-dictionary: "american"
// compile-command: "scons -u test"
// comment-column: 40
// End: