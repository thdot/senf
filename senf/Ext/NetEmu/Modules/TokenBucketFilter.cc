// $Id:$
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
//       Thorsten Horstmann <thorsten.horstmann@fokus.fraunhofer.de>

/** \file
    \brief TokenBucketFilter non-inline template implementation  */

#include "TokenBucketFilter.hh"

// Custom includes
#include <senf/Utils/membind.hh>
#include <senf/Utils/Console/ParsedCommand.hh>

#define prefix_
///////////////////////////////cc.p////////////////////////////////////////

prefix_ senf::emu::TokenBucketFilter::TokenBucketFilter(unsigned _burst, unsigned _rate, ppi::QueueingAlgorithm::ptr qAlgorithm)
    : queueAlgo_(qAlgorithm.release()),
      lastToken_( scheduler::now()),
      timer_( "TokenBucketFilter::timer", membind(&TokenBucketFilter::onTimeout, this)),
      bucketLimit_( _burst), bucketSize_( _burst),
      rate_( _rate)
{
    route( input, output).autoThrottling( false);
    input.onRequest( &TokenBucketFilter::onRequest);
    input.throttlingDisc( ppi::ThrottlingDiscipline::NONE);

    namespace fty = console::factory;
    dir.add("queue", queueAlgo_->consoleDir());
    dir.add( "burst", fty::Command(
            SENF_MEMBINDFNP( void, TokenBucketFilter, burst, (unsigned)))
        .doc( "set the bucket size in bytes"));
    dir.add( "burst", fty::Command(
            SENF_MEMBINDFNP( unsigned, TokenBucketFilter, burst, () const))
        .doc( "get the bucket size in bytes"));
    dir.add( "rateLimit", fty::Command(
            SENF_MEMBINDFNP( void, TokenBucketFilter, rate, (unsigned)))
        .doc( "set the rate limit in bits per second"));
    dir.add( "rateLimit", fty::Command(
            SENF_MEMBINDFNP( unsigned, TokenBucketFilter, rate, () const))
        .doc( "get the rate limit in bits per second"));
}

prefix_ unsigned senf::emu::TokenBucketFilter::rate()
    const
{
    return rate_;
}

prefix_ void senf::emu::TokenBucketFilter::rate(unsigned bits_per_second)
{
    rate_ = bits_per_second;
    if (rate_ == 0u) {
        // we have to flush the queue now
        queueAlgo_->clear();
    }
}

prefix_ void senf::emu::TokenBucketFilter::burst(unsigned bytes)
{
    bucketLimit_ = bytes;
}

prefix_ unsigned senf::emu::TokenBucketFilter::burst()
    const
{
    return bucketLimit_;
}

prefix_ void senf::emu::TokenBucketFilter::onTimeout()
{
    SENF_ASSERT( !queueAlgo_->empty(), "internal TokenBucketFilter error");
    fillBucket();
    while (!queueAlgo_->empty() && queueAlgo_->frontPacketSize() <= bucketSize_) {
        Packet packet (queueAlgo_->dequeue());
        bucketSize_ -= packet.size();
        output(packet);
    }
    if (! queueAlgo_->empty())
        setTimeout();
}

prefix_ void senf::emu::TokenBucketFilter::setTimeout()
{
    SENF_ASSERT( !queueAlgo_->empty(), "internal TokenBucketFilter error");
    ClockService::clock_type now (scheduler::now());
    Packet::size_type packetSize (queueAlgo_->frontPacketSize());
    SENF_ASSERT( packetSize > bucketSize_, "internal TokenBucketFilter error");
    timer_.timeout( now + ClockService::microseconds((packetSize-bucketSize_)*8000000ul / rate_));
}

prefix_ void senf::emu::TokenBucketFilter::fillBucket()
{
    ClockService::clock_type now (scheduler::now());
    ClockService::int64_type delta (ClockService::in_microseconds(now - lastToken_));
    if (delta == 0)
        return;

    lastToken_ = now;
    if (bucketSize_ == bucketLimit_)
        return;

    bucketSize_ += (delta * rate_) / 8000000;
    if (bucketSize_ > bucketLimit_)
        bucketSize_ = bucketLimit_;
}

prefix_ void senf::emu::TokenBucketFilter::onRequest()
{
    Packet const & packet (input.read());
    if (rate_ == 0) {
        // forward the packet immediatly
        output.write(packet);
        return;
    }
    Packet::size_type packetSize (packet.size());
    if (! queueAlgo_->empty()) {
        queueAlgo_->enqueue( packet);
        return;
    }
    fillBucket();
    if (packetSize <= bucketSize_) {
        bucketSize_ -= packetSize;
        output.write( packet);
        return;
    }

    if (queueAlgo_->enqueue( packet))
        setTimeout();
}

prefix_ senf::ppi::QueueingAlgorithm & senf::emu::TokenBucketFilter::qAlgorithm()
    const
{
    return *queueAlgo_;
}

prefix_ void senf::emu::TokenBucketFilter::qAlgorithm(ppi::QueueingAlgorithm::ptr qAlgorithm)
{
    queueAlgo_.reset(qAlgorithm.release());
    dir.add("queue", queueAlgo_->consoleDir());
}

///////////////////////////////cc.e////////////////////////////////////////
#undef prefix_