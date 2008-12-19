// $Id$
//
// Copyright (C) 2008 
// Fraunhofer Institute for Open Communication Systems (FOKUS)
// Competence Center NETwork research (NET), St. Augustin, GERMANY
//     Stefan Bund <g0dil@berlios.de>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the
// Free Software Foundation, Inc.,
// 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

/** \file
    \brief SyslogUDPTarget public header */

#ifndef HH_SENF_Utils_Logger_SyslogUDPTarget_
#define HH_SENF_Utils_Logger_SyslogUDPTarget_ 1

// Custom includes
#include "SyslogTarget.hh"
#include "../../Socket/Protocols/INet/INetAddressing.hh"
#include "../../Socket/ClientSocketHandle.hh"
#include "../../Socket/FramingPolicy.hh"
#include "../../Socket/ReadWritePolicy.hh"
#include "../../Socket/CommunicationPolicy.hh"

//#include "SyslogUDPTarget.mpp"
///////////////////////////////hh.p////////////////////////////////////////

namespace senf {
namespace log {

    /** \brief Log target writing UDP syslog packets

        The SyslogUDPTarget will send all log messages directly via UDP to a target host. This host
        should have a syslog daemon or relay running. The protocol is defined in <a
        href="ttp://tools.ietf.org/html/rfc3164">RFC-3164</a>.

        This log target has some important benefits:

        \li It will never block. It may however lose log messages.
        \li It does \e not add timestamp information locally.

        These are \e advantages since this makes SyslogUDPTarget a very reliable high-performance
        logging target.

        Valid facility values are from <tt>man 3 syslog</tt>:

        \par "" 
           <tt>LOG_AUTHPRIV</tt>, <tt>LOG_CRON</tt>, <tt>LOG_DAEMON</tt>, <tt>LOG_FTP</tt>,
           <tt>LOG_KERN</tt>, <tt>LOG_LOCAL0</tt>, <tt>LOG_LOCAL1</tt>, <tt>LOG_LOCAL2</tt>,
           <tt>LOG_LOCAL3</tt>, <tt>LOG_LOCAL4</tt>, <tt>LOG_LOCAL5</tt>, <tt>LOG_LOCAL6</tt>,
           <tt>LOG_LOCAL7</tt>, <tt>LOG_LPR</tt>, <tt>LOG_MAIL</tt>, <tt>LOG_NEWS</tt>,
           <tt>LOG_SYSLOG</tt>, <tt>LOG_USER</tt>, <tt>LOG_UUCP</tt>

        the default facility is <tt>LOG_USER</tt>

        The SENF log levels are mapped to syslog levels in the following way:

        <table class="senf fixedcolumn">
        <tr><td>senf::log::VERBOSE</td>   <td>\c LOG_DEBUG</td></tr>
        <tr><td>senf::log::NOTICE</td>    <td>\c LOG_INFO</td></tr>
        <tr><td>senf::log::MESSAGE</td>   <td>\c LOG_NOTICE</td></tr>
        <tr><td>senf::log::IMPORTANT</td> <td>\c LOG_WARNING</td></tr>
        <tr><td>senf::log::CRITICAL</td>  <td>\c LOG_CRIT</td></tr>
        <tr><td>senf::log::FATAL</td>     <td>\c LOG_EMERG</td></tr>
        </table>

        \note Since the UDP syslog packets are limited to 1024 characters and there must be some
            space left so a relay may optionally add a timestamp and hostname section, the log
            messages are split after 896 characters. Additionally the log messages are split at each
            newline char since non-printable characters are not allowed.

        \implementation The RFC only \e recommends the exact message format. This allows us to
            include the \c PRI part but skip the \c HEADER part (which includes the timestamp and
            hostname) for better performance. We add a space after the \c PRI to force the syslog
            daemon to skip the \c HEADER part.
     */
    class SyslogUDPTarget
        : public Target
    {
    public:
        ///////////////////////////////////////////////////////////////////////////
        // Types

        ///////////////////////////////////////////////////////////////////////////
        ///\name Structors and default members
        ///@{

        explicit SyslogUDPTarget(senf::INet4Address const & target, int facility = LOG_USER);
        explicit SyslogUDPTarget(senf::INet4SocketAddress const & target, int facility = LOG_USER);
        explicit SyslogUDPTarget(senf::INet6Address const & target, int facility = LOG_USER);
        explicit SyslogUDPTarget(senf::INet6SocketAddress const & target, int facility = LOG_USER);

        ///@}
        ///////////////////////////////////////////////////////////////////////////

        void showStream(bool flag = true);
        void showLevel(bool flag = true);
        void showArea(bool flag = true);

        void tag(std::string const & tag);

    private:
        void v_write(time_type timestamp, std::string const & stream, 
                     std::string const & area, unsigned level, 
                     std::string const & message);

        int facility_;
        std::string tag_;
        typedef senf::ClientSocketHandle< senf::MakeSocketPolicy<
            senf::DatagramFramingPolicy,
            senf::ConnectedCommunicationPolicy,
            senf::WriteablePolicy>::policy > Handle;
        Handle handle_;
        bool showStream_;
        bool showLevel_;
        bool showArea_;
    };

}}

///////////////////////////////hh.e////////////////////////////////////////
#include "SyslogUDPTarget.cci"
//#include "SyslogUDPTarget.ct"
//#include "SyslogUDPTarget.cti"
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