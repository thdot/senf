// $Id$
//
// Copyright (C) 2007
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
//   Anton Gillert <atx@berlios.de>


#ifndef HH_SENF_Socket_Protocols_DVB_DVBProtocolWrapper_
#define HH_SENF_Socket_Protocols_DVB_DVBProtocolWrapper_ 1

// Custom includes
#include <senf/Utils/Console/ScopedDirectory.hh>
#include <senf/Utils/Console/Traits.hh>
#include "DVBDemuxHandles.hh"

//-/////////////////////////////////////////////////////////////////////////////////////////////////

namespace senf {

    /** \brief Helperclass for configuration and controlling DVB protocol handles.

     */
    class DVBSectionProtocolWrapper : public DVBProtocolWrapper
    {
    private:
        const DVBDemuxSectionSocketProtocol& protocol;

    public:
        enum Flags {CHECK_CRC = DMX_CHECK_CRC,
            ONESHOT = DMX_ONESHOT,
            IMMEDIATE_START = DMX_IMMEDIATE_START,
            KERNEL_CLIENT = DMX_KERNEL_CLIENT};
        senf::console::ScopedDirectory<> dir;

        DVBSectionProtocolWrapper(senf::DVBDemuxSectionHandle sh);
        ~DVBSectionProtocolWrapper() { }

        void setSectionFilter(unsigned short int pid, u_int8_t filter,
                unsigned int flags, u_int8_t mask, u_int8_t mode, unsigned int timeout);

        void setBufferSize(unsigned long size);
                                ///< Set the size of the circular buffer used for filtered data.
                                /**< The default size is two maximum sized sections, i.e. if
                                     this function is not called a buffer size of 2 * 4096 bytes
                                     will be used.
                                     \param[in] size Size of circular buffer. */
        void startFiltering();  ///< Starts filtering
        void stopFiltering();   ///< Stops filtering
    };


    class DVBPESProtocolWrapper : public senf::DVBProtocolWrapper
    {
    private:
        const senf::DVBDemuxPESSocketProtocol& protocol;

    public:
        enum Flags {CHECK_CRC = DMX_CHECK_CRC,
            ONESHOT = DMX_ONESHOT,
            IMMEDIATE_START = DMX_IMMEDIATE_START,
            KERNEL_CLIENT = DMX_KERNEL_CLIENT};
        senf::console::ScopedDirectory<> dir;

        DVBPESProtocolWrapper(senf::DVBDemuxPESHandle sh);
        ~DVBPESProtocolWrapper() { };

        void setPESFilter(unsigned short int pid, dmx_input_t input,
                dmx_output_t output, dmx_pes_type_t pesType, unsigned int flags);
        void startFiltering(); ///< Starts filtering
        void stopFiltering();  ///< Stops filtering
    };

}

//-/////////////////////////////////////////////////////////////////////////////////////////////////
#include "DVBProtocolWrapper.cci"
//#include "DVBProtocolWrapper.ct"
//#include "DVBProtocolWrapper.cti"
#endif

