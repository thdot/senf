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
//   Thorsten Horstmann <thorsten.horstmann@fokus.fraunhofer.de>

/** \file
    \brief MIH Message-Registry public header */

#ifndef HH_SENF_Packets_80221Bundle_MIHMessageRegistry_
#define HH_SENF_Packets_80221Bundle_MIHMessageRegistry_ 1

// Custom includes
#include <boost/ptr_container/ptr_map.hpp>
#include <senf/Utils/singleton.hh>
#include <senf/Packets/Packets.hh>

#include "MIHMessageRegistry.ih"
//-/////////////////////////////////////////////////////////////////////////////////////////////////
namespace senf {

    class MIHMessageRegistry
        : public senf::singleton<MIHMessageRegistry>
    {
    public:
        typedef boost::uint16_t key_t;

    private:
        MIHMessageRegistry() {};

        typedef boost::ptr_map<key_t, detail::MIHMessageRegistry_EntryBase > Map;
        Map map_;

    public:
        using senf::singleton<MIHMessageRegistry>::instance;
        friend class senf::singleton<MIHMessageRegistry>;

        template <typename MIHPacket>
        struct RegistrationProxy {
            RegistrationProxy(key_t messageId);
        };

        template <typename MIHPacket>
        void registerMessageType(key_t messageId);

        void validate(key_t messageId, senf::Packet message);

        typedef Map::const_iterator const_iterator;
        const_iterator begin() const;
        const_iterator end() const;
    };


#   define SENF_MIH_PACKET_REGISTRY_REGISTER( packet, messageId )           \
        SENF_PACKET_REGISTRY_REGISTER(                                      \
            senf::MIHMessageRegistry, messageId, packet )                   \
        namespace {                                                         \
            senf::MIHMessageRegistry::RegistrationProxy< packet >           \
            BOOST_PP_CAT(mihPacketRegistration_, __LINE__) (messageId);     \
        }

}
//-/////////////////////////////////////////////////////////////////////////////////////////////////
//#include "MIHMessageRegistry.cci"
#include "MIHMessageRegistry.ct"
//#include "MIHMessageRegistry.cti"
#endif



// Local Variables:
// mode: c++
// fill-column: 100
// c-file-style: "senf"
// indent-tabs-mode: nil
// ispell-local-dictionary: "american"
// compile-command: "scons -u test"
// comment-column: 40
// End:
