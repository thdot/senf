// $Id$
//
// Copyright (C) 2011
// Fraunhofer Institute for Open Communication Systems (FOKUS)
//
// The contents of this file are subject to the Fraunhofer FOKUS Public License
// Version 1.0 (the "License"); you may not use this file except in compliance
// with the License. You may obtain a copy of the License at 
// http://senf.berlios.de/license.html
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
//   Thorsten Horstmann <tho@berlios.de>

/** \file
    \brief MIH Message-Registry non-inline non-template implementation */

#include "MIHMessageRegistry.hh"
//#include "MIHMessageRegistry.ih"

// Custom includes

#define prefix_
//-/////////////////////////////////////////////////////////////////////////////////////////////////
// senf::MIHMessageRegistry

#if BOOST_VERSION < 103400
#   define PTRMAP_GET_CONTENTS(v) (v)
#else
#   define PTRMAP_GET_CONTENTS(v) (*(v).second)
#endif

prefix_ void senf::MIHMessageRegistry::validate(key_t messageId, senf::Packet message)
{
    Map::const_iterator i (map_.find( messageId));
    if (i != map_.end())
        PTRMAP_GET_CONTENTS(*i).validate( message);
}

#undef PTRMAP_GET_CONTENTS

//-/////////////////////////////////////////////////////////////////////////////////////////////////
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