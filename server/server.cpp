#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <string>
#include <thread>


void ClientSession(boost::asio::ip::tcp::socket sock)
{
    char request[256];
    //Чтение запроса из сокета
    sock.read_some(boost::asio::buffer(request));

    //Поиск ip-адресса по заданному запросу
    boost::asio::io_context context;
    boost::asio::ip::tcp::resolver resolver{ context };
    auto query = boost::asio::ip::tcp::resolver::query{ request,"80" };

    boost::system::error_code errorCode;
    auto iter = resolver.resolve(query,errorCode);
    if(errorCode)
    {
        boost::asio::write(sock, boost::asio::buffer("adress not found"));
    }
    else
    {
        boost::asio::ip::tcp::endpoint anwser = *iter;
        char adress[256];
        strcpy_s(adress, anwser.address().to_string().c_str());

        std::cout << "recieve " << "=" << request << std::endl;
        std::cout << "send " << "=" << adress << std::endl;

        boost::asio::write(sock, boost::asio::buffer(adress));
    }

}

int main()
{
    boost::asio::io_context context;

    //127.0.0.1:6666
    boost::asio::ip::tcp::endpoint endp{
        boost::asio::ip::tcp::v4(),6666 
    };

    //acceptor слушает ep
    boost::asio::ip::tcp::acceptor acc(context, endp);
    while(true)
    {
        auto sock = boost::asio::ip::tcp::socket(context);
		//Ожидание подключения и передача его в сокет 
        acc.accept(sock);
        //Передаем в отдельный поток сокет и функцию клиентской сессии и отключаем его
        std::thread(ClientSession, std::move(sock)).detach();
    }

}
