// $Id$
//
// Copyright (C) 2013
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
    \brief QueueSocketSourceSink non-inline non-template implementation */

#include "QueueSocketSourceSink.hh"
#include "QueueSocketSourceSink.ih"

// Custom includes

//#include "QueueSocketSourceSink.mpp"
#define prefix_
//-/////////////////////////////////////////////////////////////////////////////////////////////////

prefix_ senf::ppi::module::QueueEthVLanFilter::QueueEthVLanFilter()
{
    route(input, output).autoThrottling(false);
    input.onRequest(&QueueEthVLanFilter::request);
    input.throttlingDisc( senf::ppi::ThrottlingDiscipline::NONE);
}

prefix_ void senf::ppi::module::QueueEthVLanFilter::request()
{
    EthernetPacket eth (input());
    QueueBufferAnnotation const & buffer (eth.annotation<QueueBufferAnnotation>());
    boost::optional<unsigned> vlanId (buffer->vlan());
    if (vlanId) {
        EthVLanPacket vlan (EthVLanPacket::createInsertBefore(eth.next()));
        vlan->vlanId() << *vlanId;
        vlan->type_length() << eth->type_length();
        eth.finalizeTo(vlan);
        vlan.reparse();
    }
    output(eth);
}

//-/////////////////////////////////////////////////////////////////////////////////////////////////
#undef prefix_
//#include "QueueSocketSourceSink.mpp"


// Local Variables:
// mode: c++
// fill-column: 100
// comment-column: 40
// c-file-style: "senf"
// indent-tabs-mode: nil
// ispell-local-dictionary: "american"
// compile-command: "scons -u test"
// End:
