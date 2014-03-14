// $Id$
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
//   Stefan Bund <senf@g0dil.de>

/** \file
    \brief DiscardStream public header */

#ifndef HH_SENF_Utils_DiscardStream_
#define HH_SENF_Utils_DiscardStream_ 1

// Custom includes
#include <boost/iostreams/concepts.hpp>
#include <boost/iostreams/stream.hpp>

//#include "DiscardStream.mpp"
//-/////////////////////////////////////////////////////////////////////////////////////////////////

namespace senf {

    class DiscardSink
        : public boost::iostreams::sink
    {
    public:
        std::streamsize write(char const * s, std::streamsize n);
    };

    class DiscardStream
        : public boost::iostreams::stream<DiscardSink>
    {
    public:
        DiscardStream();
    };

}

//-/////////////////////////////////////////////////////////////////////////////////////////////////
#include "DiscardStream.cci"
//#include "DiscardStream.ct"
//#include "DiscardStream.cti"
#endif


// Local Variables:
// mode: c++
// fill-column: 100
// comment-column: 40
// c-file-style: "senf"
// indent-tabs-mode: nil
// ispell-local-dictionary: "american"
// compile-command: "scons -u test"
// End:
