#include <iostream>
#include <boost/asio.hpp>
#include <string>
#include <thread>
#include "Server.h"

using namespace boost::asio::ip;
using namespace boost::asio;


int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "");

    thread_pool pool(std::thread::hardware_concurrency());
    
    boost::system::error_code ec;    
    boost::asio::io_context io;

    unsigned short *port, portTemp = 7777;
    port = &portTemp;
    if (argc == 2)
    {
        *port = (unsigned short)(argv[1]);
    }

    Server server(io, pool, *port);
    

    while (true)
    {
        server.do_receive();

        io.run();
    }
    

    return 0;
}