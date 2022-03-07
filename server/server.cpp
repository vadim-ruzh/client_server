#include <iostream>
#include <queue>
#include <boost/asio.hpp>
#include <boost/asio/use_future.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <string>
#include <thread>

class Context
{
    using Task = std::function<void()>;

public:
    class Work;
    class Strand;

    Context()
        : m_isWorking(false)
    {
    }

	void post(Task task)
	{
        boost::unique_lock<boost::shared_mutex> tasksLock(m_tasksChange);
		m_tasks.push(task);
	}

    void poll()
	{
        Task task;
        {
            boost::upgrade_lock<boost::shared_mutex> sharedTasksLock(m_tasksChange);
            if (m_tasks.empty())
                return;

            boost::upgrade_to_unique_lock<boost::shared_mutex> uniqueTasksLock(sharedTasksLock);
            task = m_tasks.front();
            m_tasks.pop();

           /* {
                boost::shared_lock<boost::shared_mutex> tasksLock(m_tasksChange);
                if (m_tasks.empty())
                    return;
            }

	        {
		        boost::unique_lock<boost::shared_mutex> tasksLock(m_tasksChange);
            	task = m_tasks.front();
            	m_tasks.pop();
	        }*/
        }
        task();
	}

    void run()
    {
        bool empty = false;
	    do 
	    {
            {
                boost::shared_lock<boost::shared_mutex> tasksLock(m_tasksChange);
                empty = m_tasks.empty();
            }
            poll();
        } while (!empty || m_isWorking);
    }

private:
    boost::shared_mutex m_tasksChange;

    std::queue<Task> m_tasks;
    bool m_isWorking;
};

class Context::Work
{
public:
    Work(Context& context)
	    : m_context(context)
    {
        m_context.m_isWorking = true;
    }
    ~Work()
    {
        m_context.m_isWorking = false;
    }
private:
    Context& m_context;
};

class Context::Strand
{
public:
    Strand(Context& context)
        : m_context(context)
		, m_task(nullptr)
		, thread([this]() { Run(); })
    {
    }

    void post(Task task)
    {
        m_tasks.push(task);
        // m_context.post(task);

    }
    
private:
    void Run()
    {
        while (true)
        {
            if (!m_tasks.empty())
            {
                auto& task = m_tasks.front();
                bool& isProcessed = m_context.post(task);
                while (!isProcessed)
                {
                }
            }
        }
    }

    std::queue<Task> m_tasks;
    Context& m_context;
    boost::thread thread;

    Task* m_task;
};

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

void HandleConnection(
    boost::asio::io_context& context, 
    boost::asio::ip::tcp::acceptor &acc, 
    std::shared_ptr<boost::asio::ip::tcp::socket> sock, 
    boost::system::error_code ec)
{
    auto sock = std::make_shared<boost::asio::ip::tcp::socket>(boost::asio::make_strand(context));
    acc.async_accept(*sock, 
        boost::bind(&HandleConnection, std::ref(context), std::ref(acc), sock, boost::asio::placeholders::error));

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
    /*Context context;
    Context::Work work(context);
    Context::Strand strand(context);*/

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
    boost::thread runner2 {
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
