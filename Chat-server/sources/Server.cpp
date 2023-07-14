#include "Server.h"

Server::Server(boost::asio::io_context& io_context, thread_pool& pool, short port)
    : socket_(io_context, udp::endpoint(udp::v4(), (int)port)), pool(&pool)
{
}


void Server::do_receive()
{
    boost::system::error_code ec;
    size_t bytes_recvd = socket_.receive_from(boost::asio::buffer(data_, MAX_MESSAGE_LENGTH), temp_sender_endpoint_, 0, ec);
    
    if (!std::any_of(users.begin(), users.end(), [&](udp::endpoint y) { return temp_sender_endpoint_ == y; }))
    {
        users.push_back(temp_sender_endpoint_);
    }
    if (!ec && bytes_recvd > 0)
    {
        std::cout << "Data received" << std::endl;
        do_send(bytes_recvd, data_, temp_sender_endpoint_);
    }

}


void Server::do_send(std::size_t length, char* buff, udp::endpoint& sender)
{
    for (auto& i : users)
    {
        if (i == sender)
        {
            continue;
        }
        post(*pool, boost::bind(&Server::thread_send, this, i, boost::asio::buffer(buff, length)));
    }
    
}

void Server::thread_send(udp::endpoint point, mutable_buffer Lbuff)
{
    boost::system::error_code ec;
    socket_.send_to(Lbuff, point, 0, ec);
    if (ec.value() != 0)
    {
        users.remove(point);
        std::cout << point.address() << ":" << point.port() << "doesn't respond" << std::endl;
    }
    std::cout << "Data sended to: " << point.address() << ":" <<point.port() << std::endl;
}