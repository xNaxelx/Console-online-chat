#include <iostream>
#include <boost/asio.hpp>
#include <thread>
#include <boost/bind.hpp>

using boost::asio::ip::udp;
using boost::asio::ip::address;
using namespace boost::asio::ip;

#define MAX_MESSAGE_LENGTH 1024 

void do_receive(udp::socket& socket_)
{
	while (true)
	{
		boost::system::error_code ec;
		char data_[MAX_MESSAGE_LENGTH];

		size_t bytes_recvd = socket_.receive(boost::asio::buffer(data_, MAX_MESSAGE_LENGTH), 0, ec);

		if (!ec && bytes_recvd > 0)
		{
			std::cout.write(data_, bytes_recvd);
			std::cout << "\n";
		}
		else if (ec.value() != 0)
		{
			std::cout << "Error: " << ec.message() << std::endl;
		}
	}
}

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "");

	boost::system::error_code ec;
	boost::asio::io_context io_context;
	udp::endpoint server_endpoint = udp::endpoint(make_address("127.0.0.1"), 7777);

	if (argc == 2)
	{
		server_endpoint = udp::endpoint(make_address(argv[1]), 7777);
	}
	if (argc == 3)
	{
		server_endpoint = udp::endpoint(make_address(argv[1]), std::atoi(argv[2]));
	}

	udp::socket s(io_context, udp::endpoint(udp::v4(), 0));

	std::thread t_receiver(do_receive, std::ref(s));
	t_receiver.detach();

	std::cout << "Enter message: ";

	while (true)
	{
		char message[MAX_MESSAGE_LENGTH];
		std::cin.getline(message, MAX_MESSAGE_LENGTH);
		size_t request_length = std::strlen(message);
		s.send_to(boost::asio::buffer(message, request_length), server_endpoint);

	}


	return 0;
}