#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <boost/bind.hpp>
#include <iostream>
#include <algorithm>

using boost::asio::ip::udp;
using namespace boost::asio;

#define MAX_MESSAGE_LENGTH 1024 

class Server
{
public:
    Server(boost::asio::io_context& io_context, thread_pool& pool, short port);

    void do_receive();

private:
    Server();

    void do_send(std::size_t length, char* buff, udp::endpoint& sender);
    void thread_send(udp::endpoint point, mutable_buffer Lbuff);
    udp::socket socket_;
    udp::endpoint temp_sender_endpoint_;
    std::list<udp::endpoint> users;
    char data_[MAX_MESSAGE_LENGTH];
    thread_pool* pool;
};


