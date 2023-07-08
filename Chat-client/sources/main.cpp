#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>

using boost::asio::ip::udp;
using boost::asio::ip::address;
using namespace boost::asio::ip;

udp::endpoint ep(address::from_string("127.0.0.1"), 8001);

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "");
	try
	{
		boost::system::error_code ec;
		boost::asio::io_context io_context;

		udp::socket s(io_context, udp::endpoint(udp::v4(), 0));

		std::cout << "Enter message: ";
		char request[1024];
		std::cin.getline(request, 1024);
		size_t request_length = std::strlen(request);
		s.send_to(boost::asio::buffer(request, request_length), udp::endpoint(make_address("26.12.225.238"), 7777));

		char reply[1024];
		udp::endpoint sender_endpoint;
		size_t reply_length = s.receive_from(
			boost::asio::buffer(reply, 1024), sender_endpoint);
		std::cout << "Reply is: ";
		std::cout.write(reply, reply_length);
		std::cout << "\n";
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}
	system("pause");
	return 0;
}