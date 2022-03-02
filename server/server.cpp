#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio/use_future.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <string>
#include <thread>

void ClientSession(std::shared_ptr<boost::asio::ip::tcp::socket> sock)
{
    char request[256];
    //Чтение запроса из сокета
    try
    {
        sock->async_read_some(boost::asio::buffer(request), boost::asio::use_future).get();
    }
    catch (const std::exception&)
    {

    }

    if(boost::this_thread::interruption_requested())
    {
	    return;
    }

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

void HandleConnection(boost::asio::io_context& context, boost::asio::ip::tcp::acceptor &acc, std::shared_ptr<boost::asio::ip::tcp::socket> sock, boost::system::error_code ec)
{
    auto sock = std::make_shared<boost::asio::ip::tcp::socket>(boost::asio::make_strand(context));
    acc.async_accept(*sock, boost::bind(&HandleConnection, std::ref(acc), sock, boost::asio::placeholders::error));
    /*
    [sock](boost::system::error_code ec)
    {
            ClientSession(sock, ec);
    }
     */

    if (!ec)
    {
        ClientSession(sock);
        //context.post([sock] { ClientSession(sock); });
    }

}

int main()
{
    boost::asio::io_context context;
    auto work = std::make_unique<boost::asio::io_context::work>(context);

    boost::thread runner {
        [&context]
        {
            try
		    {
		        context.run();
		    }
		    catch (...)
		    {

		    }
        }
    };
    boost::thread runner2{
        [&context]
        {
            try
            {
                context.run();
            }
            catch (...)
            {

            }
        }
    };

    //127.0.0.1:6666
    boost::asio::ip::tcp::endpoint endp{
        boost::asio::ip::tcp::v4(),6666 
    };

    //acceptor слушает ep
    boost::asio::ip::tcp::acceptor acc(context, endp);
    
    auto sock = std::make_shared<boost::asio::ip::tcp::socket>(boost::asio::make_strand(context));
    //Ожидание подключения и передача его в сокет 
    acc.async_accept(*sock, boost::bind(&HandleConnection, std::ref(acc), sock, boost::asio::placeholders::error));

    while(true)
    {
        bool isTerminate = false;
        if(isTerminate)
        {
            acc.cancel();

            sock->shutdown(boost::asio::ip::tcp::socket::shutdown_both);
            
            //acc.close();

            work.reset();

            runner.interrupt();
            runner2.interrupt();
            runner.join();
            runner2.join();

            context.stop();
        }
    }
}
