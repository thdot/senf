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
/** \file
    \brief DVBSocketController non-inline non-template implementation */

#include "DVBProtocolWrapper.hh"

// Custom includes
#include <senf/Utils/Console/ParsedCommand.hh>
#include <senf/Utils/Console/Utility.hh>

#define prefix_
//-/////////////////////////////////////////////////////////////////////////////////////////////////

SENF_CONSOLE_REGISTER_ENUM( dmx_input_t, (DMX_IN_FRONTEND)(DMX_IN_DVR) );
SENF_CONSOLE_REGISTER_ENUM( dmx_output_t, (DMX_OUT_DECODER)(DMX_OUT_TAP)(DMX_OUT_TS_TAP) );
SENF_CONSOLE_REGISTER_ENUM( dmx_pes_type_t, (DMX_PES_AUDIO0)(DMX_PES_VIDEO0)(DMX_PES_TELETEXT0)
        (DMX_PES_SUBTITLE0)(DMX_PES_PCR0)(DMX_PES_AUDIO1)(DMX_PES_VIDEO1)(DMX_PES_TELETEXT1)
        (DMX_PES_SUBTITLE1)(DMX_PES_PCR1)(DMX_PES_AUDIO2)(DMX_PES_VIDEO2)(DMX_PES_TELETEXT2)
        (DMX_PES_SUBTITLE2)(DMX_PES_PCR2)(DMX_PES_AUDIO3)(DMX_PES_VIDEO3)(DMX_PES_TELETEXT3)
        (DMX_PES_SUBTITLE3)(DMX_PES_PCR3)(DMX_PES_OTHER));

namespace senf {
    SENF_CONSOLE_REGISTER_ENUM_MEMBER(DVBSectionProtocolWrapper, Flags,
            (CHECK_CRC)(ONESHOT)(IMMEDIATE_START)(KERNEL_CLIENT));

    SENF_CONSOLE_REGISTER_ENUM_MEMBER(DVBPESProtocolWrapper, Flags,
            (CHECK_CRC)(ONESHOT)(IMMEDIATE_START)(KERNEL_CLIENT));
}

prefix_ senf::DVBSectionProtocolWrapper::DVBSectionProtocolWrapper(senf::DVBDemuxSectionHandle sh)
    : protocol(sh.protocol())
{
    namespace kw = console::kw;
    namespace fty = console::factory;

    dir.add("buffersize", fty::Command(&DVBSectionProtocolWrapper::setBufferSize, this)
            .doc("Set the size of the circular buffer used for filtered data.")
            .arg("size", "in byte") );

    dir.add("start", fty::Command(&DVBSectionProtocolWrapper::startFiltering, this)
            .doc("Starts filtering") );

    dir.add("stop", fty::Command(&DVBSectionProtocolWrapper::setBufferSize, this)
            .doc("Stops filtering") );

    dir.add("filter",
            fty::Command<void (unsigned short int,
                                    unsigned,
                                    senf::console::FlagCollection<Flags>,
                                    unsigned,
                                    unsigned,
                                    unsigned)
            >(&DVBSectionProtocolWrapper::setSectionFilter, this)
            .arg("pid", "pid to filter")
            .arg("filter", "filter",
                 kw::default_value = 62,
                 kw::default_doc = "0x3e")
            .arg("flags", "or-able: CHECK_CRC, ONESHOT, IMMEDIATE_START, KERNEL_CLIENT",
                 kw::default_value = DMX_IMMEDIATE_START | DMX_CHECK_CRC,
                 kw::default_doc = "(IMMEDIATE_START CHECK_CRC)")
            .arg("mask", "mask",
                 kw::default_value = 0xff,
                 kw::default_doc = "0xff")
            .arg("mode", "mode",
                 kw::default_value = 0,
                 kw::default_doc = "0x00")
            .arg("timeout", "timeout",
                 kw::default_value = 0,
                 kw::default_doc = "0x00")
            .doc("Sets parameters for section filter.") );

    dir.add("stop", fty::Command(&DVBSectionProtocolWrapper::setBufferSize, this)
            .doc("Stops filtering") );
}


prefix_ senf::DVBPESProtocolWrapper::DVBPESProtocolWrapper(senf::DVBDemuxPESHandle sh)
    : protocol(sh.protocol())
{
    namespace kw = console::kw;
    namespace fty = console::factory;
    dir.add("filter",
            fty::Command<void (unsigned short int,
                                    dmx_input_t,
                                    dmx_output_t,
                                    dmx_pes_type_t,
                                    senf::console::FlagCollection<Flags>)
            >(&DVBPESProtocolWrapper::setPESFilter, this)
            .arg("pid", "pid to filter")
            .arg("input", "input-filter: DMX_IN_FRONTEND DMX_IN_DVR ")
            .arg("output", "output-filter: DMX_OUT_DECODER DMX_OUT_TAP DMX_OUT_TS_TAP ")
            .arg("pesType", "PES type: DMX_PES_AUDIO[0-3] DMX_PES_VIDEO[0-3] "
                            "DMX_PES_TELETEXT[0-3], DMX_PES_SUBTITLE[0-3], DMX_PES_PCR[0-3], "
                            "DMX_PES_OTHER")
            .arg("flags", "or-able: CHECK_CRC, ONESHOT, IMMEDIATE_START, KERNEL_CLIENT",
                 kw::default_value = DMX_IMMEDIATE_START | DMX_CHECK_CRC,
                 kw::default_doc = "(IMMEDIATE_START CHECK_CRC)")
            .doc("Sets parameters for PES filter.") );

    dir.add("start", fty::Command(&DVBPESProtocolWrapper::startFiltering, this)
            .doc("Starts filtering") );

    dir.add("stop", fty::Command(&DVBPESProtocolWrapper::stopFiltering, this)
            .doc("Stops filtering") );
}

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
