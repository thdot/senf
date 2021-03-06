// $Id$
//
// Copyright (C) 2013
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
//   Thorsten Horstmann <thorsten.horstmann@fokus.fraunhofer.de>

#ifndef HH_SENF_Utils_Cpp11Support_smart_ptr_
#define HH_SENF_Utils_Cpp11Support_smart_ptr_ 1

// Custom includes
#include <memory>
#include <senf/autoconf.hh>

//-/////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef HAVE_STD__UNIQUE_PTR_VOID_
#  define SENF_SMART_PTR  std::unique_ptr
#  define SENF_MOVE(t)    std::move(t)
#else
#  define SENF_SMART_PTR  std::unique_ptr
#  define SENF_MOVE(t)    t
#endif

#ifdef SENF_CXX11_ENABLED

#include <type_traits>

namespace senf {

    /* stephantl_make_unique.h
       Based on code by Stephan T. Lavavej at
       http://channel9.msdn.com/Series/C9-Lectures-Stephan-T-Lavavej-Core-C-/STLCCSeries6
     */

    namespace detail {

        template<typename T, typename ... Args>
        std::unique_ptr<T> make_unique_helper(std::false_type, Args&&... args) {
            return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
        }

        template<typename T, typename ... Args>
        std::unique_ptr<T> make_unique_helper(std::true_type, Args&&... args) {
            static_assert(std::extent<T>::value == 0,
                    "make_unique<T[N]>() is forbidden, please use make_unique<T[]>(),");
            typedef typename std::remove_extent<T>::type U;
            return std::unique_ptr<T>(new U[sizeof...(Args)]{std::forward<Args>(args)...});
        }
    }

    template<typename T, typename ... Args>
    std::unique_ptr<T> make_unique(Args&&... args) {
        return detail::make_unique_helper<T>(
            std::is_array<T>(),std::forward<Args>(args)... );
    }
}
#endif

//-/////////////////////////////////////////////////////////////////////////////////////////////////
#endif
