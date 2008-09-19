// $Id$
//
// Copyright (C) 2007
// Fraunhofer Institute for Open Communication Systems (FOKUS)
// Competence Center NETwork research (NET), St. Augustin, GERMANY
//     Thorsten Horstmann <tho@berlios.de>
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

// Definition of non-inline non-template functions

//#include "server.hh"
//#include "server.ih"

// Custom includes
#include <string>
#include <iostream>
#include <senf/Scheduler/Scheduler.hh>
#include <senf/Utils/membind.hh>
#include <senf/Socket/Protocols/INet.hh>

class Server
{
    senf::TCPv4ServerSocketHandle serverSock;
    senf::scheduler::FdEvent acceptevent;
    senf::scheduler::FdEvent readevent;

public:
    Server(senf::INet4Address const & host, unsigned int port)
        : serverSock(senf::INet4SocketAddress(host, port)),
          acceptevent("Server accept", senf::membind(&Server::accept, this),
                      serverSock, senf::scheduler::FdEvent::EV_READ),
          readevent("Server read", 0)
        {}

    void run()
    {
        senf::scheduler::process();
    }

private:
    void accept(int event)
    {
        senf::TCPv4ClientSocketHandle clientSock (serverSock.accept());
        readevent
            .action(boost::bind(&Server::readFromClient, this, clientSock, _1))
            .handle(clientSock)
            .events(senf::scheduler::FdEvent::EV_READ)
            .enable();
    }

    void readFromClient(senf::TCPv4ClientSocketHandle clientSock, int event)
    {
        if (!clientSock) {
            readevent.disable();
            return;
        }
        std::string data (clientSock.read());
        std::cout << "'" << data << "'" << std::endl;
    }
};


int main(int argc, char const * argv[])
{
    try {
        Server myServer (senf::INet4Address::Loopback, 4243);
        myServer.run();
    }
    catch (std::exception const & ex) {
        std::cerr << senf::prettyName(typeid(ex)) << ": " << ex.what() << "\n";
    }
    return 0;
}


// Local Variables:
// mode: c++
// fill-column: 100
// c-file-style: "senf"
// indent-tabs-mode: nil
// ispell-local-dictionary: "american"
// compile-command: "scons -u"
// comment-column: 40
// End:
