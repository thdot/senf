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
//   Stefan Bund <senf@g0dil.de>

/** \file
    \brief IgnoreValue public header */

#ifndef HH_SENF_Utils_IgnoreValue_
#define HH_SENF_Utils_IgnoreValue_ 1

// Custom includes

//#include "IgnoreValue.mpp"
//-/////////////////////////////////////////////////////////////////////////////////////////////////

namespace senf {

    /** \brief Explicitly ignore a value

        To explicitly ignore a value (e.g. the return value of a function), pass that value to
        senf::IGNORE(). This has the added benefit of silencing \em any warnings about ignored
        values by g++.
     */
    template <class T>
    void IGNORE(T const &);
    template <class T>
    void IGNORE(T const *);

}

//-/////////////////////////////////////////////////////////////////////////////////////////////////
//#include "IgnoreValue.cci"
//#include "IgnoreValue.ct"
#include "IgnoreValue.cti"
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
