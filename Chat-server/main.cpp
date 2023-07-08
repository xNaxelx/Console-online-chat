#include <iostream>
#include <boost/asio.hpp>
#include <string>
#include <cstdlib>

using namespace boost::asio::ip;

int main()
{
    setlocale(LC_ALL, "");

    boost::system::error_code ec;
    std::cout << ec.message() << std::endl;
    
    boost::asio::io_context io;

    udp::endpoint ep(udp::v4(), 7777);
    udp::socket sock(io, ep);
    //sock.bind(ep, ec);
    
    while (true)
    {
        char data[1024];
        udp::endpoint sender_endpoint;
        size_t length = sock.receive_from(boost::asio::buffer(data, 1024), sender_endpoint);
        sock.send_to(boost::asio::buffer(data, length), sender_endpoint);
        std::cout << "response sended" << std::endl;
    }
    

    std::cout << ec.message() << std::endl << ec.default_error_condition();

    io.run();
    return 0;
}