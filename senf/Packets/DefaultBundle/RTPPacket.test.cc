// $Id$
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
//   Philipp Batroff <philipp.batroff@fokus.fraunhofer.de>

/** \file
    \brief RTPPacket unit tests */

#include "RTPPacket.hh"

// Custom includes

// Unit test includes
#include <senf/Utils/auto_unit_test.hh>

#define prefix_
//-/////////////////////////////////////////////////////////////////////////////////////////////////

SENF_AUTO_TEST_CASE(RTPPacket_packet)
{
    unsigned char data[] = {0x80 , 0x08 , 0x1b , 0xbb , 0x02 , 0xcb , 0xad , 0x80 , 0x50 , 0x48 , 0xa7, 0x8c };

    senf::RTPPacket p (senf::RTPPacket::create(data));

    BOOST_CHECK_EQUAL( p->version(), 2u);
    BOOST_CHECK_EQUAL( p->padding(), false);
    BOOST_CHECK_EQUAL( p->extension(), false);
    BOOST_CHECK_EQUAL( p->csrcCount(), 0u);
    BOOST_CHECK_EQUAL( p->marker(), false);
    BOOST_CHECK_EQUAL( p->payloadType(), 8u);
    BOOST_CHECK_EQUAL( p->seqNumber(), 0x1bbbu);
    BOOST_CHECK_EQUAL( p->timeStamp(), 0x2cbad80u);
    BOOST_CHECK_EQUAL( p->synSourceId(), 0x5048a78cu);

    std::ostringstream oss (std::ostringstream::out);
    SENF_CHECK_NO_THROW( p.dump( oss));
}

//-/////////////////////////////////////////////////////////////////////////////////////////////////
#undef prefix_
