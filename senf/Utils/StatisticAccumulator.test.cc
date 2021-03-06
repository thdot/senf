// $Id$
//
// Copyright (C) 2010
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
//   Mathias Kretschmer <mathias.kretschmer@fokus.fraunhofer.de>
//   Jens Moedeker <jens.moedeker@fokus.fraunhofer.de>

#include "StatisticAccumulator.hh"

// Custom includes
#include <math.h>
#include "Statistics.hh"

// Unit test includes
#include <senf/Utils/auto_unit_test.hh>

#define prefix_
///////////////////////////////ct.p////////////////////////////////////////

SENF_AUTO_TEST_CASE(StatisticAccumulator)
{
    senf::Statistics s;

    senf::StatisticAccumulator<int> sa;

    sa.accumulate( 5);
    sa.accumulate( 3);
    sa.accumulate( 4);
    sa.accumulate( 6);
    sa.accumulate( 7);

    BOOST_CHECK_EQUAL( sa.max(), 7);
    BOOST_CHECK_EQUAL( sa.min(), 3);
    BOOST_CHECK_EQUAL( sa.avg(), 5.0);
    BOOST_CHECK_EQUAL( sa.last_avg(), 0.0);
//    BOOST_CHECK_EQUAL( sa.stddev(), 1.41421354);
    BOOST_CHECK_EQUAL( sa.count(), 5);

    s(0,sa);

    BOOST_CHECK_EQUAL( sa.max(), std::numeric_limits<int>::min());
    BOOST_CHECK_EQUAL( sa.min(), std::numeric_limits<int>::max());
//    BOOST_CHECK( (boost::math::isnan)( sa.avg()));
    BOOST_CHECK( ::isnan( sa.avg()) != 0);
    BOOST_CHECK_EQUAL( sa.last_avg(), 5.0);
    BOOST_CHECK( ::isnan( sa.stddev()));
    BOOST_CHECK_EQUAL( sa.count(), 0);
}

///////////////////////////////ct.e////////////////////////////////////////
#undef prefix_
