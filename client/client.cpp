#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <string>


class Client
{
public:
    Client(boost::asio::io_context& context, boost::asio::ip::tcp::endpoint &ep, std::string query) : sock(context),endp(ep)
    {
        strcpy_s(request, query.c_str());
    }

    void ConnectToEndp()
    {
        sock.connect(endp);
    }

    void WriteRequest()
    {
	    sock.write_some(boost::asio::buffer(request));
    }

    void ReadAnwser()
    {
        while(true)
        {
			char buf[256];
            boost::system::error_code error;

        	sock.read_some(boost::asio::buffer(buf), error);

            if (error == boost::asio::error::eof)
            {
                break;
            }
            if (error)
            {
                throw boost::system::system_error(error);
            }
            std::cout <<"Adress:" << buf << std::endl;
        }
    }

    void Cicle()
	{
        ConnectToEndp();
        WriteRequest();
        ReadAnwser();
    }

private:
    boost::asio::ip::tcp::socket sock;
    char request[256];
    boost::asio::ip::tcp::endpoint endp;
};


int main()
{
    boost::asio::io_context context;

    boost::asio::ip::tcp::endpoint ep{
        boost::asio::ip::address::from_string("127.0.0.1"),
        6666
    };

    while(true)
    {
        std::cout << "enter the url:";
        std::string url;
        std::cin >> url;\
        Client cli(context, ep, url);

        cli.Cicle();
    }
	
}