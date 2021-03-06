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
//   Jens Moedeker <jens.moedeker@fokus.fraunhofer.de>

/** \file
    \brief TimerEventProxy.test non-inline non-template implementation */

#include "TimerEventProxy.hh"

// Custom includes
#include <boost/random.hpp>
#include "Scheduler.hh"

// Unit test includes
#include <senf/Utils/auto_unit_test.hh>

#define prefix_
//-/////////////////////////////////////////////////////////////////////////////////////////////////
using namespace senf;

namespace {

    int mask = 0;

    void handler(ClockService::clock_type time, int const &id)
    {
        mask = mask + id;
    }

    void run(ClockService::clock_type t) {
        scheduler::TimerEvent timeout(
                "test-timeout", &scheduler::terminate, scheduler::now() + t);
        scheduler::process();
    }

}

SENF_AUTO_TEST_CASE(timerEventProxy)
{
//    // abort on watchdog timeout
//    scheduler::watchdogAbort( true);
//    scheduler::watchdogTimeout(5000);

    ClockService::clock_type now (ClockService::now());
    {
        scheduler::TimerEventProxy<int> timers ("unit-test");

//        timers.add( t + ClockService::milliseconds(10000), 0 , &handler);
        timers.add( now + ClockService::milliseconds(800), 4, &handler);
        timers.add( now + ClockService::milliseconds(200), 1, &handler);
        BOOST_CHECK( timers.remove( 4));
        BOOST_CHECK(! timers.remove( 4));
        BOOST_CHECK_EQUAL( timers.timeout(4), ClockService::clock_type(0));
        timers.add( now + ClockService::milliseconds(700), 2, &handler);

        BOOST_CHECK_EQUAL( timers.timeout(1), now + ClockService::milliseconds(200));
        BOOST_CHECK_EQUAL( timers.timeout(2), now + ClockService::milliseconds(700));

        timers.add( now + ClockService::milliseconds(800), 2, &handler);
        BOOST_CHECK_EQUAL( timers.timeout(2), now + ClockService::milliseconds(800));
        timers.add( now, 4, &handler);
        BOOST_CHECK_EQUAL( timers.numEvents(), 3);

        run( ClockService::milliseconds( 2000));

        BOOST_CHECK_EQUAL( mask, 7);
        BOOST_CHECK_EQUAL( timers.numEvents(), 0);

        timers.add( ClockService::now() + ClockService::milliseconds(800), 1, &handler);
        timers.clear();
        run( ClockService::milliseconds( 2000));
        BOOST_CHECK_EQUAL( mask, 7);
        BOOST_CHECK_EQUAL( timers.numEvents(), 0);
    }
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
